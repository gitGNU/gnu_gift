#include <math.h>
#include <stdlib.h>
#include <ppm.h>

int comp_int(const void *int1, const void *int2) {

	 return(*((int *)int1) - *((int *)int2));
 }

enum ppm_error median_filter_pgm(PPM *im_in, PPM **im_out) {
	
	/* Applies a 3x3 median filter to the image im_in, the result being
	returned in *im_out, the space for which is allocated in this routine.
	Uses an efficient median filter implementation, described in:

	Manfred Kopp and Werner Purgathofer. Efficient 3x3 median filter
	computations. Machine Vision and Graphics, 4(1/2):79-82, 1995. */

	/* Note: The images are must be 1 byte per pixel - these could be grey
	levels or indices into a colour map. */

	/* But until I get around to it, I will use the dumbest possible
	implementation! */

	int pixels[9];
	int num_pixels, median_pos;
	int x, y, i, j;

	if (im_in->bytes_per_pixel != 1) {
		fprintf(stderr, "median_filter_pgm: ");
		return(ONE_PLANE_NEEDED);
	}

	/* create the output image */
    /* create the PPM */
    (*im_out) = new_ppm();
    add_comment((*im_out), "# Image created by median filtering.\n");
    (*im_out)->type = PGM_RAW;
    (*im_out)->width = im_in->width;
    (*im_out)->height = im_in->height;
    (*im_out)->max_col_comp = im_in->max_col_comp;
    (*im_out)->bytes_per_pixel = 1;
    (*im_out)->pixel = (byte *)malloc((*im_out)->width*(*im_out)->height*sizeof(byte));

	/* do the median filtering */
	for (y = 0; y < im_in->height; y++) {
	for (x = 0; x < im_in->width; x++) {
		num_pixels = 0;
		for (j = -1; j <= 1; j++) {
		for (i = -1; i <= 1; i++) {
			if ( ((x + i) >= 0) &&
				 ((x + i) < im_in->width) &&
				 ((y + j) >= 0) &&
				 ((y + j) < im_in->height)
			   )
				pixels[num_pixels++] = im_in->pixel[(y+j)*im_in->width + x + i];
		}
		}
		qsort(pixels, num_pixels, sizeof(int), comp_int);
		median_pos = num_pixels/2;
		if (num_pixels % 2)
			median_pos += 1;
		(*im_out)->pixel[y*(*im_out)->width + x] = pixels[median_pos];
	}
	}

	return(PPM_OK);
}

enum ppm_error mode_filter_pgm(PPM *im_in, PPM **im_out) {
	
	/* Applies a 3x3 mode filter to the image im_in, the result being
	returned in *im_out, the space for which is allocated in this routine.

	/* Note: The images are must be 1 byte per pixel - these could be grey
	levels or indices into a colour map. */

	/* Until I get around to improving it, I will use the dumbest possible
	implementation! */

	int pixels[9];
	int *pixel_count;
	int num_pixels;
	int max, max_pixel;
	int x, y, i, j;

	if (im_in->bytes_per_pixel != 1) {
		fprintf(stderr, "mode_filter_pgm: ");
		return(ONE_PLANE_NEEDED);
	}

	/* create the output image */
    /* create the PPM */
    (*im_out) = new_ppm();
    add_comment((*im_out), "# Image created by mode filtering.\n");
    (*im_out)->type = PGM_RAW;
    (*im_out)->width = im_in->width;
    (*im_out)->height = im_in->height;
    (*im_out)->max_col_comp = im_in->max_col_comp;
    (*im_out)->bytes_per_pixel = 1;
    (*im_out)->pixel = (byte *)malloc((*im_out)->width*(*im_out)->height*sizeof(byte));

	/* allocate space for the pixel value counts */
	pixel_count = (int *)malloc((*im_out)->max_col_comp*sizeof(int));
	for (i = 0; i < (*im_out)->max_col_comp; i++)
		pixel_count[i] = 0;

	/* do the mode filtering */
	for (y = 0; y < im_in->height; y++) {
	for (x = 0; x < im_in->width; x++) {
		num_pixels = 0;
		for (j = -1; j <= 1; j++) {
		for (i = -1; i <= 1; i++) {
			if ( ((x + i) >= 0) &&
				 ((x + i) < im_in->width) &&
				 ((y + j) >= 0) &&
				 ((y + j) < im_in->height)
			   ) {
				pixels[num_pixels] = (int)im_in->pixel[(y+j)*im_in->width + x + i];
				pixel_count[pixels[num_pixels]]++;
				num_pixels++;
			}
		}
		}
		max = 0;
		for (i = 0; i < num_pixels; i++) {
			if (pixel_count[pixels[i]] > max) {
				max = pixel_count[pixels[i]];
				max_pixel = pixels[i];
				pixel_count[pixels[i]] = 0; /* for next pass */
			}
		}
		(*im_out)->pixel[y*(*im_out)->width + x] = (byte)max_pixel;
	}
	}

	return(PPM_OK);
}

enum ppm_error mean_filter_pgm(PPM *im_in, PPM **im_out) {
	
	/* Applies a 3x3 mean filter to the image im_in, the result being
	returned in *im_out, the space for which is allocated in this routine.

	/* Note: The images are must be 1 byte per pixel - these could be grey
	levels or indices into a colour map. */

	/* Until I get around to improving it, I will use the dumbest possible
	implementation! */

	int pixels[9];
	int num_pixels;
	double mean;
	int x, y, i, j;

	if (im_in->bytes_per_pixel != 1) {
		fprintf(stderr, "mode_filter_pgm: ");
		return(ONE_PLANE_NEEDED);
	}

	/* create the output image */
    /* create the PPM */
    (*im_out) = new_ppm();
    add_comment((*im_out), "# Image created by mean filtering.\n");
    (*im_out)->type = PGM_RAW;
    (*im_out)->width = im_in->width;
    (*im_out)->height = im_in->height;
    (*im_out)->max_col_comp = im_in->max_col_comp;
    (*im_out)->bytes_per_pixel = 1;
    (*im_out)->pixel = (byte *)malloc((*im_out)->width*(*im_out)->height*sizeof(byte));

	/* do the mean filtering */
	for (y = 0; y < im_in->height; y++) {
	for (x = 0; x < im_in->width; x++) {
		num_pixels = 0;
		mean = 0;
		for (j = -1; j <= 1; j++) {
		for (i = -1; i <= 1; i++) {
			if ( ((x + i) >= 0) &&
				 ((x + i) < im_in->width) &&
				 ((y + j) >= 0) &&
				 ((y + j) < im_in->height)
			   ) {
				mean += (double)im_in->pixel[(y+j)*im_in->width + x + i];
				num_pixels++;
			}
		}
		}
		(*im_out)->pixel[y*(*im_out)->width + x] = (byte)rint(mean/(double)num_pixels);
	}
	}

	return(PPM_OK);
}
