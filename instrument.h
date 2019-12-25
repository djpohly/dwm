#include <stdio.h>
#include <sys/time.h>
#include <X11/Xlib.h>

static long insttimebase = -1;

#define instlog(...) do { \
		struct timeval t; \
		gettimeofday(&t, NULL); \
		if (insttimebase == -1) insttimebase = t.tv_sec; \
		t.tv_sec -= insttimebase; \
		fprintf(stderr, "[%ld.%06ld] ", t.tv_sec, t.tv_usec); \
		fprintf(stderr, __VA_ARGS__); \
	} while (0)

/* instrumentation */
static inline Status
wrap_XGetWindowAttributes(int line, Display *dpy, Window w,
		XWindowAttributes *war) {
	Status ret = XGetWindowAttributes(dpy, w, war);
	instlog("    (%d) XGetWindowAttributes(w=0x%lx) => %d\n",
			line, w, ret);
	return ret;
}
#define XGetWindowAttributes(...) wrap_XGetWindowAttributes(__LINE__, __VA_ARGS__)

static inline int
wrap_XChangeWindowAttributes(int line, Display *display, Window w,
		unsigned long valuemask, XSetWindowAttributes *attributes) {
	int ret = XChangeWindowAttributes(display, w, valuemask, attributes);
	instlog("    (%d) XChangeWindowAttributes(w=0x%lx) => %d\n",
			line, w, ret);
	return ret;
}
#define XChangeWindowAttributes(...) wrap_XChangeWindowAttributes(__LINE__, __VA_ARGS__)

static inline int
wrap_XSetWindowBorder(int line, Display *display, Window w, unsigned long bp) {
	int ret = XSetWindowBorder(display, w, bp);
	instlog("    (%d) XSetWindowBorder(w=0x%lx) => %d\n",
			line, w, ret);
	return ret;
}
#define XSetWindowBorder(...) wrap_XSetWindowBorder(__LINE__, __VA_ARGS__)

static inline int
wrap_XMapWindow(int line, Display *d, Window w) {
	int ret = XMapWindow(d, w);
	instlog("    (%d) XMapWindow(w=0x%lx) => %d\n",
			line, w, ret);
	return ret;
}
#define XMapWindow(...) wrap_XMapWindow(__LINE__, __VA_ARGS__)

static inline int
wrap_XUnmapWindow(int line, Display *d, Window w) {
	int ret = XUnmapWindow(d, w);
	instlog("    (%d) XUnmapWindow(w=0x%lx) => %d\n",
			line, w, ret);
	return ret;
}
#define XUnmapWindow(...) wrap_XUnmapWindow(__LINE__, __VA_ARGS__)

static inline int
wrap_XConfigureWindow(int line, Display *display, Window w,
		unsigned value_mask, XWindowChanges *changes) {
	int ret = XConfigureWindow(display, w, value_mask, changes);
	instlog("    (%d) XConfigureWindow(w=0x%lx) => %d\n",
			line, w, ret);
	return ret;
}
#define XConfigureWindow(...) wrap_XConfigureWindow(__LINE__, __VA_ARGS__)

static inline int
wrap_XMoveResizeWindow(int line, Display *d, Window w, int x, int y,
		unsigned width, unsigned height) {
	int ret = XMoveResizeWindow(d, w, x, y, width, height);
	instlog("    (%d) XMoveResizeWindow(w=0x%lx, %dx%d%+d%+d) => %d\n",
			line, w, width, height, x, y, ret);
	return ret;
}
#define XMoveResizeWindow(...) wrap_XMoveResizeWindow(__LINE__, __VA_ARGS__)

static inline int
wrap_XRaiseWindow(int line, Display *d, Window w) {
	int ret = XRaiseWindow(d, w);
	instlog("    (%d) XRaiseWindow(w=0x%lx) => %d\n",
			line, w, ret);
	return ret;
}
#define XRaiseWindow(...) wrap_XRaiseWindow(__LINE__, __VA_ARGS__)

static inline int
wrap_XSetInputFocus(int line, Display *d, Window w, int rt, Time t) {
	int ret = XSetInputFocus(d, w, rt, t);
	instlog("    (%d) XSetInputFocus(w=0x%lx) => %d\n",
			line, w, ret);
	return ret;
}
#define XSetInputFocus(...) wrap_XSetInputFocus(__LINE__, __VA_ARGS__)
