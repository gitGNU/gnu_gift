#ifndef _GIFT_FEATURES
#define _GIFT_FEATURES
/* feature classes */

#define COL_POS		0	/* a colour in a specific position (and at a
						specific scale) */
#define COL_HST		1	/* the proportion of a given colour */
#define GABOR_POS	2	/* the value of a Gabor filter in a particular
						position */
#define GABOR_HST	3	/* the proportion of a particular value of a Gabor
						filter in the image */

#define FREQ_MAX 0xff     /* since we will use FLOATS to store frequencies we want high resolution (WM) */

typedef unsigned int freq_type;

typedef struct _FEATURE_DATA {
	int id;
	freq_type frequency; /* i.e. f = frequency/FREQ_MAX */
} FEATURE_DATA;


/* use primitive hacks by Wolfgang for 
   adapting to Wolfgang's code */
#define WOLFGANG_HACK 

/* fixed size buffer values, so that we dont code these into the .c files */
#define MAX_WIDTH  256
#define MAX_HEIGHT 256

#endif
