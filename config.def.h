/* See LICENSE file for copyright and license details. */

#include "push.h"

/* appearance */
static const char font[]            = "-*-terminus-medium-r-*-*-16-*-*-*-*-*-*-*";
static const char normbordercolor[] = "#2e3435";
static const char normbgcolor[]     = "#000000";
static const char normfgcolor[]     = "#babdb6";
static const char selbordercolor[]  = "#eeeeec";
static const char *selbgcolor       = normfgcolor;
static const char *selfgcolor       = normbgcolor;
static const char urgbgcolor[]      = "#204a87";
static const char urgfgcolor[]      = "#729fcf";
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int topgap    = 19;       /* gap at top of screen */

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "MPlayer",   NULL,       NULL,       0,            True,        -1 },
	{ "mplayer2",  NULL,       NULL,       0,            True,        -1 },
	{ "mpv",       NULL,       NULL,       0,            True,        -1 },
	{ "qemu",      NULL,       NULL,       0,            True,        -1 },
	{ "Qsynth",    NULL,       NULL,       0,            True,        -1 },
	{ "fontforge", NULL,       NULL,       0,            True,        -1 },
	{ "Vncviewer", NULL,       NULL,       0,            True,        -1 },
	{ "StepMania", NULL,       NULL,       0,            True,        -1 },
	{ "Dolphin-emu", NULL,     NULL,       0,            True,        -1 },
};

/* layout(s) */
static const float mfact      = 0.5;  /* factor of master area size [0.05..0.95] */
static const int nmaster      = 1;    /* number of clients in master area */
static const Bool resizehints = False; /* True means respect size hints in tiled resizals */

#include "bstack.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "TTT",      bstack },
	{ "><>",      NULL },    /* no layout function means floating behavior */
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "/bin/sh", "-c",
	"cmd=`dmenu_path | dmenu -m $0` && "
	"exec \"${SHELL:-/bin/sh}\" -c \"$cmd\"",
	dmenumon, NULL };
static const char *rootcmd[]  = { "/bin/sh", "-c",
	"cmd=`dmenu_path | rdmenu -m $0` && "
	"SUDO_ASKPASS=~/bin/dpass exec sudo -A \"${SHELL:-/bin/sh}\" -c \"$cmd\"",
	dmenumon, NULL };
static const char *runtermcmd[] = { "/bin/sh", "-c",
	"cmd=`dmenu_path | tdmenu -m $0` && "
	"SHLVL=0 exec urxvtc -title \"$cmd\" -e \"${SHELL:-/bin/sh}\" -c \"$cmd\"",
	dmenumon, NULL };
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
static const char *colorcmd[] = { "/bin/sh", "-c",
	"stint | awk '{hex=sprintf(\"#%02x%02x%02x\",$1,$2,$3);"
		"printf \"^bg(%s)          ^bg() %d %d %d (%s) ^bg(%s)          ^bg()\\n\","
		"hex,$1,$2,$3,hex,hex;fflush(stdout)}' | dzen2 -p 6 -xs $(($0+1))",
	dmenumon, NULL };
static const char *todocmd[]  = { "/bin/sh", "-c",
	"ls ~/.tasks | dmenu -m $0 -p TODO: | "
		"(read list task && [ -n \"$list\" -a -n \"$task\" ] && "
		" t --task-dir ~/.tasks --list \"$list\" \"$task\")",
	dmenumon, NULL };


#include <X11/XF86keysym.h>

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_r,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_r,      spawn,          {.v = rootcmd } },
	{ MODKEY|ControlMask,           XK_r,      spawn,          {.v = runtermcmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_space,  spawn,          {.v = lockcmd } },
	{ MODKEY,                       XK_c,      spawn,          {.v = colorcmd } },
	{ MODKEY|ControlMask,           XK_t,      spawn,          {.v = todocmd } },
	{ MODKEY,                       XK_t,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_n,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_Tab,    focusstack,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_Tab,    focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_t,      pushdown,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_n,      pushup,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_h,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_s,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_s,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Escape, killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_l,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_l,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY|ShiftMask,             XK_f,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_f,      togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_apostrophe,  focusmon,  {.ui = 0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.ui = 1 } },
	{ MODKEY|ShiftMask,             XK_apostrophe,  tagmon,    {.ui = 0 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.ui = 1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY,                       XK_BackSpace,     state,   {.ui = StRestart} },
	{ MODKEY|ShiftMask,             XK_BackSpace,     state,   {.ui = StShutdown} },
	{ 0,                            XF86XK_AudioLowerVolume, spawn, {.v = lowervolcmd} },
	{ 0,                            XF86XK_AudioRaiseVolume, spawn, {.v = raisevolcmd} },
	{ 0,                            XF86XK_AudioMute, spawn,   {.v = mutecmd} },
	{ 0,                            XF86XK_AudioPlay, spawn,   {.v = playcmd} },
	{ 0,                            XF86XK_AudioStop, spawn,   {.v = stopcmd} },
	{ 0,                            XF86XK_AudioPrev, spawn,   {.v = prevcmd} },
	{ 0,                            XF86XK_AudioNext, spawn,   {.v = nextcmd} },
	{ 0,                            XF86XK_MonBrightnessUp, spawn, {.v = brightupcmd} },
	{ 0,                            XF86XK_MonBrightnessDown, spawn, {.v = brightdncmd} },
	{ MODKEY,                       XK_Next,   spawn,          {.v = lowervolcmd} },
	{ MODKEY,                       XK_Prior,  spawn,          {.v = raisevolcmd} },
	{ MODKEY|ShiftMask,             XK_Next,   spawn,          {.v = mutecmd} },
	{ MODKEY,                       XK_Insert, spawn,          {.v = playcmd} },
	{ MODKEY,                       XK_Delete, spawn,          {.v = stopcmd} },
	{ MODKEY,                       XK_Home,   spawn,          {.v = prevcmd} },
	{ MODKEY,                       XK_End,    spawn,          {.v = nextcmd} },
	{ MODKEY,                       XK_Left,   spawn,          {.v = rewcmd} },
	{ MODKEY,                       XK_Right,  spawn,          {.v = ffcmd} },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
};

static const char *dwmfifo = "/tmp/dwm.fifo";
static Command commands[] = {
	{ "dmenu",           spawn,          {.v = dmenucmd} },
	{ "term",            spawn,          {.v = termcmd} },
	{ "togglebar",       togglebar,      {0} },
	{ "focusstack+",     focusstack,     {.i = +1} },
	{ "focusstack-",     focusstack,     {.i = -1} },
	{ "incnmaster+",     incnmaster,     {.i = +1} },
	{ "incnmaster-",     incnmaster,     {.i = -1} },
	{ "setmfact+",       setmfact,       {.f = +0.05} },
	{ "setmfact-",       setmfact,       {.f = -0.05} },
	{ "zoom",            zoom,           {0} },
	{ "view",            view,           {0} },
	{ "killclient",      killclient,     {0} },
	{ "setlayout-tiled", setlayout,      {.v = &layouts[0]} },
	{ "setlayout-float", setlayout,      {.v = &layouts[1]} },
	{ "setlayout-mono",  setlayout,      {.v = &layouts[2]} },
	{ "togglelayout",    setlayout,      {0} },
	{ "togglefloating",  togglefloating, {0} },
	{ "viewall",         view,           {.ui = ~0} },
	{ "tag",             tag,            {.ui = ~0} },
	{ "focusmon+",       focusmon,       {.i = +1} },
	{ "focusmon-",       focusmon,       {.i = -1} },
	{ "tagmon+",         tagmon,         {.i = +1} },
	{ "tagmon-",         tagmon,         {.i = -1} },
	{ "view1",           view,           {.ui = 1 << 0} },
	{ "view2",           view,           {.ui = 1 << 1} },
	{ "view3",           view,           {.ui = 1 << 2} },
	{ "view4",           view,           {.ui = 1 << 3} },
	{ "view5",           view,           {.ui = 1 << 4} },
	{ "view6",           view,           {.ui = 1 << 5} },
	{ "view7",           view,           {.ui = 1 << 6} },
	{ "view8",           view,           {.ui = 1 << 7} },
	{ "view9",           view,           {.ui = 1 << 8} },
	{ "toggleview1",     toggleview,     {.ui = 1 << 0} },
	{ "toggleview2",     toggleview,     {.ui = 1 << 1} },
	{ "toggleview3",     toggleview,     {.ui = 1 << 2} },
	{ "toggleview4",     toggleview,     {.ui = 1 << 3} },
	{ "toggleview5",     toggleview,     {.ui = 1 << 4} },
	{ "toggleview6",     toggleview,     {.ui = 1 << 5} },
	{ "toggleview7",     toggleview,     {.ui = 1 << 6} },
	{ "toggleview8",     toggleview,     {.ui = 1 << 7} },
	{ "toggleview9",     toggleview,     {.ui = 1 << 8} },
	{ "tag1",            tag,            {.ui = 1 << 0} },
	{ "tag2",            tag,            {.ui = 1 << 1} },
	{ "tag3",            tag,            {.ui = 1 << 2} },
	{ "tag4",            tag,            {.ui = 1 << 3} },
	{ "tag5",            tag,            {.ui = 1 << 4} },
	{ "tag6",            tag,            {.ui = 1 << 5} },
	{ "tag7",            tag,            {.ui = 1 << 6} },
	{ "tag8",            tag,            {.ui = 1 << 7} },
	{ "tag9",            tag,            {.ui = 1 << 8} },
	{ "toggletag1",      toggletag,      {.ui = 1 << 0} },
	{ "toggletag2",      toggletag,      {.ui = 1 << 1} },
	{ "toggletag3",      toggletag,      {.ui = 1 << 2} },
	{ "toggletag4",      toggletag,      {.ui = 1 << 3} },
	{ "toggletag5",      toggletag,      {.ui = 1 << 4} },
	{ "toggletag6",      toggletag,      {.ui = 1 << 5} },
	{ "toggletag7",      toggletag,      {.ui = 1 << 6} },
	{ "toggletag8",      toggletag,      {.ui = 1 << 7} },
	{ "toggletag9",      toggletag,      {.ui = 1 << 8} },
};
