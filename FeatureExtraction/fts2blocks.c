#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <ppm.h>

#include "extract_features.proto"

#define numH 18
#define numS 3
#define numV 3
#define numGrey 4

int main(int argc, char *argv[]) {

	char *in_fname, *out_fname;
	char *point_pos;
	enum file_types ppm_type;
	enum ppm_error the_error;
 
	switch(argc) {
	case 2:
		in_fname = argv[1];
		break;
	default:
		fprintf(stderr, "Usage: %s fts_file \n\n", argv[0]);
		exit(1);
		break;
	}

	/* now get the filename prefix */
	out_fname = (char *)malloc((strlen(in_fname) + 10)*sizeof(char));
	if ((point_pos = strchr(in_fname, '.')) == NULL) {
		fprintf(stderr, "File %s has no ""."" - can't generate features filename\n\n", in_fname);
		exit(1);
	}
	else {
		strncpy(out_fname, in_fname, (int)(point_pos - in_fname));
		strcat(out_fname, ".fts");
	}
	

	/* initialise the variables required for the feature extraction */
	init_feature_variables(18*3*3+4);

	fts2blocks(in_fname);

	exit(0);
}
