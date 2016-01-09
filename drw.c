/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>

#include "drw.h"
#include "util.h"

Drw *
drw_create(Display *dpy, int screen)
{
	Drw *drw;

	drw = ecalloc(1, sizeof(Drw));
	drw->dpy = dpy;
	drw->screen = screen;
	drw->scheme = NULL;
	return drw;
}

void
drw_free(Drw *drw) {
	free(drw);
}

Clr *
drw_clr_create(Drw *drw, const char *clrname)
{
	Clr *clr;

	clr = ecalloc(1, sizeof(Clr));
	if (!XftColorAllocName(drw->dpy, DefaultVisual(drw->dpy, drw->screen),
	                       DefaultColormap(drw->dpy, drw->screen),
	                       clrname, &clr->rgb))
		die("error, cannot allocate color '%s'\n", clrname);
	clr->pix = clr->rgb.pixel;

	return clr;
}

void
drw_clr_free(Clr *clr)
{
	free(clr);
}

void
drw_setscheme(Drw *drw, ClrScheme *scheme)
{
	drw->scheme = scheme;
}

Cur *
drw_cur_create(Drw *drw, int shape)
{
	Cur *cur;

	cur = ecalloc(1, sizeof(Cur));
	cur->cursor = XCreateFontCursor(drw->dpy, shape);

	return cur;
}

void
drw_cur_free(Drw *drw, Cur *cursor)
{
	if (!cursor)
		return;
	XFreeCursor(drw->dpy, cursor->cursor);
	free(cursor);
}
