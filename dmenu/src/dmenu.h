#include <ctype.h>
#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xresource.h>
#include <X11/Xutil.h>
#ifdef XINERAMA
#include <X11/extensions/Xinerama.h>
#endif
#include <X11/Xft/Xft.h>

#include "drw.h"
#include "util.h"

// Macros {{{
#define INTERSECT(x,y,w,h,r)  (MAX(0, MIN((x)+(w),(r).x_org+(r).width)  - MAX((x),(r).x_org)) \
                             && MAX(0, MIN((y)+(h),(r).y_org+(r).height) - MAX((y),(r).y_org)))
#define LENGTH(X)             (sizeof X / sizeof X[0])
#define TEXTW(X)              (drw_fontset_getwidth(drw, (X)) + lrpad)
#define NUMBERSMAXDIGITS      100
#define NUMBERSBUFSIZE        (NUMBERSMAXDIGITS * 2) + 1
/* define opaqueness */
#define OPAQUE 0xFFU
// }}}

// Enums, struct and unions {{{
enum { SchemeNorm, SchemeSel, SchemeNormHighlight, SchemeSelHighlight,
       SchemeOut, SchemeLast }; /* color schemes */

struct item {
	char *text;
	struct item *left, *right;
	int out;
	double distance;
};

/* Xresources preferences */
enum resource_type {
	STRING = 0,
	INTEGER = 1,
	FLOAT = 2
};
typedef struct {
	char *name;
	enum resource_type type;
	void *dst;
} ResourcePref;
// }}}

// Variables {{{
static char numbers[NUMBERSBUFSIZE] = "";
static char text[BUFSIZ] = "";
static char *embed;
static int bh, mw, mh;
static int inputw = 0, promptw, passwd = 0;
static int lrpad; /* sum of left and right padding */
static size_t cursor;
static struct item *items = NULL;
static struct item *matches, *matchend;
static struct item *prev, *curr, *next, *sel;
static int mon = -1, screen;

static Atom clip, utf8;
static Display *dpy;
static Window root, parentwin, win;
static XIC xic;

static Drw *drw;
static int usergb = 0;
static Visual *visual;
static int depth;
static Colormap cmap;
static Clr *scheme[SchemeLast];
// }}}

// Functions {{{
static void load_xresources(void);
static void resource_load(XrmDatabase db, char *name, enum resource_type rtype, void *dst);
static char * cistrstr(const char *s, const char *sub);
static int (*fstrncmp)(const char *, const char *, size_t) = strncasecmp;
static char *(*fstrstr)(const char *, const char *) = cistrstr;
// }}}

// Configuration {{{
// -b  option; if 0, dmenu appears at bottom
static int topbar = 1;
// -c option; centers dmenu on screen
static int centered = 0;
// minimum width when centered
static int min_width = 600;
// -F  option; if 0, dmenu doesn't use fuzzy matching
static int fuzzy = 1;
// -fn option overrides fonts[0];
// default X11 font or font set
static const char *fonts[] = { "OperatorMonoLig Nerd Font:style:Book:size=12.0" };
// -p  option; prompt to the left of input field
static const char *prompt     = NULL;
static char normfgcolor[]     = "#95e6cb";
static char normbgcolor[]     = "#151a1e";
static char selfgcolor[]      = "#eaeaea";
static char selbgcolor[]      = "#36a3d9";
static char selhighfgcolor[]  = "#ff3333";
static char selhighbgcolor[]  = "#36a3d9";
static char normhighfgcolor[] = "#ff3333";
static char normhighbgcolor[] = "#151a1e";
static char *colors[SchemeLast][2] = {
									/*   fg                bg         */
	[SchemeNorm]          = { normfgcolor,     normbgcolor     },
	[SchemeSel]           = { selfgcolor,      selbgcolor      },
	[SchemeSelHighlight]  = { selhighfgcolor,  selhighbgcolor  },
	[SchemeNormHighlight] = { normhighfgcolor, normhighbgcolor },
	[SchemeOut]           = { "#000000",       "#ffffff"       },
};
// if 1 prompt uses SchemeSel, otherwise SchemeNorm
static int colorprompt = 1;
// Alpha
static const unsigned int bgalpha = 0xFFU;
static const unsigned int fgalpha = OPAQUE;
static const unsigned int alphas[SchemeLast][2] = {
	/*		fgalpha		bgalpha	*/
	[SchemeNorm] = { fgalpha, bgalpha },
	[SchemeSel] = { fgalpha, bgalpha },
	[SchemeOut] = { fgalpha, bgalpha },
};
// -l option; if nonzero, dmenu uses vertical list with given number of lines
static unsigned int lines = 0;
// Characters not considered part of a word while deleting words
// for example: " /?\"&[]"
static const char worddelimiters[] = " ";
// Xresources preferences to load at startup
ResourcePref resources[] = {
	{ "normfgcolor",     STRING, &normfgcolor },
	{ "normbgcolor",     STRING, &normbgcolor },
	{ "selfgcolor",      STRING, &selfgcolor },
	{ "selbgcolor",      STRING, &selbgcolor },
	{ "selhighfgcolor",  STRING, &selhighfgcolor },
	{ "selhighbgcolor",  STRING, &selhighbgcolor },
	{ "normhighfgcolor", STRING, &normhighfgcolor },
	{ "normhighbgcolor", STRING, &normhighbgcolor },
};
// }}}

// vim:fdm=marker
