#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <ppm.h>
#include "gift_features.h"
//#include "extract_features.proto"

#define numH 18
#define numS 3
#define numV 3
#define numGrey 4

int main(int argc, char *argv[]) {

	char *in_fname, *out_fname;
	char *point_pos;
	FILE *ppm_file;
	PPM *im_rgb, *im_hsv, *im_quant;
	int *colmap, colmap_size;
	enum file_types ppm_type;
	enum ppm_error the_error;
 
	switch(argc) {
	case 2:
		in_fname = argv[1];
		break;
	default:
		fprintf(stderr, "Usage: %s ppm_file \n\n", argv[0]);
		exit(1);
		break;
	}

	if ((ppm_file = fopen (in_fname, "r")) == NULL) {
		fprintf(stderr, "Can't open file: %s", in_fname);
		exit(1);
	}

	/* now get the filename prefix */
	out_fname = (char *)malloc((strlen(in_fname) + 10)*sizeof(char));
	if ((point_pos = strchr(in_fname, '.')) == NULL) {
		fprintf(stderr, "File %s has no ""."" - can't generate features filename\n\n", in_fname);
		exit(1);
	}
	else {
		/* must find the *last* "." in the filename */
		point_pos = &in_fname[strlen(in_fname)];
		while (*point_pos != '.') {
			point_pos--;
		}
		strncpy(out_fname, in_fname, (int)(point_pos - in_fname));
		strcat(out_fname, ".fts");
	}
#ifdef DEBUG
	fprintf(stderr, "Features will be written to file %s\n", out_fname);
#endif
	

	/* read the rgb image from we are going to extract features */
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

	/* quantize the image */
	colmap_size = numH*numS*numV + numGrey;
	if ((the_error = hsv_quantize_ppm(im_hsv, &im_quant, &colmap, numH, numS, numV, numGrey)) != PPM_OK) {
		ppm_handle_error(the_error);
		exit(1);
	}

	/* initialise the variables required for the feature extraction */
	init_feature_variables(colmap_size);

	/* extract the features */

	extract_mode_features(im_quant, colmap, colmap_size);

	extract_gabor_features(im_hsv);

	/* write them to the file */
	if ((the_error = write_mode_features(out_fname, colmap_size)) != PPM_OK) {
	  ppm_handle_error(the_error);
	  exit(1);
	}
	
	/* everything is OK */
	destroy_ppm(&im_hsv);
	destroy_ppm(&im_quant);
	free(colmap);
	exit(0);
}
