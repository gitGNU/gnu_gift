#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <ppm.h>

#include "hsv2rgb.proto"

int main(int argc, char *argv[]) {
	
	FILE *ppm_file;
	PPM *im1, *im_rgb;
	enum file_types ppm_type;
	enum ppm_error the_error;

	ppm_file = stdin;

	switch(ppm_type = read_magic_no(ppm_file)) {
	case PGM_ASC: case PPM_ASC: case PGM_RAW: case PPM_RAW:
		if ((the_error = read_ppm(ppm_file, &im1, ppm_type)) != PPM_OK) {
			ppm_handle_error(the_error);
			exit(1);
		}
		break;
	default:
		fprintf(stderr, "Unrecognized file type.\n");
		break;	
	}

	if ((the_error = hsv2rgb_ppm(im1, &im_rgb)) != PPM_OK) {
		ppm_handle_error(the_error);
		exit(1);
	}

	if ((the_error = write_ppm(stdout, im_rgb, PPM_RAW)) != PPM_OK) {
		ppm_handle_error(the_error);
		exit(1);
	}

	exit(0);
}
