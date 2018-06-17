/* See LICENSE file for copyright and license details. */

typedef struct {
	Cursor cursor;
} Cur;

enum { ColFg, ColBg, ColBorder }; /* Clr scheme index */
typedef XftColor Clr;

typedef struct {
	Display *dpy;
	int screen;
	Clr *scheme;
} Drw;

/* Drawable abstraction */
Drw *drw_create(Display *dpy, int screen);
void drw_free(Drw *drw);

/* Colorscheme abstraction */
void drw_clr_create(Drw *drw, Clr *dest, const char *clrname);
Clr *drw_scm_create(Drw *drw, const char *clrnames[], size_t clrcount);

/* Cursor abstraction */
Cur *drw_cur_create(Drw *drw, int shape);
void drw_cur_free(Drw *drw, Cur *cursor);

/* Drawing context manipulation */
void drw_setscheme(Drw *drw, Clr *scm);
