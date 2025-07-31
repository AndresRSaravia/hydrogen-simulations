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
typedef struct 
{
	int index_i;
	int index_j;
	int index_k;
	int index_l;
	bool res;
} data_tuple;

/* ---------------- Monte Carlo functions ---------------- */
// Get Eads mean
double get_Eads_mean(int **matrix, int n) {
	int **tmpmatrix_ij = initialize_matrix(3);
	double Eads_mean = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			int numnei_ij = get_numnei(i,j,n,matrix);
			copy_from_matrix(i,j,n,tmpmatrix_ij,matrix);
			double diff_energy_ij = get_lookup_value(tmpmatrix_ij, numnei_ij);
			Eads_mean += diff_energy_ij - 0.5*(-31.744828518);
		}
		
	}
	return Eads_mean/(n*n);
}
	
// Monte Carlo selection
data_tuple mc_kawasaki_selection(int **matrix, int n, double k0, double T) {
	data_tuple data;
	int **tmpmatrix_ij = initialize_matrix(3);
	int **tmpmatrix_kl = initialize_matrix(3);
	int i = udiscrete(0,n-1);
	int j = udiscrete(0,n-1);
	int k = udiscrete(0,n-1);
	int l = udiscrete(0,n-1);
	data.index_i = i;
	data.index_j = j;
	data.index_k = k;
	data.index_l = l;
	data.res = matrix[i][j]!=matrix[k][l];
	//printf("selection %d %d with %d\n",i,j,n);
	//printf("selection %d %d with %d\n",k,l,n);
	if (matrix[i][j]!=matrix[k][l]) {
		bool hydrogen_ij = matrix[i][j]==1;
		int numnei_ij,numnei_kl;
		double diff_energy_ij,diff_energy_kl;
		double Eads;
		if (hydrogen_ij) {
			numnei_ij = get_numnei(i,j,n,matrix);
			copy_from_matrix(i,j,n,tmpmatrix_ij,matrix);
			diff_energy_ij = get_lookup_value(tmpmatrix_ij, numnei_ij);
			matrix[i][j] = 0;
			numnei_kl = get_numnei(k,l,n,matrix);
			copy_from_matrix(k,l,n,tmpmatrix_kl,matrix);
			diff_energy_kl = get_lookup_value(tmpmatrix_kl, numnei_kl);
			Eads = diff_energy_kl - diff_energy_ij;
		} else {
			numnei_kl = get_numnei(k,l,n,matrix);
			copy_from_matrix(k,l,n,tmpmatrix_kl,matrix);
			diff_energy_kl = get_lookup_value(tmpmatrix_kl, numnei_kl);
			matrix[k][l] = 0;
			numnei_ij = get_numnei(i,j,n,matrix);
			copy_from_matrix(i,j,n,tmpmatrix_ij,matrix);
			diff_energy_ij = get_lookup_value(tmpmatrix_ij, numnei_ij);
			Eads = diff_energy_ij - diff_energy_kl;
		}
		//printf("diff de ese momento: %lf ; %lf\n", diff_energy_ij,diff_energy_kl);
		//printf("Eads de ese momento: %lf\n", Eads);
		//print_array(tmpmatrix,3);
		double dE = Eads;
		double expo = exp(-(dE) / (k0 * T));
		double p = fmin(1, expo);
		if (numnei_ij==78) {
			printf("exponencial: %lf\n", exp(-(dE) / (k0 * T)));
			printf("p min: %lf\n\n", fmin(1, exp(-(dE) / (k0 * T))));
		}
		int res = Bernoulli(p);
		if (res == 1 && hydrogen_ij) {
			matrix[k][l] = 1;
		} else if (res == 1 && !hydrogen_ij) {
			matrix[i][j] = 1;
		} else if (res == 0 && hydrogen_ij) {
			matrix[i][j] = 1;
		} else { // (res == 0 && !hydrogen_ij)
			matrix[k][l] = 1;
		}
	}
	tmpmatrix_ij = free_matrix(tmpmatrix_ij,3);
	tmpmatrix_kl = free_matrix(tmpmatrix_kl,3);
	return data;
}

// Monte Carlo (Kawasaki)
void mc_kawasaki() {
	// Initialization of variables
	int n = 200; // 200
	double k0 = 8.617333262e-5;
	double T[] = {20.,40.,60.,80.,100.};
	int Tn = 5; // 5
	double covers[] = {0.3,0.5,0.7};
	int ncover = 3; // 3
	int nstep = 2000; // 3000
	int eachstep = 100; // PLEASE CAREFUL WITH THIS, CONSIDER THE RATIO OF VARIABLES n, nstep and eachstep BIG NUMBERS -> BIG STORAGE
	int laststep = 5;
	// File variable
	char filename[1024] = "out_files/mc_kawasaki.json";
	snprintf(filename, sizeof(filename), "out_files/mc_kawasaki (n=%d, steps=%d).json", n, nstep);
	FILE *mc_kawasaki_file;
	mc_kawasaki_file = fopen(filename, "w");
	fprintf(mc_kawasaki_file, "{\n");
	for (int coveri = 0; coveri < ncover; coveri++) {
		for (int index = 0; index < Tn; index++) {
			fprintf(mc_kawasaki_file, "\t\"(%f,%f)\":{\n", covers[coveri],T[index]);
			fprintf(mc_kawasaki_file, "\t\t\"n\": %d,\n", n);
			fprintf(mc_kawasaki_file, "\t\t\"nstep\": %d,\n", nstep);
			fprintf(mc_kawasaki_file, "\t\t\"eachstep\": %d,\n", eachstep);
			fprintf(mc_kawasaki_file, "\t\t\"laststep\": %d,\n", laststep);
			time_t tstart;
			time(&tstart);
			fprintf(mc_kawasaki_file, "\t\t\"selections\": {\n");
			int **matrix = initialize_matrix(n);
			cover_matrix(matrix,n,covers[coveri]);
			for (int step = 0; step < nstep; step++) {
				for (int try = 0; try < n*n; try++) {
					data_tuple data = mc_kawasaki_selection(matrix,n,k0,T[index]);
					//printf("mc simulation T: %lf, step: %d, state: %d ((%d,%d),(%d,%d))\n", T[index], step, data.res, data.index_i, data.index_j, data.index_k, data.index_l);
				}
				printf("mc simulation cover: %f T: %lf, step: %d\n", covers[coveri], T[index], step);
				if (step%eachstep==0 || step==nstep-1 || step>=nstep-laststep) {
					fprintf(mc_kawasaki_file, "\t\t\t\"%d\": {\n", step);
					//fprintf(mc_kawasaki_file, "\t\t\t\t\"indexes_ijkl_postvalues\": [[%d,%d,%d],[%d,%d,%d]],\n", data.index_i, data.index_j, matrix[data.index_i][data.index_j], data.index_k, data.index_l, matrix[data.index_k][data.index_l]);
					fprintf(mc_kawasaki_file, "\t\t\t\t\"matrix\": [\n");
					for (int i = 0; i < n; i++) {
						fprintf(mc_kawasaki_file, "\t\t\t\t\t[");
						for (int j = 0; j < n; j++) {
							if (j!=n-1) {
								fprintf(mc_kawasaki_file, "%d,",matrix[i][j]);
							} else if (i!=n-1) {
								fprintf(mc_kawasaki_file, "%d],\n",matrix[i][j]);
							} else {
								fprintf(mc_kawasaki_file, "%d]\n",matrix[i][j]);
							}
						}
					}
					fprintf(mc_kawasaki_file, "\t\t\t\t]\n");
					if (step!=nstep-1) {
						fprintf(mc_kawasaki_file, "\t\t\t},\n");
					} else {
						fprintf(mc_kawasaki_file, "\t\t\t}\n");
					}
				} else {
					//fprintf(mc_kawasaki_file, "\t\t\t\t\"indexes_ijkl_postvalues\": [[%d,%d,%d],[%d,%d,%d]]\n", data.index_i, data.index_j, matrix[data.index_i][data.index_j], data.index_k, data.index_l, matrix[data.index_k][data.index_l]);
				}
			}
			matrix = free_matrix(matrix,n);
			fprintf(mc_kawasaki_file, "\t\t},\n");
			time_t tend;
			time(&tend);
			printf("T: %lf, time: %lld\n", T[index], tend-tstart);
			fprintf(mc_kawasaki_file, "\t\t\"time\": %lld\n", tend-tstart);
			if (index!=Tn-1 || coveri!=ncover-1) {
				fprintf(mc_kawasaki_file, "\t},\n");
			} else {
				fprintf(mc_kawasaki_file, "\t}\n");
			}
		}
	}
	fprintf(mc_kawasaki_file, "}\n");
	fclose(mc_kawasaki_file);
}
