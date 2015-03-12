/* See LICENSE file for copyright and license details. */

/* key definitions */
#define MODKEY Mod4Mask

/* commands */
static const char *termcmd[]  = { "/usr/bin/env", "SHLVL=0", "urxvtc", NULL };
static const char *lockcmd[]  = { "xscreensaver-command", "-lock", NULL };
static const char *lowervolcmd[]  = { "amixer", "sset", "Master", "3%-", NULL };
static const char *raisevolcmd[]  = { "amixer", "sset", "Master", "3%+", NULL };
static const char *mutecmd[]  = { "amixer", "sset", "Master", "toggle", NULL };
static const char *playcmd[]  = { "mpc", "toggle", NULL };
static const char *stopcmd[]  = { "mpc", "stop", NULL };
static const char *prevcmd[]  = { "mpc", "prev", NULL };
static const char *nextcmd[]  = { "mpc", "next", NULL };
static const char *ffcmd[]  = { "mpc", "seek", "+20", NULL };
static const char *rewcmd[]  = { "mpc", "seek", "-20", NULL };
static const char *brightupcmd[] = { "xbacklight", "-inc", "10", "-time", "75", NULL };
static const char *brightdncmd[] = { "xbacklight", "-dec", "10", "-time", "75", NULL };


#include <X11/keysym.h>
#include <X11/XF86keysym.h>

static Key keys[] = {
	/* modifier                     key        function        argument */
	/*
	{ MODKEY,                       XK_r,      spawn,          dmenucmd },
	{ MODKEY|ShiftMask,             XK_r,      spawn,          rootcmd },
	{ MODKEY|ControlMask,           XK_r,      spawn,          runtermcmd },
	{ MODKEY,                       XK_c,      spawn,          colorcmd },
	{ MODKEY|ControlMask,           XK_t,      spawn,          todocmd },
	*/

	{ MODKEY|ShiftMask,             XK_Return, spawn,          termcmd },
	{ MODKEY,                       XK_space,  spawn,          lockcmd },
	{ 0,              XF86XK_AudioLowerVolume, spawn,          lowervolcmd},
	{ 0,              XF86XK_AudioRaiseVolume, spawn,          raisevolcmd},
	{ 0,                     XF86XK_AudioMute, spawn,          mutecmd},
	{ 0,                     XF86XK_AudioPlay, spawn,          playcmd},
	{ 0,                     XF86XK_AudioStop, spawn,          stopcmd},
	{ 0,                     XF86XK_AudioPrev, spawn,          prevcmd},
	{ 0,                     XF86XK_AudioNext, spawn,          nextcmd},
	{ 0,               XF86XK_MonBrightnessUp, spawn,          brightupcmd},
	{ 0,             XF86XK_MonBrightnessDown, spawn,          brightdncmd},
	{ MODKEY,                       XK_Next,   spawn,          lowervolcmd},
	{ MODKEY,                       XK_Prior,  spawn,          raisevolcmd},
	{ MODKEY|ShiftMask,             XK_Next,   spawn,          mutecmd},
	{ MODKEY,                       XK_Insert, spawn,          playcmd},
	{ MODKEY,                       XK_Delete, spawn,          stopcmd},
	{ MODKEY,                       XK_Home,   spawn,          prevcmd},
	{ MODKEY,                       XK_End,    spawn,          nextcmd},
	{ MODKEY,                       XK_Left,   spawn,          rewcmd},
	{ MODKEY,                       XK_Right,  spawn,          ffcmd},

	/*
	{ MODKEY,                    XK_BackSpace, print,          "restart" },
	{ MODKEY|ShiftMask,          XK_BackSpace, print,          "exit" },
	*/
	{ MODKEY,                       XK_t,      print,          "focusstack+" },
	{ MODKEY,                       XK_n,      print,          "focusstack-" },
	/*
	{ MODKEY,                       XK_Tab,    print,          "focusstack+" },
	{ MODKEY|ShiftMask,             XK_Tab,    print,          "focusstack-" },
	{ MODKEY|ShiftMask,             XK_t,      print,          "pushdown" },
	{ MODKEY|ShiftMask,             XK_n,      print,          "pushup" },
	{ MODKEY|ShiftMask,             XK_h,      print,          "incnmaster+" },
	{ MODKEY|ShiftMask,             XK_s,      print,          "incnmaster-" },
	{ MODKEY,                       XK_h,      print,          "setmfact-" },
	{ MODKEY,                       XK_s,      print,          "setmfact+" },
	{ MODKEY,                       XK_Return, print,          "zoom" },
	{ MODKEY,                       XK_Escape, print,          "killclient" },
	{ MODKEY|ShiftMask,             XK_l,      print,          "setlayout-tiled" },
	{ MODKEY,                       XK_l,      print,          "setlayout-bstack" },
	{ MODKEY|ShiftMask,             XK_f,      print,          "setlayout-float" },
	{ MODKEY,                       XK_f,      print,          "togglefloating" },
	{ MODKEY,                       XK_0,      print,          "viewall" },
	{ MODKEY|ShiftMask,             XK_0,      print,          "tag" },
	{ MODKEY,                   XK_apostrophe, print,          "focusmon0" },
	{ MODKEY,                        XK_comma, print,          "focusmon1" },
	{ MODKEY|ShiftMask,         XK_apostrophe, print,          "tagmon0" },
	{ MODKEY|ShiftMask,              XK_comma, print,          "tagmon1" },

#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,       print,          "view" #TAG}, \
	{ MODKEY|ControlMask,           KEY,       print,          "toggleview" #TAG}, \
	{ MODKEY|ShiftMask,             KEY,       print,          "tag" #TAG}, \
	{ MODKEY|ControlMask|ShiftMask, KEY,       print,          "toggletag" #TAG}

	TAGKEYS(XK_1, 0),
	TAGKEYS(XK_2, 1),
	TAGKEYS(XK_3, 2),
	TAGKEYS(XK_4, 3),
	TAGKEYS(XK_5, 4),
	TAGKEYS(XK_6, 5),
	TAGKEYS(XK_7, 6),
	TAGKEYS(XK_8, 7),
	TAGKEYS(XK_9, 8),
	*/
};
