/* -*- mode: c -*- 
*/
/* 

    GIFT, a flexible content based image retrieval system.
    Copyright (C) 1998, 1999, 2000 CUI, University of Geneva

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <ppm.h>
#include <gift_features.h>

#include "gabor.h"
//#include "extract_features.proto"

#define square(x) ((x)*(x))

/* This determines the number of bands into which the gabor filter energies
 * are quantized.
/* 
#define num_gabor_ranges 16
*/
#define num_gabor_ranges 10

const int image_size = 256;
const int smallest_colour_block = 16;
const int gabor_block_size = 16;
int num_colour_scales;
int num_total_colour_blocks;
int *num_blocks_at_scale;
int **col_counts;
int *col_histogram;
byte *block_mode;
int num_colours_seen = 0;

/* This specifies the bands into which the gabor filter energies are
 * quantized.
/*
double gabor_ranges[num_gabor_ranges] = {0.0625, 0.125, 0.25, 0.5, 1, 1.5, 2, 3, 4, 5, 6, 7, 8, 9, 10, 100000};
*/
double gabor_ranges[num_gabor_ranges] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 100000};

int ***block_gabor_class;
int ***gabor_histogram;

void init_feature_variables(int colmap_size) {

	int i, j, k;
	int r, r1;
	int sqrt_num_blocks_at_this_scale;

	/* colour features */
	r = image_size/smallest_colour_block;
	num_colour_scales = (int)(log((double)r)/log(2.0));
	num_total_colour_blocks = (int)((double)square(r)*4.0*(1 - 1/pow(4.0, (double)num_colour_scales))/3.0);
	block_mode = (byte *)malloc(num_total_colour_blocks*sizeof(byte));
	col_counts = (int **)malloc(num_total_colour_blocks*sizeof(int *));
	col_histogram = (int *)malloc(colmap_size*sizeof(int));
	num_blocks_at_scale = (int *)malloc(num_colour_scales*sizeof(int));
	k = 0;
	num_blocks_at_scale[0] = square(r);
	for (i = 0; i < num_colour_scales; i++) {
		for (j = 0; j < num_blocks_at_scale[i]; j++) {
			col_counts[k++] = (int *)malloc(colmap_size*sizeof(int));
		}
		if (i != num_colour_scales - 1)
			num_blocks_at_scale[i+1] = num_blocks_at_scale[i]/4;
	}
#ifdef DEBUG
	fprintf(stderr, "num_total_colour_blocks: %d\n", num_total_colour_blocks);
#endif

	/* Gabor features */
	block_gabor_class = (int ***)malloc(num_gabor_scales*sizeof(int **));
	gabor_histogram = (int ***)malloc(num_gabor_scales*sizeof(int **));
	for (i = 0; i < num_gabor_scales; i++) {
		block_gabor_class[i] = (int **)malloc(num_gabors_per_scale*sizeof(int *));
		gabor_histogram[i] = (int **)malloc(num_gabors_per_scale*sizeof(int *));
		for (j = 0; j < num_gabors_per_scale; j++) {
			block_gabor_class[i][j] = (int *)malloc(square(image_size/gabor_block_size)*sizeof(int));
			gabor_histogram[i][j] = (int *)calloc(num_gabor_ranges, sizeof(int));
		}
	}
}

void extract_gabor_features(PPM *im_hsv) {

	int i, j, x, y, k;
	int scale, orientation;
	int energy_class;
	PPM *value_plane;
	double *value_image_dbl, *filtered_image;
	double gabor_mean;

	/* for Gabor features, this is what I'm going to do:
		- Apply each of the 12 filters (3 scales, 4 orientations) to each
		  pixel in the image.
		- Use 16x16 blocks only.
		- In each block find the rms energy of each filter.
		- quantize these into num_gabor_ranges levels, as specified in the
		  array gabor_ranges[]
	*/

	/* make a float version of the value plane of the image */
	value_image_dbl = (double *)malloc(im_hsv->width*im_hsv->height*sizeof(double));
	filtered_image = (double *)malloc(im_hsv->width*im_hsv->height*sizeof(double));
	value_plane = ppm_plane(im_hsv, VALUE);
	for (i = 0; i < im_hsv->width*im_hsv->height; i++) {
		value_image_dbl[i] = (double)(value_plane->pixel[i]);
	}
	destroy_ppm(&value_plane);

	/* apply each filter to the image */
	for (scale = 0; scale < num_gabor_scales; scale++) {
		for (orientation = 0; orientation < num_gabors_per_scale; orientation++) {

			/* filter the image */
			gabor_filter(value_image_dbl, im_hsv->width, im_hsv->height, scale, orientation, filtered_image);

			/* extract the rms energy for each block */
			k = 0; /* block counter */
			for (y = 0; y < im_hsv->height; y += gabor_block_size) {
			for (x = 0; x < im_hsv->width; x += gabor_block_size) {
				gabor_mean = 0;
				for (i = 0; i < gabor_block_size; i++) {
				for (j = 0; j < gabor_block_size; j++) {
					gabor_mean += square((filtered_image[(y + i)*im_hsv->width + (x + j)]));
				}
				}
				gabor_mean /= square(gabor_block_size);
				gabor_mean = sqrt(gabor_mean);

				/* find the energy class for this block */
				for (energy_class = 0; energy_class < num_gabor_ranges; energy_class++) {
					if (gabor_mean < gabor_ranges[energy_class])
						break;
				}

				/* store this class in the appropriate place */
				block_gabor_class[scale][orientation][k] = energy_class;

				/* increment the histogram entry for this combination */
				gabor_histogram[scale][orientation][energy_class]++;
				
				k++; /* increment block counter */
			}
			}
		}
	}
}

void extract_mode_features(PPM *im, int *colmap, int colmap_size) {

	int i, j, k, last_k, k1, m, n, r, x, y;
	byte colour;
	int scale, block_size, num_blocks, old_num_blocks;
	int max_count, mode_index;
	int b1, b2, b3, b4;
#ifdef GENERATE_BLOCK_IMAGES
	PPM *out_image, *out_image_hsv, *out_image_rgb;
	enum ppm_error the_error;
	FILE *outfile;
	char out_fname[256];
#endif

#ifdef GENERATE_BLOCK_IMAGES
    /* make space for an HSV image of the same size as that read */
    out_image = new_ppm();
    out_image->type = PGM_RAW;
    out_image->width = im->width;
    out_image->height = im->height;
    out_image->max_col_comp = im->max_col_comp;
    out_image->bytes_per_pixel = 1;
    out_image->pixel = (byte *)malloc(out_image->width*out_image->height*sizeof(byte));
#endif

	/* first extract the lowest level blocks, directly from the pixels of
	the PGM image */
	scale = 0;
	block_size = smallest_colour_block;
	num_blocks = square(image_size/block_size);
	k = last_k = 0;
	for (y = 0; y < im->height; y += block_size) {
	for (x = 0; x < im->width; x += block_size) {
		for (n = 0; n < colmap_size; n++) {
			col_counts[k][n] = 0;	
		}
		for (i = 0; i < block_size; i++) {
		for (j = 0; j < block_size; j++) {
			colour = im->pixel[(y + j)*im->width + (x + i)];
			col_counts[k][colour]++;
		}
		}

		/* find the mode */
		max_count = mode_index = 0;
		for (n = 0; n < colmap_size; n++) {
			if (col_counts[k][n] > max_count) {
				max_count = col_counts[k][n];
				mode_index = n;
			}
		}
		block_mode[k] = mode_index;
		k++;
	}
	}

#ifdef GENERATE_BLOCK_IMAGES
	/* generate an image with this */ 
	k1 = 0;
	for (y = 0; y < im->height; y += block_size) {
	for (x = 0; x < im->width; x += block_size) {
		for (i = 0; i < block_size; i++) {
		for (j = 0; j < block_size; j++) {
			out_image->pixel[(y + j)*im->width + (x + i)] = block_mode[k1];
		}
		}
		k1++;
	}
	}
	/* convert from the colour map to an HSV image */
	if ((the_error = colmap2rgb_ppm(out_image, colmap, colmap_size, &out_image_hsv)) != PPM_OK) {
		ppm_handle_error(the_error);
		exit(1);
	}
	/* convert to RGB */
	if ((the_error = hsv2rgb_ppm(out_image_hsv, &out_image_rgb)) != PPM_OK) {
		ppm_handle_error(the_error);
		exit(1);
	}
	/* write it */
	sprintf(out_fname, "blocks_%dx%d.ppm", block_size, block_size);
	outfile = fopen(out_fname, "wb");
	if ((the_error = write_ppm(outfile, out_image_rgb, PPM_RAW)) != PPM_OK) {
		ppm_handle_error(the_error);
		exit(1);
	}
	fclose(outfile);
	destroy_ppm(&out_image_rgb);
	destroy_ppm(&out_image_hsv);
#endif


	/* now do the other scales */
	for (scale = 1; scale < num_colour_scales; scale++) {
		block_size *= 2;
		num_blocks /= 4;
		r = (int)sqrt((double)num_blocks);
		for (i = 0; i < num_blocks; i++) {
			m = (i/r)*2*r; /* note: this first is *integer* division */
			b1 = last_k + 2*i + m;
			b2 = last_k + 2*i + m + 1;
			b3 = last_k + 2*i + m + 2*r;
			b4 = last_k + 2*i + m + 2*r + 1;
			max_count = mode_index = 0;
			for (n = 0; n < colmap_size; n++) {
				col_counts[k + i][n] = 
					col_counts[b1][n] + col_counts[b2][n] + 
					col_counts[b3][n] + col_counts[b4][n];
				if (col_counts[k + i][n] > max_count) {
					max_count = col_counts[k + i][n];
					mode_index = n;
				}
			}
			block_mode[k + i] = mode_index;
		}
#ifdef GENERATE_BLOCK_IMAGES
		/* generate an image with this */ 
		for (y = 0; y < im->height; y += block_size) {
		for (x = 0; x < im->width; x += block_size) {
			for (i = 0; i < block_size; i++) {
			for (j = 0; j < block_size; j++) {
				out_image->pixel[(y + j)*im->width + (x + i)] = block_mode[k1];
			}
			}
			k1++;
		}
		}
		/* convert from the colour map to an HSV image */
		if ((the_error = colmap2rgb_ppm(out_image, colmap, colmap_size, &out_image_hsv)) != PPM_OK) {
			ppm_handle_error(the_error);
			exit(1);
		}
		/* convert to RGB */
		if ((the_error = hsv2rgb_ppm(out_image_hsv, &out_image_rgb)) != PPM_OK) {
			ppm_handle_error(the_error);
			exit(1);
		}
		/* write it */
		sprintf(out_fname, "blocks_%dx%d.ppm", block_size, block_size);
		outfile = fopen(out_fname, "wb");
		if ((the_error = write_ppm(outfile, out_image_rgb, PPM_RAW)) != PPM_OK) {
			ppm_handle_error(the_error);
			exit(1);
		}
		fclose(outfile);
		destroy_ppm(&out_image_rgb);
		destroy_ppm(&out_image_hsv);
#endif
		last_k = k;
		k += num_blocks;
	}

	/* now finally consolidate all the colour counts to produce the
	histogram for the whole image */
	k -= num_blocks;
	for (n = 0; n < colmap_size; n++)
		col_histogram[n] = 
			col_counts[k][n] + col_counts[k + 1][n] + 
			col_counts[k + 2][n] + col_counts[k + 3][n];
}

enum ppm_error write_mode_features(char *out_fname, int colmap_size) {

	FILE *out_file = NULL;
	int feature_index;
	int block_features_offset;
	int scale, orientation, energy_class;
	int num_features;
	FEATURE_DATA *feature;
	int i;

	/***** count the features *****/

	/* we know that there will be a mode color for each block */
	num_features = num_total_colour_blocks;

	/* and there will be one for each non-zero histogram entry. Convert
	histogram entries to range [0, FREQ_MAX] while we're at it */
	for (i = 0; i < colmap_size; i++) {
		col_histogram[i] = (int)(rint(FREQ_MAX*(double)col_histogram[i]/(double)(square(image_size))));
		if (col_histogram[i] != 0)
			num_features++;
	}

#ifdef NO_FLAT_FEATURES
	/* count the block entries which are not of class zero (which indicates
	a filter response too low to write), and the non-zero gabor histogram
	entries */
	for (scale = 0; scale < num_gabor_scales; scale++) {
		for (orientation = 0; orientation < num_gabors_per_scale; orientation++) {
			/* blocks */
			for (i = 0; i < square((image_size/gabor_block_size)); i++) {
				if (block_gabor_class[scale][orientation][i] != 0)
					num_features++;
			}

			/* histogram */
			for (energy_class = 0; energy_class < num_gabor_ranges; energy_class++) {
				if (gabor_histogram[scale][orientation][energy_class] != 0)
					num_features++;
			}
		}
	}
#else
	/* here we ARE storing features for filter responses in the lowest
	 * energy band, so the number of block features is fixed (and v.
	 * large!). We also count the non-zero gabor histogram entries */

	/* blocks */
	num_features += num_gabor_scales*num_gabors_per_scale*square((image_size/gabor_block_size));

	/* histogram */
	for (scale = 0; scale < num_gabor_scales; scale++) {
		for (orientation = 0; orientation < num_gabors_per_scale; orientation++) {
			for (energy_class = 0; energy_class < num_gabor_ranges; energy_class++) {
				if (gabor_histogram[scale][orientation][energy_class] != 0)
					num_features++;
			}
		}
	}
#endif

#ifdef DEBUG
	fprintf(stderr, "Image contains %d features.\n", num_features);
#endif

	/* allocate space to store the features */
	feature = (FEATURE_DATA *)malloc(num_features*sizeof(FEATURE_DATA));

	/***** store the features in the array *****/
	feature_index = 0;

	/* colour block features */
	for (i = 0; i < num_total_colour_blocks; i++) {
		/* note that each block can have colmap_size values */
		feature[feature_index].id = i*colmap_size + (int)block_mode[i];
		feature[feature_index].frequency = FREQ_MAX; /* binary features */
		feature_index++;
	}
	block_features_offset = num_total_colour_blocks*colmap_size;

	/* colour histogram features */
	for (i = 0; i < colmap_size; i++) {
		if (col_histogram[i] != 0) {
			feature[feature_index].id = block_features_offset + i;
			feature[feature_index].frequency = (freq_type)col_histogram[i];
			feature_index++;
		}
	}
	block_features_offset += colmap_size;

	/* gabor block features */
	for (scale = 0; scale < num_gabor_scales; scale++) {
		for (orientation = 0; orientation < num_gabors_per_scale; orientation++) {
			/* blocks */
			for (i = 0; i < square((image_size/gabor_block_size)); i++) {
#ifdef NO_FLAT_FEATURES
				if (block_gabor_class[scale][orientation][i] != 0) {
					feature[feature_index].id = block_features_offset + block_gabor_class[scale][orientation][i];
					feature[feature_index].frequency = FREQ_MAX;
					feature_index++;
				}
#else
				feature[feature_index].id = block_features_offset + block_gabor_class[scale][orientation][i];
				feature[feature_index].frequency = FREQ_MAX;
				feature_index++;
#endif
				block_features_offset += num_gabor_ranges;
			}
		}
	}

	/* gabor histogram features */
	for (scale = 0; scale < num_gabor_scales; scale++) {
		for (orientation = 0; orientation < num_gabors_per_scale; orientation++) {
			for (energy_class = 0; energy_class < num_gabor_ranges; energy_class++) {
				if (gabor_histogram[scale][orientation][energy_class] != 0) {
					feature[feature_index].id = block_features_offset;
					feature[feature_index].frequency = (freq_type)rint(FREQ_MAX*(double)gabor_histogram[scale][orientation][energy_class]/(double)square((image_size/gabor_block_size)));
					feature_index++;
				}
				block_features_offset++;
			}
		}
	}

#ifdef DEBUG
	fprintf(stderr, "%d features found.\n", feature_index);
	fprintf(stderr, "block_features_offset = %d.\n", block_features_offset);
#endif	

		
	/***** now write the file *****/

	/* open it */
	if ((out_file = fopen(out_fname, "wb")) == NULL) {
		fprintf(stderr, "Error opening file %s for writing.\n\n", out_fname);
		return(FILE_OPEN_ERROR);
	}

	/* write the number of features */
	if (fwrite(&num_features, sizeof(int), 1, out_file) != 1) {
		fprintf(stderr, "Error writing file %s.\n\n", out_fname);
		return(FILE_WRITE_ERROR);
	}

	/** HACK: convert all this features to floats.

	    WARNING: we write here floats into an
	    integer field of a structure

	    Why is this necessary?
	    The original version rounds at many places
	    So we cannot just set FREQ_MAX=1.
	    (WM)*/
	for(i=0;
	    i<num_features;
	    i++){
	  *((float*)&(feature[i].frequency))=((float)feature[i].frequency/(float)FREQ_MAX);//(WM HACK)
	};

	/* write the features, as a block */
	if (fwrite(feature, sizeof(FEATURE_DATA), num_features, out_file) != num_features) {
		fprintf(stderr, "Error writing file %s.\n\n", out_fname);
		return(FILE_WRITE_ERROR);
	}

	if (fclose(out_file) == EOF) {
		fprintf(stderr, "Error closing file %s.\n\n", out_fname);
		return(FILE_CLOSE_ERROR);
	}

	/* everything is OK if we got this far */
	return(PPM_OK);
}

void fts2blocks(char *fts_fname) {
	
	FILE *fts_file, *outfile;
	int num_features, feature_index;
	FEATURE_DATA *feature;
	int block_features_offset;
	int block_size, num_blocks, scale;
	int *colmap;
	int colmap_size = 18*3*3 + 4;
	int x, y, i, j, k;
	PPM *hsv_image, *qimage, *rgb_image;
	enum ppm_error the_error;
	char out_fname[256];
 
	/* read the number of features */
	fts_file = fopen(fts_fname, "rb");
	fread(&num_features, sizeof(int), 1, fts_file);
	
	/* allocate space for the features */
	feature = (FEATURE_DATA *)malloc(num_features*sizeof(FEATURE_DATA));
 
	/* read the features */
	fread(feature, sizeof(FEATURE_DATA), num_features, fts_file);

	fclose(fts_file);

	/* "unpack" the features in the array */
	feature_index = 0;

	/* block features */
	for (i = 0; i < num_total_colour_blocks; i++) {
		/* note that each block can have colmap_size values */
		block_mode[i] = feature[feature_index].id - i*colmap_size;
		feature_index++;
	}
	block_features_offset = num_total_colour_blocks*colmap_size;

	/* colour histogram features */
	for (i = 0; i < colmap_size; i++)
		col_histogram[i] = 0;

	for (; feature_index < num_features; feature_index++) {
		i = feature[feature_index].id - block_features_offset;
		col_histogram[i] = (byte)(feature[feature_index].frequency);
	}

	/* make a dummy image so that we can generate a colour map */
    hsv_image = new_ppm();
    hsv_image->type = PPM_RAW;
    hsv_image->width = 256;
    hsv_image->height = 256;
    hsv_image->max_col_comp = 255;
    hsv_image->bytes_per_pixel = 3;
    hsv_image->pixel = (byte *)malloc(3*256*256*sizeof(byte));
	hsv_quantize_ppm(hsv_image, &qimage, &colmap, 18, 3, 3, 4);
	destroy_ppm(&hsv_image);

	/* now write the block images */
	k = 0;
    block_size = smallest_colour_block;
    num_blocks = square(image_size/block_size);
	for (scale = 0; scale < num_colour_scales; scale++) {
		for (y = 0; y < 256; y += block_size) {
		for (x = 0; x < 256; x += block_size) {
			for (i = 0; i < block_size; i++) {
			for (j = 0; j < block_size; j++) {
				qimage->pixel[(y + j)*256 + (x + i)] = block_mode[k];
			}
			}
			k++;
		}
		}
		/* convert from the colour map to an HSV image */
		if ((the_error = colmap2rgb_ppm(qimage, colmap, colmap_size, &hsv_image)) != PPM_OK) {
			ppm_handle_error(the_error);
			exit(1);
		}
		/* convert to RGB */
		if ((the_error = hsv2rgb_ppm(hsv_image, &rgb_image)) != PPM_OK) {
			ppm_handle_error(the_error);
			exit(1);
		}
		/* write it */
		sprintf(out_fname, "fts2blocks_%dx%d.ppm", block_size, block_size);
		outfile = fopen(out_fname, "wb");
		if ((the_error = write_ppm(outfile, rgb_image, PPM_RAW)) != PPM_OK) {
			ppm_handle_error(the_error);
			exit(1);
		}
		fclose(outfile);
		destroy_ppm(&hsv_image);
		destroy_ppm(&rgb_image);
		block_size *= 2;
		num_blocks /= 4;
	}
}

enum ppm_error write_feature_descriptions(FILE *out_file, int *colmap, int colmap_size) {

	int feature_index = 0;
	int block_size;
	int i, j, k;
	int scale, orientation;

	/* block features */
	block_size = smallest_colour_block;
	for (i = 0; i < num_colour_scales; i++) {
		for (j = 0; j < num_blocks_at_scale[i]; j++) {
			for (k = 0; k < colmap_size; k++) {
				fprintf(out_file, "%d %d COL_POS block size = %dx%d position = %d H,S,V = %d, %d, %d\n", feature_index, COL_POS, block_size, block_size, j, colmap[3*k + HUE], colmap[3*k + SATURATION], colmap[3*k + VALUE]);
				feature_index++;
			}
		}
		block_size *= 2;
	}

	/* colour histogram features */
	for (i = 0; i < colmap_size; i++) {
		fprintf(out_file, "%d %d COL_HST H,S,V = %d, %d, %d\n", feature_index, COL_HST, colmap[3*i + HUE], colmap[3*i + SATURATION], colmap[3*i + VALUE]);
		feature_index++;
	}
		
	/* gabor block features */
	for (scale = 0; scale < num_gabor_scales; scale++) {
		for (orientation = 0; orientation < num_gabors_per_scale; orientation++) {
			/* blocks */
			for (i = 0; i < square((image_size/gabor_block_size)); i++) {
				for (j = 0; j < num_gabor_ranges; j++) {
					fprintf(out_file, "%d %d GABOR_POS block size = %dx%d position = %d SCALE, ORIENTATION, ENERGY = %d, %d, %f\n", feature_index, GABOR_POS, gabor_block_size, gabor_block_size, i, scale, orientation, gabor_ranges[j]);
				feature_index++;
				}
			}
		}
	}

	/* gabor histogram features */
	for (scale = 0; scale < num_gabor_scales; scale++) {
		for (orientation = 0; orientation < num_gabors_per_scale; orientation++) {
			for (j = 0; j < num_gabor_ranges; j++) {
				fprintf(out_file, "%d %d GABOR_HST SCALE, ORIENTATION, ENERGY UPPER BOUND = %d, %d, %f\n", feature_index, GABOR_HST, scale, orientation, j);
				feature_index++;
			}
		}
	}

	/* everything is OK if we got this far */
	return(PPM_OK);
}
