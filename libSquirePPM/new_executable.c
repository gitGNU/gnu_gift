#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#include <ppm.h>

double linear(double x) {
	return(x);
}

double sigmoid(double x) {

	double gain = 0.5e4;
	double threshold = 0.0005;
	double default_val = 0.05;
	double ret_val;

	ret_val = default_val + (x - default_val)/(1 + exp(-gain*(x - threshold)));
	return(ret_val);
	
}

double threshold(double x) {

	double t = 0.0005;
	double ret_val;

	if (x > t)
		ret_val = x;
	else
		ret_val = 1;
	return(ret_val);
}

int main(int argc, char *argv[]) {
	
	FILE *source_file, *normalized_image_file;
	PPM *source_image, *normalized_image;
	double *dbl_normalized_image;
	enum file_types ppm_type;
	enum ppm_error the_error;
	int window_size;

	switch(argc) {
		case 2:
			window_size = atoi(argv[1]);
		break;
	default:
		fprintf(stderr, "Usage: ppm_normalize window_size < file1.ppm > file2.ppm\n\n");
		exit(1);
		break;
	}

	/* act as a pipe */
	source_file = stdin;
	normalized_image_file = stdout;

	switch(ppm_type = read_magic_no(source_file)) {
	case PGM_ASC: case PPM_ASC: case PGM_RAW: case PPM_RAW:
		if ((the_error = read_ppm(source_file, &source_image, ppm_type)) != PPM_OK) {
			ppm_handle_error(the_error);
			exit(1);
		}
		break;
	default:
		fprintf(stderr, "Unrecognized file type.\n");
		exit(1);
		break;	
	}
	
	dbl_normalized_image = ppm_local_normalize(source_image, window_size, sigmoid);
	normalized_image = ppm_double_to_byte(dbl_normalized_image, source_image->width, source_image->height);

	if ((the_error = write_ppm(normalized_image_file, normalized_image, normalized_image->type)) != PPM_OK) {
		ppm_handle_error(the_error);
		exit(1);
	}
	exit(0);
}
