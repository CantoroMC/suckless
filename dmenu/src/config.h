// Default settings; can be overridden by command line.

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
static const char *fonts[] = {
	"OperatorMonoLig Nerd Font:style:Book:size=12.0"
};

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
