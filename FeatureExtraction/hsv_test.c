#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <ppm.h>

#include "hsv_test.proto"

int main(int argc, char *argv[]) {
	
	PPM *im_hsv;
	enum file_types ppm_type;
	enum ppm_error the_error;
	int dim;
	float f;
	double V;

	switch(argc) {
	case 3:
		dim = atoi(argv[1]);
		sscanf(argv[2], "%f", &f);
		V = (double)f;
		break;
	default:
		fprintf(stderr, "Usage: %s dimension value\n\n", argv[0]);
		exit(1);
		break;
	}

	im_hsv = hsv_test_pattern(dim, V);
	if ((the_error = write_ppm(stdout, im_hsv, PPM_RAW)) != PPM_OK) {
		ppm_handle_error(the_error);
		exit(1);
	}

	exit(0);
}
