#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
/* for memset(), others */
#include <string.h>
#include <unistd.h>
#include <ppm.h>

#include "gabor.h"

/* for uint32_t */
#include <stdint.h>

/* note that all this is taken from the paper JaH98 in ~/tex/bib/squizz.bib */
/* (with my corrections!) */

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

void create_filter_kernels(double ** kernelsxy) {

	uint32_t i, j;
	int32_t x, x_c; /* ints to force type promotion later */
	double u0, u, v, sigma, theta;

	/* set the values of the kernels */
	u0 = u00;
	for (i = 0; i < num_gabor_scales; i++) {
		sigma = sigma_m(u0);
		for (j = 0; j < num_gabors_per_scale; j++) {
			theta = j*theta_step;
			u = u0*cos(theta);
			v = u0*sin(theta);
			x_c = kernal_size[i]/2; /* since sizes are odd, this gives the
									       centre value */

			kernelsxy[(i*num_gabors_per_scale+j)] = (double *)malloc(kernal_size[i]*sizeof(double)*4);
			for (x = 0; x < kernal_size[i]; x++) {
				/* note that x is "y" for kernels 12 and 22 */
				kernelsxy[(i*num_gabors_per_scale+j)][x] = (1/(sqrt(2*M_PI)*sigma)*exp(-sq((x - x_c))/(2*sq(sigma)))*cos(2*M_PI*u*(x - x_c)));
				kernelsxy[(i*num_gabors_per_scale+j)][x+kernal_size[i]] = (1/(sqrt(2*M_PI)*sigma)*exp(-sq((x - x_c))/(2*sq(sigma)))*cos(2*M_PI*v*(x - x_c)));                    
				kernelsxy[(i*num_gabors_per_scale+j)][x+kernal_size[i]*2] = (1/(sqrt(2*M_PI)*sigma)*exp(-sq((x - x_c))/(2*sq(sigma)))*sin(2*M_PI*u*(x - x_c)));
				kernelsxy[(i*num_gabors_per_scale+j)][x+kernal_size[i]*3] = (1/(sqrt(2*M_PI)*sigma)*exp(-sq((x - x_c))/(2*sq(sigma)))*sin(2*M_PI*v*(x - x_c)));
			}
		}
		u0 = u0/2;
	}
}

/* conv, conv2, and output need to be cleared before feeding them to this function. */
/* conv and conv2 are just temporary space, for juggling image data between filters */
void gabor_filter(double *image, int width, int height, int filter_scale, int orientation, double **kernelsxy, double *conv, double *conv2, double *output) {

	uint32_t x, y;
	uint32_t k;
	double * target_kernal;
	double * target_conv;
	double * target_image;
	double temparray[kernal_size[2]];

	/* first convolution */
	target_kernal=kernelsxy[filter_scale*num_gabors_per_scale+orientation];
	for (x = 0; x < width; x++) {
	for (y = 0; y < height; y++) {
		target_image=&image[(width*height)-(y*width+x+kernal_size[filter_scale]/2)];
		if ((x>=kernal_size[filter_scale]/2) && ((x+kernal_size[filter_scale]/2)<width))
		  {
		    for (k = 0; k < kernal_size[filter_scale]; k++)
		      temparray[k]= target_kernal[k]*target_image[k];
		    for (k = 0; k < kernal_size[filter_scale]; k++)
		      conv[((width*height)-1)-(x*height+y)] += temparray[k];
		  }
		else
		  {
		for (k=0; k < kernal_size[filter_scale]; k++) {
			if ((x+kernal_size[filter_scale]/2 >= k) && (x+kernal_size[filter_scale]/2 < width+k)) {
				conv[((width*height)-1)-(x*height+y)] +=
					target_kernal[k]*target_image[k];
			}
		}
		  }
	}
	}

	/* second convolution */
	target_kernal=&target_kernal[kernal_size[filter_scale]];
	for (x = 0; x < width; x++) {
	for (y = 0; y < height; y++) {
		target_conv=&conv[((width*height)-1)-(x*height+y+(kernal_size[filter_scale]/2))];
		if (((y>=kernal_size[filter_scale]/2)) && ((y+kernal_size[filter_scale]/2)<height))
		  {
		    /* first do the matrix multiply, then do the summation, so the matrix multiply can be vectored. */
		    for (k = 0; k < kernal_size[filter_scale]; k++)
		      temparray[k] = target_kernal[k]*target_conv[k];
		    for (k = 0; k < kernal_size[filter_scale]; k++)
		      output[y*width+x] += temparray[k];
		  }
		else
		  {
		for (k = 0; k < kernal_size[filter_scale]; k++) {
			if (((y+(kernal_size[filter_scale]/2))>=k) && (y+(kernal_size[filter_scale]/2)<height+k))
				output[y*width + x] +=
					target_kernal[k]*target_conv[k];
		}
		  }
	}
	}

	/* third convolution */
	target_kernal=&target_kernal[kernal_size[filter_scale]];
	for (x = 0; x < width; x++) {
	for (y = 0; y < height; y++) {
		target_image=&image[(width*height)-(y*width+x+kernal_size[filter_scale]/2)];
		if ((x>=kernal_size[filter_scale]/2) && ((x+kernal_size[filter_scale]/2)<width))
		  {
		    for (k = 0; k < kernal_size[filter_scale]; k++)
		      temparray[k]= target_kernal[k]*target_image[k];
		    for (k = 0; k < kernal_size[filter_scale]; k++)
		      conv2[((width*height)-1)-(x*height+y)] += temparray[k];
		  }
		else
		  {
		for (k=0; k < kernal_size[filter_scale]; k++) {
			if ((x+kernal_size[filter_scale]/2 >= k) && (x+kernal_size[filter_scale]/2 < width+k)) {
				conv2[((width*height)-1)-(x*height+y)] +=
					target_kernal[k]*target_image[k];
			}
		}
		  }
	}
	}

	/* fourth convolution */
	target_kernal=&target_kernal[kernal_size[filter_scale]];
	for (x = 0; x < width; x++) {
	for (y = 0; y < height; y++) {
		target_conv=&conv2[((width*height)-1)-(x*height+y+(kernal_size[filter_scale]/2))];
		if (((y>=kernal_size[filter_scale]/2)) && ((y+kernal_size[filter_scale]/2)<height))
		  {
		    /* first do the matrix multiply, then do the summation, so the matrix multiply can be vectored. */
		    for (k = 0; k < kernal_size[filter_scale]; k++)
		      temparray[k] = target_kernal[k]*target_conv[k];
		    for (k = 0; k < kernal_size[filter_scale]; k++)
		      output[y*width+x] -= temparray[k];
		  }
		else
		  {
		for (k = 0; k < kernal_size[filter_scale]; k++) {
			if (((y+(kernal_size[filter_scale]/2))>=k) && (y+(kernal_size[filter_scale]/2)<height+k))
				output[y*width + x] -=
					target_kernal[k]*target_conv[k];
		}
		}
	}
	}

}
