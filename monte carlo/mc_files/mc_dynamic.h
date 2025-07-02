#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include "../aux_files/aux_constants.h"
#include "../aux_files/aux_distribution.h"
#include "../aux_files/aux_matrix.h"

/* ---------------- Average struct ---------------- */
typedef struct {
	double theta;
	double ntotal;
	double nfirst;
	double nsecond;
} averages_tuple;

/* ---------------- Monte Carlo functions ---------------- */
// Monte Carlo step
void mc_step(int **matrix, int n, double mu, double k0, double T);

// Monte Carlo classic simulation
averages_tuple mc_simulation(int **matrix, int n, double mu, double k0, double T, int niter);

// Monte Carlo (Classic)
void mc_classic();

// Monte Carlo (Hysteresis)
void mc_hysteresis();
