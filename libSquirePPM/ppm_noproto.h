/* constants and structures for PPM and PGM utilities */

/* David Squire 980922 */

#ifndef dprint
#define dprint(expr) fprintf(stderr, #expr " = %f\n", (float)expr)
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

typedef unsigned char byte;
#define MAX_BYTE 255
#define MIN_BYTE 0

#define PGM_ASC_MAGIC '2'
#define PGM_RAW_MAGIC '5'
#define PPM_ASC_MAGIC '3'
#define PPM_RAW_MAGIC '6'

enum file_types {
	PGM_ASC,
	PGM_RAW,
	PPM_ASC,
	PPM_RAW,
	NOT_PPM
};

enum ppm_error {
	PPM_OK,
	UNKNOWN_TYPE,
	EARLY_EOF,
	READ_ERROR,
	COL_TOO_LARGE,
	ONE_PLANE_IN,
	ONE_PLANE_OUT,
	THREE_PLANES_NEEDED,
	ONE_PLANE_NEEDED,
	TOO_MANY_BINS,
	FILE_OPEN_ERROR,
	FILE_READ_ERROR,
	FILE_WRITE_ERROR,
	FILE_CLOSE_ERROR
};

/* planes of a colour image */
#define RED		0
#define GREEN	1
#define BLUE	2

#define HUE			0
#define SATURATION	1
#define VALUE		2

typedef struct _PPM {
	enum file_types type;
	int width, height;
	byte max_col_comp;
	byte bytes_per_pixel;
	char *comments;
	int comment_length;
	byte *pixel;
} PPM;

