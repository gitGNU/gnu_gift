#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#include "ppm.h"

/* PPM and PGM utilities */

/* David Squire 980924 */

void ppm_handle_error(enum ppm_error the_error) {
	switch(the_error) {
		case UNKNOWN_TYPE:
			fprintf(stderr, "This is not a PPM/PGM file.\n");
			break;
		case EARLY_EOF:
			fprintf(stderr, "Unexpected end of file.\n");
			break;
		case READ_ERROR:
			fprintf(stderr, "Error reading file.\n");
			break;
		case COL_TOO_LARGE:
			fprintf(stderr, "Maximum colour value too large for raw format.\n");
			break;
		case ONE_PLANE_IN:
			fprintf(stderr, "Cannot save a plane of a greyscale image.\n");
			break;
		case ONE_PLANE_OUT:
			fprintf(stderr, "Cannot save a plane as a colour image.\n");
			break;
		case THREE_PLANES_NEEDED:
			fprintf(stderr, "Input image must have three planes.\n");
			break;
		case ONE_PLANE_NEEDED:
			fprintf(stderr, "Input image must have one plane.\n");
			break;
		case TOO_MANY_BINS:
			fprintf(stderr, "Quantisation can result in at most 256 bins.\n");
			break;
		default:
			break;
	}
}
