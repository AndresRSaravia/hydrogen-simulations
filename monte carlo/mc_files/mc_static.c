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
			Eads = diff_energy_ij - diff_energy_kl;
		} else {
			numnei_kl = get_numnei(k,l,n,matrix);
			copy_from_matrix(k,l,n,tmpmatrix_kl,matrix);
			diff_energy_kl = get_lookup_value(tmpmatrix_kl, numnei_kl);
			matrix[k][l] = 0;
			numnei_ij = get_numnei(i,j,n,matrix);
			copy_from_matrix(i,j,n,tmpmatrix_ij,matrix);
			diff_energy_ij = get_lookup_value(tmpmatrix_ij, numnei_ij);
			Eads = diff_energy_kl - diff_energy_ij;
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
	int n = 100; // 100
	double k0 = 8.617333262e-5;
	double T[] = {100.,200.,300.,400.,500.,600.,700.,800.,900.,1000.};
	int Tn = 10; // 10
	int niter = 1000; // 100000 CAREFUL WITH THIS, BIG NUMBERS -> BIG STORAGE ISSUES // CUIDADO CON QUE TE LLENE LA COMPU
	int eachiter = 10;
	// File variable
	FILE *mc_kawasaki_file;
	mc_kawasaki_file = fopen("out_files/mc_kawasaki.json", "w");
	fprintf(mc_kawasaki_file, "{\n");
	for (int index = 0; index < Tn; index++) {
		fprintf(mc_kawasaki_file, "\t\"%d\":{\n", index);
		fprintf(mc_kawasaki_file, "\t\t\"n\": %d,\n", n);
		fprintf(mc_kawasaki_file, "\t\t\"k0\": %e,\n", k0);
		fprintf(mc_kawasaki_file, "\t\t\"Ti\": %f,\n", T[index]);
		fprintf(mc_kawasaki_file, "\t\t\"niter\": %d,\n", niter);
		time_t tstart;
		time(&tstart);
		fprintf(mc_kawasaki_file, "\t\t\"selections\": {\n");
		int **matrix = initialize_matrix(n);
		cover_matrix(matrix,n,0.5);
		for (int iter = 0; iter < niter; iter++) {
			for (int try = 0; try < n*n; try++) {
				data_tuple data = mc_kawasaki_selection(matrix,n,k0,T[index]);
				//printf("mc simulation T: %lf, iter: %d, state: %d ((%d,%d),(%d,%d))\n", T[index], iter, data.res, data.index_i, data.index_j, data.index_k, data.index_l);
			}
			double Eads_mean = get_Eads_mean(matrix,n);
			printf("mc simulation T: %lf, iter: %d, Eads mean: %lf\n", T[index], iter, Eads_mean);
			if (iter==niter-1) { //iter%10==0 || 
				fprintf(mc_kawasaki_file, "\t\t\t\"%d\": {\n", iter);
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
				if (iter!=niter-1) {
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
		if (index!=Tn-1) {
			fprintf(mc_kawasaki_file, "\t},\n");
		} else {
			fprintf(mc_kawasaki_file, "\t}\n");
		}
	}
	fprintf(mc_kawasaki_file, "}\n");
	fclose(mc_kawasaki_file);
}
