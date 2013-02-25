/* See LICENSE file for copyright and license details. */

typedef struct {
	unsigned long rgb;
} Clr;

typedef struct {
	Cursor cursor;
} Cur;

typedef struct {
	Clr *fg;
	Clr *bg;
	Clr *border;
} ClrScheme;

typedef struct {
	Display *dpy;
	int screen;
	ClrScheme *scheme;
} Drw;

typedef struct {
	unsigned int w;
	unsigned int h;
} Extnts;

/* Drawable abstraction */
Drw *drw_create(Display *dpy, int screen);
void drw_free(Drw *drw);

/* Colour abstraction */
Clr *drw_clr_create(Drw *drw, const char *clrname);
void drw_clr_free(Clr *clr);

/* Cursor abstraction */
Cur *drw_cur_create(Drw *drw, int shape);
void drw_cur_free(Drw *drw, Cur *cursor);

/* Drawing context manipulation */
void drw_setscheme(Drw *drw, ClrScheme *scheme);
