/* constants and structures for PPM and PGM utilities */

/* David Squire 980922 */
#include <stdio.h>
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
	FILE_CLOSE_ERROR,
	MEMORY_ERROR
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
        /* a temporary cache location, so that we dont convert double->int->double for the entire image in extract_features */
        /* a copy of the value plane, in reverse order, as doubles */
        double * value_plane_double_reversed;
} PPM;

#ifdef __STDC__
# define	P(s) s
#else
# define P(s) ()
#endif


/* ppm_comment.c */
void add_comment P((PPM *the_ppm , char *the_comment ));

/* ppm_error.c */
void ppm_handle_error P((enum ppm_error the_error ));

/* ppm_io.c */
void read_non_whitespace P((FILE *f , byte *buff ));
enum file_types read_magic_no P((FILE *f ));
enum ppm_error read_ppm P((FILE *ppm_file , PPM **ppm_data , enum file_types ppm_type ));
void write_header P((FILE *outfile , PPM *im1 ));
enum ppm_error write_ppm P((FILE *outfile , PPM *im1 , enum file_types ppm_type ));
enum ppm_error write_ppm_plane P((FILE *outfile , PPM *im1 , enum file_types ppm_type , int plane ));

/* ppm_memory.c */
PPM *new_ppm P((void ));
void destroy_ppm P((PPM **the_ppm ));

/* ppm_normalize.c */
double *ppm_local_normalize P((PPM *im_input , int window_size , double (*variance_transformer )(double )));
PPM *ppm_double_to_byte P((double *dbl_image , int width , int height ));
PPM *ppm_norm_contrast P((PPM *im_input ));
PPM *ppm_local_norm_contrast P((PPM *im_input , int window_size ));

/* ppm_plane.c */
PPM *ppm_plane P((PPM *im_input , int plane_wanted ));

/* ppm_subimage.c */
PPM *ppm_subimage P((PPM *source_image , int top_left_x , int top_left_y , int width , int height ));

#undef P
