#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <fft.h>
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

	/* create the filter kernels, if it has not already been done */
	if (kernel11 == NULL)
		create_filter_kernels();

	conv = (double *)calloc(width*height, sizeof(double));

	/* first convolution */
	for (x = 0; x < width; x++) {
	for (y = 0; y < height; y++) {
		output[y*width + x] = 0; /* might as well be here */
		for (t_x = -kernal_size[filter_scale]/2; t_x <= kernal_size[filter_scale]/2; t_x++) {
			if (((x - t_x) >= 0) && ((x - t_x) < width)) {
				conv[y*width + x] +=
					kernel11[filter_scale][n_theta][t_x + kernal_size[filter_scale]/2]*image[y*width+ (x - t_x)];
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
				kernel21[filter_scale][n_theta][t_x + kernal_size[filter_scale]/2]*image[y*width + (x - t_x)];
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

void main(int argc, char *argv[]) {
	
	char *in_fname, *out_fname, *out_fname_prefix, buffer[80];
	char *point_pos;
	FILE *ppm_file, *outfile;
	PPM *im_rgb, *im_hsv, *im_filtered;
	int *colmap, colmap_size;
	enum file_types ppm_type;
	enum ppm_error the_error;
	double *db_val_image, *filtered_image;	
	double max_energy;
	double min_energy;
	int scale, orientation;
	int i;

    switch(argc) {
    case 4:
        in_fname = argv[1];
		scale = atoi(argv[2]);
		orientation = atoi(argv[3]);
        break;
    default:
        fprintf(stderr, "Usage: %s ppm_file scale orientation\n\n", argv[0]);
        exit(1);
        break;
    }
 
    if ((ppm_file = fopen (in_fname, "r")) == NULL) {
        fprintf(stderr, "Can't open file: %s", in_fname);
        exit(1);
    }
 
    /* now get the filename prefix */
    out_fname_prefix = (char *)malloc((strlen(in_fname) + 40)*sizeof(char));
    out_fname = (char *)malloc((strlen(in_fname) + 40)*sizeof(char));
    if ((point_pos = strchr(in_fname, '.')) == NULL) {
        fprintf(stderr, "File %s has no ""."" - can't generate filtered image filenames\n\n", in_fname);
        exit(1);
    }
	strncpy(out_fname_prefix, in_fname, (int)(point_pos - in_fname));

	/* read the rgb image which we are going to filter */
	switch(ppm_type = read_magic_no(ppm_file)) {
	case PGM_ASC: case PPM_ASC: case PGM_RAW: case PPM_RAW:
		if ((the_error = read_ppm(ppm_file, &im_rgb, ppm_type)) != PPM_OK) {
			ppm_handle_error(the_error);
			exit(1);
		}
        break;
    default:
        fprintf(stderr, "Unrecognized file type.\n");
        exit(1);
        break;
    }

    /* convert it to hsv */
    if ((the_error = rgb2hsv_ppm(im_rgb, &im_hsv)) != PPM_OK) {
        ppm_handle_error(the_error);
        exit(1);
    }

	/* make space for the output filtered image */
    im_filtered = new_ppm();
    add_comment(im_filtered, "# Value image block quantized image.\n");
    im_filtered->type = PGM_RAW;
    im_filtered->width = im_hsv->width;
    im_filtered->height = im_hsv->height;
    im_filtered->max_col_comp = 255;
    im_filtered->bytes_per_pixel = 1;
    im_filtered->pixel = (byte *)malloc(im_hsv->width*im_hsv->height*sizeof(byte));

	/* extract the value plane */
	db_val_image = (double *)malloc(im_hsv->width*im_hsv->height*sizeof(double));
	filtered_image = (double *)malloc(im_hsv->width*im_hsv->height*sizeof(double));
	for (i = 0; i < im_hsv->width*im_hsv->height; i++)
		db_val_image[i] = (double)(im_hsv->pixel[3*i + VALUE]);
		
	/* now apply the filter */
	gabor_filter(db_val_image, im_hsv->width, im_hsv->height, scale, orientation, filtered_image);
	max_energy = 0;
	min_energy = 255*255;
	for (i = 0; i < im_hsv->width*im_hsv->height; i++) {
#ifdef GABOR_WRITE_FILTERED
		im_filtered->pixel[i] = (byte)sqrt(sq(filtered_image[i]));
#endif
		printf("%f\n", sq(filtered_image[i]));
		if (sq(filtered_image[i]) > max_energy)
			max_energy = sq(filtered_image[i]);
		if (sq(filtered_image[i]) < min_energy)
			min_energy = sq(filtered_image[i]);
	}
#ifdef GABOR_WRITE_FILTERED
	sprintf(out_fname, "%s_gabor_%d_%d.pgm", out_fname_prefix, scale, orientation);
	outfile = fopen(out_fname, "wb");
	if ((the_error = write_ppm(outfile, im_filtered, PGM_RAW)) != PPM_OK) {
		ppm_handle_error(the_error);
		exit(1);
	}
	fclose(outfile);
#endif
}
