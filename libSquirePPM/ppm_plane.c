#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#include <ppm.h>

/* PPM and PGM utilities */
 
/* David Squire 990518 */

PPM *ppm_plane(PPM *im_input, int plane_wanted) {
	
	PPM  *im_output;
	int i, bytes_per_pixel;
	char comment[80];

	if ((im_input->type == PGM_ASC) || (im_input->type == PGM_RAW)) {
		fprintf(stderr, "Can't extract a plane from a PGM image!\n");
		return (NULL);
	}	

	/* set up the output PGM image */
	im_output = new_ppm();
	/* this was causing problems for "convert"
	sprintf(comment, "# Plane %d of image\n", plane_wanted);
	add_comment(im_output, comment);
	*/
	im_output->type = PGM_RAW;
	im_output->width = im_input->width;
	im_output->height = im_input->height;
	im_output->max_col_comp = im_input->max_col_comp;
	im_output->bytes_per_pixel = 1;
	im_output->pixel = (byte *)malloc(im_output->width*im_output->height*sizeof(byte));

	/* extract the desired plane */
	bytes_per_pixel = im_input->bytes_per_pixel;
	for (i = 0; i < im_output->width*im_output->height; i++)
		im_output->pixel[i] = im_input->pixel[bytes_per_pixel*i + plane_wanted];

	return(im_output);
}
