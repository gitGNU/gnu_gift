#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <ppm.h>

//#include "extract_features.proto"

/* for c99 uint32_t */

#include <stdint.h>

/* this is common between extract_features and this file. -ENEEDHEADER? */

/* the number of scales to recognise features at */
#define num_gabor_scales 3
/* the number of color blocks we're going to break the image into (overlapping) */
#define num_total_colour_blocks (256+64+16+4)

int main(int argc, char *argv[]) {

	FILE *ppm_file;
	PPM *im_hsv, *im_quant;
	int *colmap, colmap_size;
	enum file_types ppm_type;
	enum ppm_error the_error;
	int numH, numS, numV, numGrey;
	/* the following variables are here just to be initialized. */
	/* the colour count of each block. for quantizing the image */
	uint32_t * col_counts[num_total_colour_blocks];
	uint32_t ** gabor_histogram[num_gabor_scales];
	uint32_t ** block_gabor_class[num_gabor_scales];

 
	switch(argc) {
	case 5:
		numH = atoi(argv[1]);
		numS = atoi(argv[2]);
		numV = atoi(argv[3]);
		numGrey = atoi(argv[4]);
		break;
	case 1:
		numH = 18;
		numS = 3;
		numV = 3;
		numGrey = 4;
		break;
	default:
		fprintf(stderr, "Usage: %s [numH, numS, numV, numGrey] < ppm_file \n\n", argv[0]);
		exit(1);
		break;
	}

	ppm_file = stdin;

	/* read the hsv image from we are going to extract features */
	switch(ppm_type = read_magic_no(ppm_file)) {
	case PGM_ASC: case PPM_ASC: case PGM_RAW: case PPM_RAW:
		if ((the_error = read_ppm(ppm_file, &im_hsv, ppm_type)) != PPM_OK) {
			ppm_handle_error(the_error);
			exit(1);
		}
		break;
	default:
		fprintf(stderr, "Unrecognized file type.\n");
		exit(1);
		break;
	}

	/* quantize the image */
	colmap_size = numH*numS*numV + numGrey;
	if ((the_error = hsv_quantize_ppm(im_hsv, &im_quant, &colmap, numH, numS, numV, numGrey)) != PPM_OK) {
		ppm_handle_error(the_error);
		exit(1);
	}

	/* initialise the variables required for the feature extraction */
        init_feature_variables(colmap_size, col_counts, block_gabor_class, gabor_histogram);

	/* write them to the file */
	if ((the_error = write_feature_descriptions(stdout, colmap, colmap_size)) != PPM_OK) {
		ppm_handle_error(the_error);
		exit(1);
	}
	
	/* everything is OK */
	destroy_ppm(&im_hsv);
	destroy_ppm(&im_quant);
	free(colmap);
	exit(0);
}
