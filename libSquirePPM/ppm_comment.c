#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#include "ppm.h"

/* PPM and PGM utilities */

/* David Squire 980924 */

void add_comment(PPM *the_ppm, char *the_comment) {

	char *original_comments = NULL, *c;
	char *new_comment;
	int i = 0, j;

	if (the_comment[0] != '#') {
		new_comment = (char *)malloc((strlen(the_comment) + 3)*sizeof(char));
		new_comment[0] = '#';
		new_comment[1] = ' ';
		strcpy(&(new_comment[2]), the_comment);
	}
	else {
		new_comment = (char *)malloc((strlen(the_comment) + 1)*sizeof(char));
		strcpy(new_comment, the_comment);
	}

	if (the_ppm->comment_length != 0) {
		original_comments = malloc(the_ppm->comment_length*sizeof(char));
		c = the_ppm->comments;
		while (*c != '\0')
			original_comments[i++] = *c++;
		original_comments[i] = '\0';
	}

	if (the_ppm->comments != NULL)
		free(the_ppm->comments);

	the_ppm->comment_length = i + strlen(new_comment);
	the_ppm->comments = malloc((the_ppm->comment_length+1)*sizeof(char));
	i = 0;
	if (original_comments != NULL) {
		c = original_comments;
		while (*c != '\0')
			the_ppm->comments[i++] = *c++;
	}
	for (j = 0; j < strlen(new_comment); j++, i++)
		the_ppm->comments[i] = new_comment[j];
	the_ppm->comments[i] = '\0';

	free(new_comment);
}
