#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <ppm.h>

#include "rgb2hsv.proto"

void main(int argc, char *argv[]) {
	
	FILE *ppm_file;
	PPM *im1, *im_hsv;
	enum file_types ppm_type;
	enum ppm_error the_error;

/* Remove this and take the file from stdin */
/*
	switch(argc) {
	case 1:
		ppm_file = stdin;
		break;
	case 2:	
		if ((ppm_file = fopen (argv[1], "r")) == NULL) {
				fprintf(stderr, "Can't open file: %s", argv[1]);
				exit(1);
		}
		break;
	default:
		if (argc > 2) {
			fprintf(stderr, "Usage: ppmraw2asc file1\n       ppmraw2asc < file1\n       cat file1 | ppmraw2asc\n\n");
			exit(1);
		}
		break;
	}
*/
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

	if ((the_error = rgb2hsv_ppm(im1, &im_hsv)) != PPM_OK) {
		ppm_handle_error(the_error);
		exit(1);
	}

	if ((the_error = write_ppm(stdout, im_hsv, PPM_RAW)) != PPM_OK) {
		ppm_handle_error(the_error);
		exit(1);
	}

	exit(0);
}
