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
	double theta;
	double ntotal;
	double nfirst;
	double nsecond;
} averages_tuple;

/* ---------------- Monte Carlo functions ---------------- */
// Monte Carlo step
void mc_step(int **matrix, int n, double mu, double k0, double T) {
	int **tmpmatrix = initialize_matrix(3);
	for (int tmpk = 0; tmpk < n*n; tmpk++) {
		/*if (tmpk%1000 == 0) {
			printf("%d\n", tmpk);
		}*/
		int i = udiscrete(0,n-1);
		int j = udiscrete(0,n-1);
		//printf("selection %d %d with %d\n",i,j,n);
		int numnei = get_numnei(i,j,n,matrix);
		copy_from_matrix(i,j,n,tmpmatrix,matrix);
		double diff_energy = get_lookup_value(tmpmatrix, numnei);
		double Eads = diff_energy - 0.5*(-31.744828518); // -3.6 + numnei * 0.1;
		//printf("diff de ese momento: %lf\n", diff_energy);
		//printf("Eads de ese momento: %lf\n", Eads);
		//print_array(tmpmatrix,3);
		int dN;
		double dE;
		if (matrix[i][j] == 0) {
			dN = +1;
			dE = +Eads;
		} else{
			dN = -1;
			dE = -Eads;
		}
		double expo = exp(-(dE - mu * dN) / (k0 * T));
		double p = fmin(1, expo);
		if (numnei==78) {
			printf("exponencial: %lf\n", exp(-(dE - mu * dN) / (k0 * T)));
			printf("p min: %lf\n\n", fmin(1, exp(-(dE - mu * dN) / (k0 * T))));
		}
		int res = Bernoulli(p);
		if (res == 1) {
			matrix[i][j] = 1-matrix[i][j];
		}
	}
	tmpmatrix = free_matrix(tmpmatrix,3);
}

// Monte Carlo simulation
averages_tuple mc_simulation(int **matrix, int n, double mu, double k0, double T, int niter) {
	double *averagesstep = (double *)malloc(niter * sizeof(double));
	for (int l = 0; l < niter; l++) {
		mc_step(matrix, n, mu, k0, T);
		//print_array(matrix,n);
		int count = 0;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				count += matrix[i][j];
			}
		}
		averagesstep[l] = (double)count / (n * n);
	}
	/*printf("promedios para T: %lf, mu: %lf ", T, mu);
	for (int ita = 0; ita < niter; ita++) {
		printf("%lf ", averagesstep[ita]);
	}
	printf("\n");*/
	// Average struct initialization
	averages_tuple results;
	// Calculate theta
	double meanmu = 0.0;
	for (int i = niter/2; i < niter; i++) {
		meanmu += averagesstep[i];
	}
	meanmu = meanmu/(niter/2);
	// Calculate ntotal, nfirst, nsecond
	double ntotal = 0.0;
	double nfirst = 0.0;
	double nsecond = 0.0;
	int nhydrogen = 0.0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (matrix[i][j] == 1) {
				nhydrogen++;
				int ver = matrix[(i - 1 + n) % n][j] + matrix[(i + 1) % n][j];
				int hor = matrix[i][(j - 1 + n) % n] + matrix[i][(j + 1) % n];
				int di1 = matrix[(i + 1) % n][(j + 1) % n] + matrix[(i + 1) % n][(j - 1 + n) % n];
				int di2 = matrix[(i - 1 + n) % n][(j + 1) % n] + matrix[(i - 1 + n) % n][(j - 1 + n) % n];
				ntotal += ver + hor + di1 + di2;
				nfirst += ver + hor;
				nsecond += di1 + di2;
			}
		}
	}
	if (nhydrogen == 0) {
		results.ntotal = 0;
		results.nfirst = 0;
		results.nsecond = 0;
	}
	else {
		results.ntotal = ntotal/nhydrogen;
		results.nfirst = nfirst/nhydrogen;
		results.nsecond = nsecond/nhydrogen;
	}
	results.theta = meanmu;
	//printf("promedio de mui=%lf con (%d) iteraciones: %lf\n",mu,niter/2,results.theta);
	//printf("promedio de vecinos: total %lf primeros %lf segundos %lf\n",results.ntotal,results.nfirst,results.nsecond);
	free(averagesstep);
	return results;
}

void mc_classic() {
	// Initialization of variables
	int n = 300; // 300
	int mun = 200; // 200
	double mustart = -1.0, muend = -0.25; // mustart = -1.0, muend = -0.25
	double mustep = (muend-mustart)/(mun-1);
	double mu[mun];
	for (int i = 0; i < mun; i++) {
		mu[i] = mustart + i * mustep;
	}
	double k0 = 8.617333262e-5;
	double T[] = {100.,200.,300.,400.,500.,600.,700.,800.,900.,1000.};
	int Tn = 10; // 10
	int niter = 100; // 100
	// File variable
	FILE *mc_classic_file;
	mc_classic_file = fopen("out_files/mc_classic.json", "w");
	fprintf(mc_classic_file, "{\n");
	for (int index = 0; index < Tn; index++) {
		fprintf(mc_classic_file, "\t\"%d\":{\n", index);
		fprintf(mc_classic_file, "\t\t\"n\": %d,\n", n);
		fprintf(mc_classic_file, "\t\t\"muinfo\": [%f,%f,%d],\n", mustart, muend, mun);
		fprintf(mc_classic_file, "\t\t\"k0\": %e,\n", k0);
		fprintf(mc_classic_file, "\t\t\"Ti\": %f,\n", T[index]);
		fprintf(mc_classic_file, "\t\t\"niter\": %d,\n", niter);
		time_t tstart;
		time(&tstart);
		fprintf(mc_classic_file, "\t\t\"averagesmu\": [");
		int **matrix = initialize_matrix(n);
		for (int i = 0; i < mun; i++) {
			// sin memoria
			for (int i1 = 0; i1 < n; i1++) {
				for (int i2 = 0; i2 < n; i2++) {
					matrix[i1][i2] = 0;
				}
			}
			averages_tuple results = mc_simulation(matrix,n,mu[i],k0,T[index],niter);
			printf("mc simulation T: %lf, iter: %d, mean value: %lf\n", T[index], i, results.theta);
			if (i!=mun-1) {
				fprintf(mc_classic_file, "[%e,%e,%e,%e],", results.theta, results.ntotal, results.nfirst, results.nsecond);
			}
			else{
				fprintf(mc_classic_file, "[%e,%e,%e,%e]", results.theta, results.ntotal, results.nfirst, results.nsecond);
			}
		}
		//printf("pre matrix free\n");
		for (int i1 = 0; i1 < n; i1++) {
			free(matrix[i1]);
		}
		free(matrix);
		//printf("post matrix free\n");
		fprintf(mc_classic_file, "],\n");
		time_t tend;
		time(&tend);
		printf("T: %lf, time: %lld\n", T[index], tend-tstart);
		fprintf(mc_classic_file, "\t\t\"time\": %lld\n", tend-tstart);
		if (index!=Tn-1) {
			fprintf(mc_classic_file, "\t},\n");
		}
		else {
			fprintf(mc_classic_file, "\t}\n");
		}
	}
	fprintf(mc_classic_file, "}\n");
	fclose(mc_classic_file);
}

void mc_hysteresis() {
	// Initialization of variables
	int n = 100; // 300
	int mun = 400; // 200
	double mustart = -0.69, muend = -0.655; // mustart = -1.0, muend = -0.25
	double mustep = (muend-mustart)/(mun-1);
	double mu[mun];
	for (int i = 0; i < mun; i++) {
		mu[i] = mustart + i * mustep;
	}
	double k0 = 8.617333262e-5;
	double T[] = {1.,5.,10.,15.,20.,30.,40.,50.,60.,70.,80.,90.,100.};
	int Tn = 13; // 10
	int niter = 10; // 100
	// File variable
	FILE *mc_hysteresis_file;
	mc_hysteresis_file = fopen("out_files/mc_hysteresis.json", "w");
	fprintf(mc_hysteresis_file, "{\n");
	for (int index = 0; index < Tn; index++) {
		fprintf(mc_hysteresis_file, "\t\"%d\":{\n", index);
		fprintf(mc_hysteresis_file, "\t\t\"n\": %d,\n", n);
		fprintf(mc_hysteresis_file, "\t\t\"muinfo\": [%f,%f,%d],\n", mustart, muend, mun);
		fprintf(mc_hysteresis_file, "\t\t\"k0\": %e,\n", k0);
		fprintf(mc_hysteresis_file, "\t\t\"Ti\": %f,\n", T[index]);
		fprintf(mc_hysteresis_file, "\t\t\"niter\": %d,\n", niter);
		time_t tstart;
		time(&tstart);
		fprintf(mc_hysteresis_file, "\t\t\"averagesmu\": [");
		int **matrix = initialize_matrix(n);
		for (int i = 0; i < mun; i++) {
			averages_tuple results = mc_simulation(matrix,n,mu[i],k0,T[index],niter);
			printf("mc simulation (->) T: %lf, iter: %d, mean value: %lf\n", T[index], i, results.theta);
			fprintf(mc_hysteresis_file, "[%e,%e,%e,%e],", results.theta, results.ntotal, results.nfirst, results.nsecond);
		}
		for (int i = mun-1; 0 <= i; i--) {
			averages_tuple results = mc_simulation(matrix,n,mu[i],k0,T[index],niter);
			printf("mc simulation (<-) T: %lf, iter: %d, mean value: %lf\n", T[index], i, results.theta);
			if (i!=0) {
				fprintf(mc_hysteresis_file, "[%e,%e,%e,%e],", results.theta, results.ntotal, results.nfirst, results.nsecond);
			}
			else{
				fprintf(mc_hysteresis_file, "[%e,%e,%e,%e]", results.theta, results.ntotal, results.nfirst, results.nsecond);
			}
		}
		matrix = free_matrix(matrix,n);
		fprintf(mc_hysteresis_file, "],\n");
		time_t tend;
		time(&tend);
		printf("T: %lf, time: %lld\n", T[index], tend-tstart);
		fprintf(mc_hysteresis_file, "\t\t\"time\": %lld\n", tend-tstart);
		if (index!=Tn-1) {
			fprintf(mc_hysteresis_file, "\t},\n");
		}
		else {
			fprintf(mc_hysteresis_file, "\t}\n");
		}
	}
	fprintf(mc_hysteresis_file, "}\n");
	fclose(mc_hysteresis_file);
}
