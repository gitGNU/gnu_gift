#include <stdio.h>
#include <math.h>

#define MAX_VALUE 2000.0
#define STEP 0.1

void main(int argc, char *argv[]) {

	char *histogram_filename = argv[1];
	FILE *histogram_file, *infile;
	int num_entries = MAX_VALUE/STEP;
	double *hist;
	float f;
	double data;
	int i;


	hist = (double *)calloc(num_entries, sizeof(double));

	if ((histogram_file = fopen(histogram_filename, "rb")) != NULL) {
		fread(hist, sizeof(double), num_entries, histogram_file);
		fclose(histogram_file);
	}
	infile = stdin;

	while(fscanf(infile, "%f", &f) != EOF) {
		data = (double)f;
		if (data >= MAX_VALUE) {
			fprintf(stderr, "Value %f greater than max. allowed in histogram (%f)\n", data, MAX_VALUE);
			data = 0.9999*MAX_VALUE;
		}
		i = (int)floor(data/STEP);
		hist[i]++;
	}

	histogram_file = fopen(histogram_filename, "wb");
	fwrite(hist, sizeof(double), num_entries, histogram_file);
	fclose(histogram_file);
}
