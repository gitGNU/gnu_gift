#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <values.h>
#include <string.h>
#include <unistd.h>

#include <ppm.h>

/* PPM and PGM utilities */
 
/* David Squire 990520 */

double *ppm_local_normalize(PPM *im_input, int window_size, double (*variance_transformer)(double)) {
	
	int width, height;
	int i, j, x, y, n;
	double *dbl_image;
	double pixel, sum, sum_squares, mean, variance, normalized_pixel;
	char comment[80];

	if ((im_input->type == PPM_ASC) || (im_input->type == PPM_RAW)) {
		fprintf(stderr, "Can't normalize a PPM image!\n");
		return (NULL);
	}	

	/* allocate space for the normalized image */
	width = im_input->width;
	height = im_input->height;
	dbl_image = (double *)malloc(width*height*sizeof(double));
	/* move across the image, normalizing each pizel based on the
	 * statistics of the surrounding (square) window */
	for (x = 0; x < width; x++) {
	for (y = 0; y < height; y++) {
		/* calculate the statistics for the window */
		n = 0;
		sum = sum_squares = 0;
		for (i = -window_size/2; i < window_size/2; i++) {
		for (j = -window_size/2; j < window_size/2; j++) {
			if (((x + i) >= 0) && ((x + i) < width)
						&& ((y + j) >= 0) && ((y + j) < height)) {
				
				n++;
				pixel =	(double)(im_input->pixel[(y + j)*width + (x + i)])/255.0;
				sum += pixel;
				sum_squares += pixel*pixel;
			}
		}
		}
		mean = sum/(double)n;
		variance = sum_squares/(double)n - mean*mean;
		dbl_image[y*width + x] = (((double)im_input->pixel[y*width + x])/255 - mean)/sqrt(variance_transformer(variance));
		/* HACK: The mean should now be zero, and the variance 1, for each
		 * region (approximately). Clip any thing more than 4 std. devs.
		 * away from the mean, just to see if this improves visual
		 * appearance */
		 /*
		 if (dbl_image[y*width + x] > 4)
		 	dbl_image[y*width + x] = 4;
		 if (dbl_image[y*width + x] < -4)
		 	dbl_image[y*width + x] = -4;
			*/
	}
	}
	return (dbl_image);
}

PPM *ppm_double_to_byte(double *dbl_image, int width, int height) {

	PPM  *im_output;
	int x, y;
	double min = MAXFLOAT, max = -MAXFLOAT;

	/* set up the output PGM image */
	im_output = new_ppm();
	im_output->type = PGM_RAW;
	im_output->width = width;
	im_output->height = height;
	im_output->max_col_comp = 255; /* this is max. possible */
	im_output->bytes_per_pixel = 1;
	im_output->pixel = (byte *)malloc(im_output->width*im_output->height*sizeof(byte));

	/* find the minimum and maximum values of the input image */
	for (x = 0; x < width; x++) {
	for (y = 0; y < height; y++) {
		if (dbl_image[y*width + x] > max) {
			max = dbl_image[y*width + x];
		}
		else {
			if (dbl_image[y*width + x] < min)
				min = dbl_image[y*width + x];
		}
	}
	}
	
	/* now copy the input image to the output image, normalizing to the
	 * range [0,255] */
	for (x = 0; x < width; x++) {
	for (y = 0; y < height; y++) {
		im_output->pixel[y*width + x] = (byte)rint(255*(dbl_image[y*width + x] - min)/(max - min));
	}
	}

	return(im_output);
}

PPM *ppm_norm_contrast(PPM *im_input) {

	PPM  *im_output;
	int width, height;
	int x, y;
	double min = MAXFLOAT, max = -MAXFLOAT;

	/* set up the output PGM image */
	im_output = new_ppm();
	im_output->type = PGM_RAW;
	width = im_output->width = im_input->width;
	height = im_output->height = im_input->height;
	im_output->max_col_comp = 255; /* this is max. possible */
	im_output->bytes_per_pixel = 1;
	im_output->pixel = (byte *)malloc(im_output->width*im_output->height*sizeof(byte));

	/* find the minimum and maximum values of the input image */
	for (x = 0; x < width; x++) {
	for (y = 0; y < height; y++) {
		if (im_input->pixel[y*width + x] > max) {
			max = im_input->pixel[y*width + x];
		}
		else {
			if (im_input->pixel[y*width + x] < min)
				min = im_input->pixel[y*width + x];
		}
	}
	}
	
	/* now copy the input image to the output image, normalizing to the
	 * range [0,255] */
	for (x = 0; x < width; x++) {
	for (y = 0; y < height; y++) {
		im_output->pixel[y*width + x] = (byte)rint(255*(im_input->pixel[y*width + x] - min)/(max - min));
	}
	}

	return(im_output);
}

PPM *ppm_local_norm_contrast(PPM *im_input, int window_size) {
	
	PPM  *im_output;
	int width, height;
	int i, j, x, y;
	byte pixel;
	double max, min, sum, n;
	char comment[80];

	if ((im_input->type == PPM_ASC) || (im_input->type == PPM_RAW)) {
		fprintf(stderr, "Can't normalize a PPM image!\n");
		return (NULL);
	}	

	/* set up the output PGM image */
	im_output = new_ppm();
	im_output->type = PGM_RAW;
	width = im_output->width = im_input->width;
	height = im_output->height = im_input->height;
	im_output->max_col_comp = 255; /* this is max. possible */
	im_output->bytes_per_pixel = 1;
	im_output->pixel = (byte *)malloc(im_output->width*im_output->height*sizeof(byte));

	for (x = 0; x < width; x++) {
	for (y = 0; y < height; y++) {
		/* calculate the max and min pixel the window */
		min = 255;
		max = 0;
		sum = n = 0;
		for (i = -window_size/2; i < window_size/2; i++) {
		for (j = -window_size/2; j < window_size/2; j++) {
			if (((x + i) >= 0) && ((x + i) < width)
						&& ((y + j) >= 0) && ((y + j) < height)) {
				pixel = im_input->pixel[(y + j)*width + (x + i)];
				sum += pixel;
				n++;
				if (pixel > max)
					max = pixel;
				if (pixel < min)
					min = pixel;
			}
		}
		}
		im_output->pixel[y*width + x] = 128 + (byte)rint(127*((double)(im_input->pixel[y*width + x]) - sum/n)/(max - min));
	}
	}
	return (im_output);
}

