#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <ppm.h>

#include "hsv_quantize_median_filter.proto"

int main(int argc, char *argv[]) {
	
	FILE *ppm_file;
	PPM *im_hsv, *im_quant, *im_filtered, *im_output;
	int *colmap;
	enum file_types ppm_type;
	enum ppm_error the_error;
	int numH, numS, numV, numGrey;

	switch(argc) {
	case 5:
		numH = atoi(argv[1]);
		numS = atoi(argv[2]);
		numV = atoi(argv[3]);
		numGrey = atoi(argv[4]);
		break;
	default:
		fprintf(stderr, "Usage: %s numH, numS, numV, numGrey\n\n", argv[0]);
		exit(1);
		break;
	}

	ppm_file = stdin;

	/* read the image we are going to quantize */
	switch(ppm_type = read_magic_no(ppm_file)) {
	case PGM_ASC: case PPM_ASC: case PGM_RAW: case PPM_RAW:
		if ((the_error = read_ppm(ppm_file, &im_hsv, ppm_type)) != PPM_OK) 
		{
			ppm_handle_error(the_error);
			exit(1);
		}
		break;
	default:
		fprintf(stderr, "Unrecognized file type.\n");
		exit(1);
		break;
	}

	/* quantize it */
	if ((the_error = hsv_quantize_ppm(im_hsv, &im_quant, &colmap, numH, numS, numV, numGrey)) != PPM_OK) {
		ppm_handle_error(the_error);
		exit(1);
	}

	/* median filter the quantized image */
	if ((the_error = median_filter_pgm(im_quant, &im_filtered)) != PPM_OK) {
		ppm_handle_error(the_error);
		exit(1);
	}

	/* convert the PGM and colour map to an "RGB" image */
	if ((the_error = colmap2rgb_ppm(im_filtered, colmap, numH*numS*numV + numGrey, &im_output)) != PPM_OK) {
		ppm_handle_error(the_error);
		exit(1);
	}

	/* write this "RGB" image to stdout */
	if ((the_error = write_ppm(stdout, im_output, PPM_RAW)) != PPM_OK) {
		ppm_handle_error(the_error);
		exit(1);
	}

	destroy_ppm(&im_output);
	destroy_ppm(&im_filtered);
	destroy_ppm(&im_quant);
	destroy_ppm(&im_hsv);
	exit(0);
}
