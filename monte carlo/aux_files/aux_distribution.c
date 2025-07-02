#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

// Miracle counter (random got to 1, weiiird)
int count_miracles = 0;

/* ---------------- Probabilities ---------------- */
// Uniform variable between 0 and 1
double randomfloat() {
	double U = ((double)rand())/((double)(RAND_MAX));
	return U;
}

// Discrete variable between istart and iend
int udiscrete(int istart, int iend) {
	double U = randomfloat();
	if (U==1.) {
		//printf("a miracle!\n");
		count_miracles++;
		return iend;
	} else
	{
		//printf("random %lf\n",U);
		return (int)(U*(double)((iend+1-istart)))+istart;
	}	
}

// Bernoulli variable 
int Bernoulli(double p) {
	double U = randomfloat(); // / RAND_MAX
	if (U < p) {
		return 1;
	}else{
		return 0;
	}
}
