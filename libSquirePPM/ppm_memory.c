#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#include "ppm.h"

/* PPM and PGM utilities */

/* David Squire 980924 */

PPM *new_ppm() {
	
	PPM *the_ppm;

	the_ppm = (PPM *)malloc(sizeof(PPM));
	the_ppm->comments = NULL;
	the_ppm->comment_length = 0;
	the_ppm->pixel = NULL;

	return(the_ppm);
}

void destroy_ppm(PPM **the_ppm) {

	if ((*the_ppm)->comments != NULL)
		free((*the_ppm)->comments);
	if ((*the_ppm)->pixel != NULL)
		free((*the_ppm)->pixel);
	free(*the_ppm);
	*the_ppm = NULL;
}
