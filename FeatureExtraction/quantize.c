#include <math.h>
#include <ppm.h>

enum ppm_error hsv_quantize_ppm(PPM *im_hsv, PPM **im_quant, int **colmap, int numH, int numS, int numV, int numG) {

	int i;
	int num_bins;
	int iH, iS, iV, iG;
	double H, S, V, G;
	double Hstep = 360/(double)numH, Sstep = 1/(double)numS, Vstep = 1/(double)numV, Gstep = 255/(double)numG;
	int coords[3], rows[3], colmap_index;
	const double not_one = 0.99999999;

	/* quantise an HSV image in PPM format. The output is a PGM image,
	im_quant, containing indices into an HSV colour map, colmap. */

	/* note that greys are a special case - they occur when H == S == 0
	under my scheme, and must be treated separately */

	/* create the colourmap, giving the "centre" value of each bin */
	num_bins = numH*numS*numV + numG; 
	if (num_bins > 256) {
		fprintf(stderr, "hsv_quantise_ppm: ");
		return(TOO_MANY_BINS);
	}
	rows[HUE] = numH;
	rows[SATURATION] = numS;
	rows[VALUE] = numV;
	*colmap = (int *)malloc(3*num_bins*sizeof(int));	

	for (iV = 0, V = Vstep/2; iV < numV; iV++, V += Vstep) {
		coords[VALUE] = iV;
		for (iS = 0, S = Sstep/2; iS < numS; iS++, S += Sstep) {
			coords[SATURATION] = iS;
			for (iH = 0, H = Hstep/2; iH < numH; iH++, H += Hstep) {
				coords[HUE] = iH;
				colmap_index = coords_to_count(coords, rows, 3);
				(*colmap)[3*colmap_index + HUE] = (int)rint(255.0*H/360.0);
				(*colmap)[3*colmap_index + SATURATION] = (int)rint(255.0*S);
				(*colmap)[3*colmap_index + VALUE] = (int)rint(255.0*V);
			}
		}
	}
	for (iG = 0, G = Gstep/2; iG < numG; iG++, G += Gstep) {
		colmap_index = numH*numS*numV + iG;
		(*colmap)[3*colmap_index + HUE] = 0;
		(*colmap)[3*colmap_index + SATURATION] = 0;
		(*colmap)[3*colmap_index + VALUE] = (int)rint(G);
	}


	/* create the PGM */
	*im_quant = new_ppm();
	add_comment((*im_quant), "# HSV quantized image.\n");
	(*im_quant)->type = PGM_RAW;
	(*im_quant)->width = im_hsv->width;
	(*im_quant)->height = im_hsv->height;
	(*im_quant)->max_col_comp = 255;
	(*im_quant)->bytes_per_pixel = 1;
	(*im_quant)->pixel = (byte *)malloc((*im_quant)->width*(*im_quant)->height*sizeof(byte));

	/* now go over each pixel in the input image, and put it in the
	appropriate bin */
	for (i = 0; i < im_hsv->width*im_hsv->height; i++) {
		H = (double)im_hsv->pixel[3*i + HUE]/255.0; /* no point in
													   multiplying by 360,
													   as would just have
													   to divide again
													   below */
		S = (double)im_hsv->pixel[3*i + SATURATION]/255.0;
		V = (double)im_hsv->pixel[3*i + VALUE]/255.0;
		if ((H == 0) && (S == 0))  { /* i.e. it's a grey pixel */
			(*im_quant)->pixel[i] = (byte)numH*numS*numV + (byte)floor(not_one*V*numG);
		}
		else {
			coords[HUE] = (int)floor(not_one*H*numH);
			coords[SATURATION] = (int)floor(not_one*S*numS);
			coords[VALUE] = (int)floor(not_one*V*numV);
			(*im_quant)->pixel[i] = (byte)coords_to_count(coords, rows, 3);
		}
	}
	
	return(PPM_OK);
}

enum ppm_error colmap2rgb_ppm(PPM *im_quant, int *colmap, int num_bins, PPM **im_rgb) {

	/* take a PGM image im_quant whose entries are indices into the colour
	map colmap, and convert this to an RGB PPM image stored in im_rgb. */

	/* note that this routine is also used for HSV images */

	int i;

	/* create the PPM */
	(*im_rgb) = new_ppm();
	add_comment((*im_rgb), "# Image created from a quantised image.\n");
	(*im_rgb)->type = PPM_RAW;
	(*im_rgb)->width = im_quant->width;
	(*im_rgb)->height = im_quant->height;
	(*im_rgb)->max_col_comp = 255;
	(*im_rgb)->bytes_per_pixel = 3;
	(*im_rgb)->pixel = (byte *)malloc(3*(*im_rgb)->width*(*im_rgb)->height*sizeof(byte));
	for (i = 0; i < (*im_rgb)->width*(*im_rgb)->height; i++) {
		(*im_rgb)->pixel[3*i + RED] = colmap[3*im_quant->pixel[i] + RED];
		(*im_rgb)->pixel[3*i + GREEN] = colmap[3*im_quant->pixel[i] + GREEN];
		(*im_rgb)->pixel[3*i + BLUE] = colmap[3*im_quant->pixel[i] + BLUE];
	}

	return(PPM_OK);
}
