#include <stdio.h>
#include <math.h>
#include "ppm.h"

double max3(double v1, double v2, double v3) {

	double max;

	max = (v1 > v2) ? v1 : v2;
	max = (max > v3) ? max : v3;
	return(max);
}

double min3(double v1, double v2, double v3) {

	double min;

	min = (v1 < v2) ? v1 : v2;
	min = (min < v3) ? min : v3;
	return(min);
}

void count_to_coords(int *coords, int c, int *r, int d) {
 
    int i;
 
    for (i = 0; i < d; i++) {
        coords[i] = c % r[i];
        c = (c - coords[i])/r[i];
    }
}
 
int coords_to_count(int *coords, int *r, int d) {
 
    int i, count;
 
    count = 0;
    for (i = d - 1; i >= 0; i--)
        count = r[i]*count + coords[i];
    return(count);
}


/* David Squire 980922 */

void rgb2hsv(double R, double G, double B, double *H, double *S, double *V) {
	
	double H1, S1, V1, R1, G1, B1;
	double max, min, diff;

	max = max3(R, G, B);
	min = min3(R, G, B);
	diff = max - min;

	if (max == 0)
		H1 = S1 = V1 = 0;
	else {
		V1 = max;
		S1 = diff/max;
		if (S1 == 0)
			/* H1 is undefined, but give it a value anyway */
			H1 = 0;
		else {
#ifdef HSV_TRAVIS
			R1 = (max - R)/diff;
			G1 = (max - G)/diff;
			B1 = (max - B)/diff;

			if ((R == max) && (G == min))
				H1 = 5 + B1;
			else {
				if ((R == max) && (G != min))
					H1 = 1 - G1;
				else {
					if ((G == max) && (B == min))
						H1 = 1 + R1;
					else {
						if ((G == max) && (B != min))
							H1 = 3 - B1;
						else {
							if (R == max)
								H1 = 3 + G1;
							else
								H1 = 5 - R1;
						}
					}
				}
			}

			H1 *= 60; /* convert to range [0, 360] degrees */
#else
	/* assume Foley & VanDam HSV */
			if (R == max)
				H1 = (G - B)/diff;
			if (G == max)
				H1 = 2 + (B - R)/diff;
			if (B == max)
				H1 = 4 + (R - G)/diff;

			H1 *= 60; /* convert to range [0, 360] degrees */
			if (H1 < 0)
				H1 += 360;
#endif
		}
	}
	*H = H1;
	*S = S1;
	*V = V1;
}

void hsv2rgb(double H, double S, double V, double *R, double *G, double *B) {

	int i;
	double f, p, q, t;

	/* assume Foley & VanDam HSV */

	if ((S == 0) && (H == 0))
		*R = *G = *B = V;

	if (H == 360)
		H = 0;

	H /= 60;
	i = (int)floor(H);
	f = H - i;
	p = V*(1 - S);
	q = V*(1 - S*f);
	t = V*(1 - S*(1 - f));

	switch(i) {
	case 0:
		*R = V, *G = t, *B = p;
		break;
	case 1:
		*R = q, *G = V, *B = p;
		break;
	case 2:
		*R = p, *G = V, *B = t;
		break;
	case 3:
		*R = p, *G = q, *B = V;
		break;
	case 4:
		*R = t, *G = p, *B = V;
		break;
	case 5:
		*R = V, *G = p, *B = q;
		break;
	default:
		break;
	}


	

}

/* Converts a colour PPM image im_rgb, assumed to be in RGB format to
im_hsv in HSV format. It is assumed that space has already been allocated
for im_hsv. */

/* David Squire 980922 */

enum ppm_error rgb2hsv_ppm(PPM *im_rgb, PPM **im_hsv) {
	
	double R, G, B;
	double H, S, V;
	double max, min, diff;
	long i, num_pixels;
	double max_col;

	if (im_rgb->bytes_per_pixel != 3) {
		fprintf(stderr, "rgb2hsv_ppm: ");
		return(THREE_PLANES_NEEDED);
	}

	/* create the hsv image */
    /* make space for an HSV image of the same size as that read */
    (*im_hsv) = new_ppm();
    (*im_hsv)->type = PPM_RAW;
    (*im_hsv)->width = im_rgb->width;
    (*im_hsv)->height = im_rgb->height;
    (*im_hsv)->max_col_comp = im_rgb->max_col_comp;
    (*im_hsv)->bytes_per_pixel = 3;
    (*im_hsv)->pixel = (byte *)malloc(3*(*im_hsv)->width*(*im_hsv)->height*sizeof(byte));

	max_col = (double)im_rgb->max_col_comp;
	num_pixels = im_rgb->width*im_rgb->height;

	for (i = 0; i < num_pixels; i++) {

		R = (double)im_rgb->pixel[3*i + RED]/max_col;
		G = (double)im_rgb->pixel[3*i + GREEN]/max_col;
		B = (double)im_rgb->pixel[3*i + BLUE]/max_col;

		rgb2hsv(R, G, B, &H, &S, &V);

		/* now convert back to bytes and store in output image */
		(*im_hsv)->pixel[3*i + HUE] = (byte)rint((H/360.0)*max_col);
		(*im_hsv)->pixel[3*i + SATURATION] = (byte)rint(S*max_col);
		(*im_hsv)->pixel[3*i + VALUE] = (byte)rint(V*max_col);
	}

	add_comment((*im_hsv), "# Image converted from RGB to HSV format.\n");

	return(PPM_OK);
}

enum ppm_error hsv2rgb_ppm(PPM *im_hsv, PPM **im_rgb) {
	
	double R, G, B;
	double H, S, V;
	double max, min, diff;
	long i, num_pixels;
	double max_col;

	if (im_hsv->bytes_per_pixel != 3) {
		fprintf(stderr, "hsv2rgb_ppm: ");
		return(THREE_PLANES_NEEDED);
	}

	/* create the rgb image */
   /* make space for an HSV image of the same size as that read */
    (*im_rgb) = new_ppm();
    (*im_rgb)->type = PPM_RAW;
    (*im_rgb)->width = im_hsv->width;
    (*im_rgb)->height = im_hsv->height;
    (*im_rgb)->max_col_comp = im_hsv->max_col_comp;
    (*im_rgb)->bytes_per_pixel = 3;
    (*im_rgb)->pixel = (byte *)malloc(3*(*im_rgb)->width*(*im_rgb)->height*sizeof(byte));

	max_col = (double)(*im_rgb)->max_col_comp;
	num_pixels = (*im_rgb)->width*(*im_rgb)->height;

	for (i = 0; i < num_pixels; i++) {

		/* convert from bytes to doubles in [0, 360], [0, 1], [0, 1] */
		H = 360*(double)im_hsv->pixel[3*i + HUE]/max_col;
		S = (double)im_hsv->pixel[3*i + SATURATION]/max_col;
		V = (double)im_hsv->pixel[3*i + VALUE]/max_col;

		hsv2rgb(H, S, V, &R, &G, &B);

		/* now convert back to bytes and store in output image */
		(*im_rgb)->pixel[3*i + RED] = (byte)rint(R*max_col);
		(*im_rgb)->pixel[3*i + GREEN] = (byte)rint(G*max_col);
		(*im_rgb)->pixel[3*i + BLUE] = (byte)rint(B*max_col);
	}

	add_comment((*im_rgb), "# Image converted from HSV to RGB format.\n");

	return(PPM_OK);
}

void polar_to_hsv(double x, double y, int dim, double *H, double *S) {

	double theta, radius;

	theta = atan2(y, x);
	if (y < 0)
		theta += 2*M_PI;
	*H = 180*theta/M_PI;

	radius = hypot(x, y)/((double)dim/2);
	if (radius > 1)
		*H = *S = 0;
	else
		*S = radius;
}

PPM *hsv_test_pattern(int dim, double V) {
	
	PPM *im_hsv;
	int i, j;
	double x, y, H, S;

	/* create space for the test pattern */
	im_hsv = new_ppm();
	add_comment(im_hsv, "# HSV test pattern. In HSV format.\n");
	im_hsv->type = PPM_RAW;
	im_hsv->width = im_hsv->height = dim;
	im_hsv->max_col_comp = 255;
	im_hsv->bytes_per_pixel = 3;
	im_hsv->pixel = (byte *)malloc(3*dim*dim*sizeof(byte));

	/* now set the HSV values for each pixel */
	for (i = 0; i < dim; i++) {		/* rows */
		y = (dim - i) - dim/2;
		for (j = 0; j < dim; j++) {	/* columns */
			x = j - dim/2;
			polar_to_hsv(x, y, dim, &H, &S);
			im_hsv->pixel[3*i*dim + 3*j + HUE] = (byte)rint(((double)H/360.0)*255.0);
			im_hsv->pixel[3*i*dim + 3*j + SATURATION] = (byte)rint((double)S*255.0);
			im_hsv->pixel[3*i*dim + 3*j + VALUE] = (byte)rint((double)V*255.0);

		}
	}
	return(im_hsv);
}
