#define sq(x) ((x)*(x))

/* note that all this is taken from the paper JaH98 in ~/tex/bib/squizz.bib */
/* define some constants for the filter bank */
#define theta_step M_PI/4

/* define the functions which determine the parameters of the filter back */

#define u00 (0.5/(1.0 + tan(B_theta)))
#define sigma_m(u) (sqrt(2.0*log(2.0))/(3*u*tan(B_theta)))

#define num_gabor_scales 3
#define gabor_kernel_size_0 9
#define gabor_kernel_size_1 17
#define gabor_kernel_size_2 35

#define num_gabors_per_scale 4
