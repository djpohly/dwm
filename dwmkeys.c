/* See LICENSE file for copyright and license details.
 *
 * Key grabber for dwm.  Spawns things, or writes dwm commands to stdout.
 */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <X11/Xlib.h>
#include <X11/Xproto.h>

/* macros */
#define CLEANMASK(mask)         (mask & ~(numlockmask|LockMask) & (ShiftMask|ControlMask|Mod1Mask|Mod2Mask|Mod3Mask|Mod4Mask|Mod5Mask))
#define LENGTH(X)               (sizeof X / sizeof X[0])

typedef struct {
	unsigned int mod;
	KeySym keysym;
	void (*func)(const void *);
	const void *arg;
} Key;

/* function declarations */
static void cleanup(void);
static void grabkeys(void);
static void keypress(XEvent *e);
static void mappingnotify(XEvent *e);
static void print(const void *arg);
static void run(void);
static void sigchld(int unused);
static void spawn(const void *arg);
static void updatenumlockmask(void);

/* variables */
static int screen;
static unsigned int numlockmask = 0;
static Display *dpy;
static Window root;
static int (*xerrorxlib)(Display *, XErrorEvent *);

/* configuration, allows nested code to access above variables */
#include "dwmkeys.h"

/* function implementations */
void
cleanup(void) {
	XUngrabKey(dpy, AnyKey, AnyModifier, root);
	XSync(dpy, False);
}

void
grabkeys(void) {
	updatenumlockmask();
	{
		unsigned int i, j;
		unsigned int modifiers[] = { 0, LockMask, numlockmask, numlockmask|LockMask };
		KeyCode code;

		XUngrabKey(dpy, AnyKey, AnyModifier, root);
		for(i = 0; i < LENGTH(keys); i++)
			if((code = XKeysymToKeycode(dpy, keys[i].keysym)))
				for(j = 0; j < LENGTH(modifiers); j++)
					XGrabKey(dpy, code, keys[i].mod | modifiers[j], root,
						 True, GrabModeAsync, GrabModeAsync);
	}
}

void
keypress(XEvent *e) {
	unsigned int i;
	KeySym keysym;
	XKeyEvent *ev;

	ev = &e->xkey;
	keysym = XKeycodeToKeysym(dpy, (KeyCode)ev->keycode, 0);
	for(i = 0; i < LENGTH(keys); i++)
		if(keysym == keys[i].keysym
		&& CLEANMASK(keys[i].mod) == CLEANMASK(ev->state)
		&& keys[i].func)
			keys[i].func(keys[i].arg);
}

void
mappingnotify(XEvent *e) {
	XMappingEvent *ev = &e->xmapping;

	XRefreshKeyboardMapping(ev);
	if(ev->request == MappingKeyboard)
		grabkeys();
}

void
print(const void *arg) {
	printf("%s\n", (char *) arg);
}

void
run(void) {
	XEvent ev;
	/* main event loop */
	XSync(dpy, False);
	while(!XNextEvent(dpy, &ev)) {
		switch(ev.type) {
			case KeyPress:
				keypress(&ev);
				break;
			case MappingNotify:
				mappingnotify(&ev);
				break;
		}
	}
}

void
sigchld(int unused) {
	if(signal(SIGCHLD, sigchld) == SIG_ERR) {
		fprintf(stderr, "Can't install SIGCHLD handler\n");
		exit(EXIT_FAILURE);
	}
	while(0 < waitpid(-1, NULL, WNOHANG));
}

void
spawn(const void *arg) {
	if(fork() == 0) {
		if(dpy)
			close(ConnectionNumber(dpy));
		dup2(STDERR_FILENO, STDOUT_FILENO);
		setsid();
		execvp(((char **)arg)[0], (char **)arg);
		fprintf(stderr, "hotkeys: execvp %s", ((char **)arg)[0]);
		perror(" failed");
		exit(EXIT_SUCCESS);
	}
}

void
updatenumlockmask(void) {
	unsigned int i, j;
	XModifierKeymap *modmap;

	numlockmask = 0;
	modmap = XGetModifierMapping(dpy);
	for(i = 0; i < 8; i++)
		for(j = 0; j < modmap->max_keypermod; j++)
			if(modmap->modifiermap[i * modmap->max_keypermod + j]
			   == XKeysymToKeycode(dpy, XK_Num_Lock))
				numlockmask = (1 << i);
	XFreeModifiermap(modmap);
}

int
xerror(Display *dpy, XErrorEvent *ee)
{
	if (ee->error_code == BadWindow
	|| (ee->request_code == X_GrabKey && ee->error_code == BadAccess))
		return 0;
	fprintf(stderr, "dwmkeys: fatal error: request code=%d, error code=%d\n",
		ee->request_code, ee->error_code);
	return xerrorxlib(dpy, ee); /* may call exit */
}

int
main(int argc, char *argv[]) {
	if(!(dpy = XOpenDisplay(NULL))) {
		fprintf(stderr, "hotkeys: cannot open display\n");
		exit(EXIT_FAILURE);
	}
	/* clean up any zombies immediately */
	sigchld(0);

	/* init screen */
	root = RootWindow(dpy, screen);
	xerrorxlib = XSetErrorHandler(xerror);
	/* select for events */
	grabkeys();
	/* output lines as soon as printed */
	setlinebuf(stdout);
	run();
	cleanup();
	XCloseDisplay(dpy);
	return EXIT_SUCCESS;
}
