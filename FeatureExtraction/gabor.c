#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <ppm.h>

#include "gabor.h"

/* note that all this is taken from the paper JaH98 in ~/tex/bib/squizz.bib */
/* (with my corrections!) */

static double ***kernel11 = NULL;
static double ***kernel12 = NULL;
static double ***kernel21 = NULL;
static double ***kernel22 = NULL;
static int kernal_size[num_gabor_scales] = {gabor_kernel_size_0, gabor_kernel_size_1, gabor_kernel_size_2};

void save_norm_double_pgm(double *double_im, int w, int h, char *fname) {

	PPM *im;
	float *norm_im;
	int i;
	FILE *outfile;
	int the_error;

	/* copy the image, and then normalise the contrast */
	norm_im = (float *)malloc(w*h*sizeof(float));
	for (i = 0; i < w*h; i++)
		norm_im[i] = (float)double_im[i];
	normaliseContrast(norm_im, w*h);

	im = new_ppm();
	im->type = PGM_RAW;
	im->width = w;
	im->height = h;
	im->max_col_comp = 255;
	im->bytes_per_pixel = 1;
	im->pixel = (byte *)malloc(w*h*sizeof(byte));
	for (i = 0; i < w*h; i++)
		im->pixel[i] = (byte)norm_im[i];
	outfile = fopen(fname, "wb");
	if ((the_error = write_ppm(outfile, im, PGM_RAW)) != PPM_OK) {
		ppm_handle_error(the_error);
		exit(1);
	}
	fclose(outfile);
	destroy_ppm(&im);
	free(norm_im);
}

void create_filter_kernels() {

	int i, j, n;
	int x, x_c;
	double u0, u, v, sigma, theta;
	char fname[256];

	/* allocate space for all the filter kernels */
	kernel11 = (double ***)malloc(num_gabor_scales*sizeof(double **));
	kernel12 = (double ***)malloc(num_gabor_scales*sizeof(double **));
	kernel21 = (double ***)malloc(num_gabor_scales*sizeof(double **));
	kernel22 = (double ***)malloc(num_gabor_scales*sizeof(double **));
	for (i = 0; i < num_gabor_scales; i++) {
		kernel11[i] = (double **)malloc(num_gabors_per_scale*sizeof(double *));
		for (j = 0; j < num_gabors_per_scale; j++)
			kernel11[i][j] = (double *)malloc(kernal_size[i]*sizeof(double));
		kernel12[i] = (double **)malloc(num_gabors_per_scale*sizeof(double *));
		for (j = 0; j < num_gabors_per_scale; j++)
			kernel12[i][j] = (double *)malloc(kernal_size[i]*sizeof(double));
		kernel21[i] = (double **)malloc(num_gabors_per_scale*sizeof(double *));
		for (j = 0; j < num_gabors_per_scale; j++)
			kernel21[i][j] = (double *)malloc(kernal_size[i]*sizeof(double));
		kernel22[i] = (double **)malloc(num_gabors_per_scale*sizeof(double *));
		for (j = 0; j < num_gabors_per_scale; j++)
			kernel22[i][j] = (double *)malloc(kernal_size[i]*sizeof(double));
	}

	/* now set the values of the kernels */
	u0 = u00;
	for (i = 0; i < num_gabor_scales; i++) {
		sigma = sigma_m(u0);
		for (j = 0; j < num_gabors_per_scale; j++) {
			theta = j*theta_step;
			u = u0*cos(theta);
			v = u0*sin(theta);
			x_c = kernal_size[i]/2; /* since sizes are odd, this gives the
									       centre value */
			for (x = 0; x < kernal_size[i]; x++) {
				/* note that x is "y" for kernels 12 and 22 */
				kernel11[i][j][x] = (1/(sqrt(2*M_PI)*sigma)*exp(-sq((x - x_c))/(2*sq(sigma)))*cos(2*M_PI*u*(x - x_c)));
				kernel12[i][j][x] = (1/(sqrt(2*M_PI)*sigma)*exp(-sq((x - x_c))/(2*sq(sigma)))*cos(2*M_PI*v*(x - x_c)));
				kernel21[i][j][x] = (1/(sqrt(2*M_PI)*sigma)*exp(-sq((x - x_c))/(2*sq(sigma)))*sin(2*M_PI*u*(x - x_c)));
				kernel22[i][j][x] = (1/(sqrt(2*M_PI)*sigma)*exp(-sq((x - x_c))/(2*sq(sigma)))*sin(2*M_PI*v*(x - x_c)));
			}
		}
		u0 = u0/2;
	}
}

void gabor_filter(double *image, int width, int height, int filter_scale, int n_theta, double *output) {

	double *conv;
	int x, y, t_x, t_y;
	int i;

	conv = (double *)calloc(width*height, sizeof(double));

	/* first convolution */
	for (x = 0; x < width; x++) {
	for (y = 0; y < height; y++) {
		output[y*width + x] = 0; /* might as well be here */
		for (t_x = -kernal_size[filter_scale]/2; t_x <= kernal_size[filter_scale]/2; t_x++) {
			if (((x - t_x) >= 0) && ((x - t_x) < width)) {
				conv[y*width + x] +=
					kernel11[filter_scale][n_theta][t_x + kernal_size[filter_scale]/2]*image[65536-(y*width+ (x - t_x))];
			}
		}
	}
	}

	/* second convolution */
	for (x = 0; x < width; x++) {
	for (y = 0; y < height; y++) {
		for (t_y = -kernal_size[filter_scale]/2; t_y <= kernal_size[filter_scale]/2; t_y++) {
			if (((y - t_y) >= 0) && ((y - t_y) < height))
				output[y*width + x] +=
					kernel12[filter_scale][n_theta][t_y + kernal_size[filter_scale]/2]*conv[(y - t_y)*width + x];
		}
	}
	}

	for (i = 0; i < width*height; i++)
		conv[i] = 0;

	/* third convolution */
	for (x = 0; x < width; x++) {
	for (y = 0; y < height; y++) {
		for (t_x = -kernal_size[filter_scale]/2; t_x <= kernal_size[filter_scale]/2; t_x++) {
			if (((x - t_x) >= 0) && ((x - t_x) < width)) {
				conv[y*width + x] +=
				kernel21[filter_scale][n_theta][t_x + kernal_size[filter_scale]/2]*image[65536-(y*width + (x - t_x))];
			}
		}
	}
	}

	/* fourth convolution */
	for (x = 0; x < width; x++) {
	for (y = 0; y < height; y++) {
		for (t_y = -kernal_size[filter_scale]/2; t_y <= kernal_size[filter_scale]/2; t_y++) {
			if (((y - t_y) >= 0) && ((y - t_y) < height))
				output[y*width + x] -=
				kernel22[filter_scale][n_theta][t_y + kernal_size[filter_scale]/2]*conv[(y - t_y)*width + x];
		}
	}
	}

	free(conv);
}
