#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#include "ppm.h"

/* PPM and PGM utilities */

/* David Squire 990423 */

PPM *ppm_subimage(PPM *source_image, int top_left_x, int top_left_y, int width, int height) {
	
	/* grab a widthxheight subimage from source_image, with top left corner
	 * at top_left_x, top_left_y. Use periodic extension of source_image, so
	 * subimage could in fact be bigger than source_image if wanted. */

	PPM *subimage;
	int x, y;
	int source_x, source_y;
	long sub_offset, source_offset;
	int bpp, p;
	
	subimage = new_ppm();
	subimage->type = source_image->type;
	subimage->width = width;
	subimage->height = height;
	subimage->max_col_comp = source_image->max_col_comp;
	bpp = subimage->bytes_per_pixel = source_image->bytes_per_pixel;
	subimage->pixel = (byte *)malloc(subimage->bytes_per_pixel*width*height*sizeof(byte));

	for (y = 0; y < height; y++) {
		source_y = (top_left_y + y) % source_image->height;
		for (x = 0; x < width; x++) {
			source_x = (top_left_x + x) % source_image->width;
			sub_offset = (y*width + x)*bpp;
			source_offset = (source_y*source_image->width + source_x)*bpp;
			for (p = 0; p < bpp; p++) {
				subimage->pixel[sub_offset + p] =
					source_image->pixel[source_offset + p];
			}
		}	
	}

	return(subimage);
	
}
