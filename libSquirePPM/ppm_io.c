#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#include "ppm.h"

/* PPM and PGM utilities */

/* David Squire 980924 */

void read_non_whitespace(FILE *f, byte *buff) {

  int i, good_char;
  byte c;

  good_char = 0;
  while (!good_char) {
    while (isspace(c = (char)fgetc(f)))
      ;
    if (c == '#')
      /* skip any comment lines */
      while ((c = (char)fgetc(f)) != '\n')
	;
    else
      good_char = 1;
  }
  i = 0;
  buff[i++] = c;
  while (!isspace(buff[i++] = (char)fgetc(f)))
    ;
  buff[--i] = 0;
}

enum file_types read_magic_no(FILE *f) {

  char magic[2];

  magic[0] = (char)fgetc(f);
  magic[1] = (char)fgetc(f);
  if (magic[0] != 'P')
    return(NOT_PPM);
  switch(magic[1]) {
  case PGM_ASC_MAGIC:
    return(PGM_ASC);
    break;
  case PGM_RAW_MAGIC:
    return(PGM_RAW);
    break;
  case PPM_ASC_MAGIC:
    return(PPM_ASC);
    break;
  case PPM_RAW_MAGIC:
    return(PPM_RAW);
    break;
  default:
    return(NOT_PPM);
    break;
  }
}

enum ppm_error read_ppm(FILE* ppm_file, PPM **ppm_data, enum file_types ppm_type) {

  byte c, buffer[256];
  int d;
  long i, num_pixel_bytes, num_read;

  /* allocate space for the ppm data structure */
  *ppm_data = new_ppm();
  (*ppm_data)->type = ppm_type;

  /* get width from file, skipping whitespace and comments */
  read_non_whitespace(ppm_file, buffer);
  (*ppm_data)->width = atoi((const char *)buffer);

  /* get height from file, skipping whitespace and comments */
  read_non_whitespace(ppm_file, buffer);
  (*ppm_data)->height = atoi((const char *)buffer);

  /* get max. colour value from file, skipping whitespace and comments */
  read_non_whitespace(ppm_file, buffer);
  (*ppm_data)->max_col_comp = (byte)atoi((const char *)buffer);
  if ((*ppm_data)->max_col_comp > 255) {
    fprintf(stderr, "maxval > 255. Too large for RAWBITS format.\n");		
    return(COL_TOO_LARGE);
  }

  switch(ppm_type) {
  case PGM_RAW: case PPM_RAW:
    if (ppm_type == PGM_RAW)
      (*ppm_data)->bytes_per_pixel = 1;
    else
      (*ppm_data)->bytes_per_pixel = 3;
    num_pixel_bytes = (*ppm_data)->bytes_per_pixel*(*ppm_data)->width*(*ppm_data)->height;
    (*ppm_data)->pixel = (byte *)malloc(num_pixel_bytes*sizeof(byte));
    if ((num_read = fread((*ppm_data)->pixel, sizeof(byte), num_pixel_bytes, ppm_file)) < num_pixel_bytes) {
      fprintf(stderr, "Read %d bytes. Expected %d.\n", num_read, num_pixel_bytes);
      if (feof(ppm_file))
	return(EARLY_EOF);
      else
	return(READ_ERROR);
    }
    break;
  case PGM_ASC: case PPM_ASC:
    if (ppm_type == PGM_ASC)
      (*ppm_data)->bytes_per_pixel = 1;
    else
      (*ppm_data)->bytes_per_pixel = 3;
    num_pixel_bytes = (*ppm_data)->bytes_per_pixel*(*ppm_data)->width*(*ppm_data)->height;
    (*ppm_data)->pixel = (byte *)malloc(num_pixel_bytes*sizeof(byte));
    num_read = 0;
    for (i = 0; i < (*ppm_data)->width*(*ppm_data)->height; i++) {
      if (ppm_type == PGM_ASC) {
	fscanf(ppm_file, "%d", &d);
	(*ppm_data)->pixel[i] = (byte)d;
	num_read++;
      }
      else {
	fscanf(ppm_file, "%d", &d);
	(*ppm_data)->pixel[3*i] = (byte)d;
	fscanf(ppm_file, "%d", &d);
	(*ppm_data)->pixel[3*i + 1] = (byte)d;
	fscanf(ppm_file, "%d", &d);
	(*ppm_data)->pixel[3*i + 2] = (byte)d;
	num_read += 3;
      }
      if(ferror(ppm_file)) {
	fprintf(stderr, "Read %d bytes. Expected %d.\n", num_read, num_pixel_bytes);
	if (feof(ppm_file))
	  return(EARLY_EOF);
	else
	  return(READ_ERROR);
      }
    }
    break;
  default:
    fprintf(stderr, "read_ppm: This should never be reached!\n");
    return(UNKNOWN_TYPE);
    break;
  }
  return(PPM_OK);
}

void write_header(FILE *outfile, PPM *im1) {

  fprintf(outfile, "# Written by Squizz's PPM/PGM utilities\n");
  if (im1->comment_length != 0)
    fprintf(outfile, "%s", im1->comments);
  fprintf(outfile, "%d %d\n", im1->width, im1->height);
  fprintf(outfile, "%d\n", (int)(im1->max_col_comp));
}

enum ppm_error write_ppm(FILE *outfile, PPM *im1, enum file_types ppm_type) {

  long i;

  switch(ppm_type) {
  case PGM_ASC: case PPM_ASC:
    if (im1->bytes_per_pixel == 1)
      fprintf(outfile, "P%c\n", PGM_ASC_MAGIC);
    else
      fprintf(outfile, "P%c\n", PPM_ASC_MAGIC);
    write_header(outfile, im1);
    if (im1->bytes_per_pixel == 1) {
      for (i = 0; i < im1->width*im1->height; i++) {
	fprintf(outfile, "%d ", im1->pixel[i]);
	if (!(i%18))
	  fprintf(outfile, "\n");
      }
    }
    else {
      for (i = 0; i < 3*im1->width*im1->height; i += 3) {
	fprintf(outfile, "%d ", im1->pixel[i]);
	fprintf(outfile, "%d ", im1->pixel[i+1]);
	fprintf(outfile, "%d\t", im1->pixel[i+2]);
	if (!(i%12))
	  fprintf(outfile, "\n");
      }
    }
    break;
  case PGM_RAW: case PPM_RAW:
    if (im1->bytes_per_pixel == 1)
      fprintf(outfile, "P%c\n", PGM_RAW_MAGIC);
    else
      fprintf(outfile, "P%c\n", PPM_RAW_MAGIC);
    write_header(outfile, im1);
    i = im1->bytes_per_pixel*im1->width*im1->height;
    fwrite(im1->pixel, sizeof(byte), i, outfile);
    break;
  default:
    fprintf(stderr, "write_ppm: This should never be reached!\n");
    return(UNKNOWN_TYPE);
  }

  return(PPM_OK);
}

enum ppm_error write_ppm_plane(FILE *outfile, PPM *im1, enum file_types ppm_type, int plane) {

  long base, i;

  if (im1->bytes_per_pixel == 1)
    return(ONE_PLANE_IN);
  switch(ppm_type) {
  case PPM_ASC: case PPM_RAW:
    return(ONE_PLANE_OUT);
    break;
  case PGM_ASC: case PGM_RAW:
    if (ppm_type == PPM_ASC)
      fprintf(outfile, "P%c\n", PGM_ASC_MAGIC);
    else
      fprintf(outfile, "P%c\n", PGM_RAW_MAGIC);
    fprintf(outfile, "# Written by Squizz's PPM/PGM utilities\n");
    fprintf(outfile, "%d %d\n", im1->width, im1->height);
    fprintf(outfile, "%d\n", (int)(im1->max_col_comp));
    for (i = 0; i < 3*im1->width*im1->height; i += 3) {
      if (ppm_type == PPM_ASC) {
	fprintf(outfile, "%d ", im1->pixel[plane + i]);
	if (!(i%18))
	  fprintf(outfile, "\n");
      }
      else
	fwrite(&(im1->pixel[plane + i]), sizeof(byte), 1, outfile);
    }
    break;
  default:
    fprintf(stderr, "write_ppm_plane: This should never be reached!\n");
    return(UNKNOWN_TYPE);
  }
  return(PPM_OK);
}
