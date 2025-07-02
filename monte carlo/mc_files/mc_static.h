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
	int index_i;
	int index_j;
	int index_k;
	int index_l;
	bool res;
} data_tuple;

/* ---------------- Monte Carlo functions ---------------- */
// Get Eads mean
double get_Eads_mean(int **matrix, int n);

// Monte Carlo selection
void mc_kawasaki_selection(int **matrix, int n, double k0, double T);

// Monte Carlo (Kawasaki)
void mc_kawasaki();
