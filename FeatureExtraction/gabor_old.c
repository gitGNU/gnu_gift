#include <math.h>
#include <ppm.h>
#include <fft.h>

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
		x_c = kernal_size[i]/2; /* since the filter sizes are odd,
										 this gives the centre pixel */
		for (j = 0; j < num_gabors_per_scale; j++) {
			theta = j*theta_step;
			u = u0*cos(theta);
			v = u0*sin(theta);
			for (x = 0; x < kernal_size[i]; x++) {
				/* note that x is "y" for kernels 12 and 22 */
				kernel11[i][j][x] = gabor_f11(sigma, x - x_c, u);
#ifdef DEBUG
				fprintf(stderr, "kernel11[%d][%d][%d] = %f\n", i, j, x, kernel11[i][j][x]);
#endif
				kernel11[i][j][x] = gabor_f11(sigma, x - x_c, u);
				kernel12[i][j][x] = gabor_f12(sigma, x - x_c, v);
				kernel21[i][j][x] = gabor_f21(sigma, x - x_c, u);
				kernel22[i][j][x] = gabor_f22(sigma, x - x_c, v);
			}
		}
		u0 = u0/2;
	}
}

void gabor_filter(double *I, int block_size, int filter_scale, int n_theta, double *output) {

	double *conv;
	int x, y, t_x, t_y;
	int i;

	/* create the filter kernels, if it has not already been done */
	if (kernel11 == NULL)
		create_filter_kernels();

	conv = (double *)calloc(sq(block_size), sizeof(double));

	/* first convolution */
	for (x = 0; x < block_size; x++) {
	for (y = 0; y < block_size; y++) {
		output[y*block_size + x] = 0; /* might as well be in this loop */
		for (t_x = x - kernal_size[filter_scale]/2; t_x <= kernal_size[filter_scale]/2; t_x++) {
			if (((x - t_x) >= 0) && ((x - t_x) < block_size))
				conv[y*block_size + x] +=
					kernel11[filter_scale][n_theta][t_x + kernal_size[filter_scale]/2]*I[y*block_size + (x - t_x)];
		}
	}
	}
	save_norm_double_pgm(conv, block_size, block_size, "Conv1.pgm");


	/* second convolution */
	for (x = 0; x < block_size; x++) {
	for (y = 0; y < block_size; y++) {
		for (t_y = y - kernal_size[filter_scale]/2; t_y <= kernal_size[filter_scale]/2; t_y++) {
			if (((y - t_y) >= 0) && ((y - t_y) < block_size))
				output[y*block_size + x] +=
					kernel12[filter_scale][n_theta][t_y + kernal_size[filter_scale]/2]*conv[(y - t_y)*block_size + x];
		}
	}
	}
	save_norm_double_pgm(output, block_size, block_size, "Conv2.pgm");

	for (i = 0; i < sq(block_size); i++) 
		conv[i] = 0; /* get it ready for next step */

	/* third convolution */
	for (x = 0; x < block_size; x++) {
	for (y = 0; y < block_size; y++) {
		for (t_x = x - kernal_size[filter_scale]/2; t_x <= kernal_size[filter_scale]/2; t_x++) {
			if (((x - t_x) >= 0) && ((x - t_x) < block_size)) {
				conv[y*block_size + x] +=
				kernel21[filter_scale][n_theta][t_x + kernal_size[filter_scale]/2]*I[y*block_size + (x - t_x)];
			}
		}
	}
	}
	save_norm_double_pgm(conv, block_size, block_size, "Conv3.pgm");

	/* fourth convolution */
	for (x = 0; x < block_size; x++) {
	for (y = 0; y < block_size; y++) {
		for (t_y = y - kernal_size[filter_scale]/2; t_y <= kernal_size[filter_scale]/2; t_y++) {
			if (((y - t_y) >= 0) && ((y - t_y) < block_size))
				output[y*block_size + x] -=
				kernel22[filter_scale][n_theta][t_y + kernal_size[filter_scale]/2]*conv[(y - t_y)*block_size + x];
		}
	}
	}
	save_norm_double_pgm(output, block_size, block_size, "Conv4.pgm");

	free(conv);
}

int main(int argc, char *argv[]) {
	
	double *impulse = NULL;
	double *impulse_response = NULL;
	int im_size;
	char out_fname[256];
	int i, j, x, y;
	int scale = 0;
	double u0, u, v, sigma, theta;

	/***** TEST *****/

	/*
	*
	*
	scale = 2;
	u0 = u00;
	for (i = 0; i < scale; i++)
		u0 /= 2;
	dprint(u0);
	sigma = 1/(3*sigma_m(u0));
	dprint(sigma);
	theta = theta_step;
	u = u0*cos(theta);
	v = u0*sin(theta);
	dprint(u);
	dprint(v);
	im_test->width = 3*kernal_size[scale];
	im_test->height = 3*kernal_size[scale];
	free(im_test->pixel);
	im_test->pixel = (byte *)malloc(sq(3*kernal_size[scale])*sizeof(byte));
	impulse = (double *)calloc(sq(3*kernal_size[scale]), sizeof(double));
	fimpulse_response = (float *)calloc(sq(3*kernal_size[scale]), sizeof(float));
	for (i = 0; i < 3*kernal_size[scale]; i++) {
	x = i - (3*kernal_size[scale])/2;
	for (j = 0; j < 3*kernal_size[scale]; j++) {
		y = j - (3*kernal_size[scale])/2;
		impulse[j*3*kernal_size[scale] + i] =
			gabor_f11(sigma, (double)x, u)*gabor_f12(sigma, (double)y, v) -
			gabor_f21(sigma, (double)x, u)*gabor_f22(sigma, (double)y, v);
		fimpulse_response[j*3*kernal_size[scale] + i] = (float)impulse[j*3*kernal_size[scale] + i];
	}
	}
	normaliseContrast(fimpulse_response, sq(3*kernal_size[scale]));
	for (j = 0; j < sq(3*kernal_size[scale]); j++)
		im_test->pixel[j] = (byte)fimpulse_response[j];
	sprintf(out_fname, "impulse.pgm");
	outfile = fopen(out_fname, "wb");
	if ((the_error = write_ppm(outfile, im_test, PGM_RAW)) != PPM_OK) {
		ppm_handle_error(the_error);
		exit(1);
	}
	fclose(outfile);
	*
	*
	*/

	/* generate images of the impulse response of the 1st filter at each
	scale. */
	for (i = 0; i < num_gabor_scales; i++) {
		if (impulse != NULL)
			free(impulse);
		if (impulse_response != NULL)
			free(impulse_response);
		im_size = kernal_size[i];
		impulse = (double *)calloc(sq(im_size), sizeof(double));
		impulse_response = (double *)calloc(sq(im_size), sizeof(double));
		impulse[((im_size/2)*im_size) + im_size/2] = 1;
		gabor_filter(impulse, im_size, i ,0, impulse_response);

		sprintf(out_fname, "impulse_%d.pgm", i);
		save_norm_double_pgm(impulse, im_size, im_size, out_fname);
		sprintf(out_fname, "impulse_response_%d.pgm", i);
		save_norm_double_pgm(impulse_response, im_size, im_size, out_fname);
	}
}
