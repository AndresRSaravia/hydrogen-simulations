#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------------- Probabilities ---------------- */
// Uniform variable between 0 and 1
double randomfloat();

// Discrete variable between istart and iend
int udiscrete(int istart, int iend);

// Bernoulli variable 
int Bernoulli(double p);
