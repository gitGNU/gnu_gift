#define sq(x) ((x)*(x))

/* note that all this is taken from the paper JaH98 in ~/tex/bib/squizz.bib */
/* with my corrections! */

/* define the functions which determine the parameters of the filter back */


#define num_gabor_scales 3
#define gabor_kernel_size_0 9
#define gabor_kernel_size_1 17
#define gabor_kernel_size_2 35

#define num_gabors_per_scale 4
#define theta_step (M_PI/num_gabors_per_scale)
#define u00 (0.5/(1.0 + 1.0/3.0))
#define sigma_m(u) (3.0*sqrt(2.0*log(2.0))/(2.0*M_PI*u))
