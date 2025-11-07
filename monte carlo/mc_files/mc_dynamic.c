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
	int add_attempt;
	int add_success;
	int rem_attempt;
	int rem_success;
} dynamic_stats_tuple;

typedef struct 
{
	int add_attempt;
	int add_success;
	int rem_attempt;
	int rem_success;
}  step_stats_tuple;

/* ---------------- Monte Carlo functions ---------------- */
// Monte Carlo tries
step_stats_tuple mc_n2tries(int **matrix, int n, double mu, double k0, double T) {
	step_stats_tuple tmp_stats;
	tmp_stats.add_attempt = 0;
	tmp_stats.add_success = 0;
	tmp_stats.rem_attempt = 0;
	tmp_stats.rem_success = 0;

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
		double Eads = diff_energy - E_H;
		//printf("diff de ese momento: %lf\n", diff_energy);
		//printf("Eads de ese momento: %lf\n", Eads);
		//print_array(tmpmatrix,3);
		int dN;
		double dE;
		if (matrix[i][j] == 0) {
			tmp_stats.add_attempt++;
			dN = +1;
			dE = +Eads;
		} else{
			tmp_stats.rem_attempt++;
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
			if (matrix[i][j] == 0) {
				tmp_stats.add_success++;
			} else{
				tmp_stats.rem_success++;
			}
			matrix[i][j] = 1-matrix[i][j];
		}
	}
	tmpmatrix = free_matrix(tmpmatrix,3);
	return tmp_stats;
}

// Monte Carlo steps
dynamic_stats_tuple mc_steps(int **matrix, int n, double mu, double k0, double T, int nstep) {
	dynamic_stats_tuple results;
	results.theta = 0;
	results.ntotal = 0;
	results.nfirst = 0;
	results.nsecond = 0;
	results.add_attempt = 0;
	results.add_success = 0;
	results.rem_attempt = 0;
	results.rem_success = 0;
	double *averagesstep = (double *)malloc(nstep * sizeof(double));
	for (int l = 0; l < nstep; l++) {
		step_stats_tuple tmp_stats = mc_n2tries(matrix, n, mu, k0, T);
		results.add_attempt += tmp_stats.add_attempt;
		results.add_success += tmp_stats.add_success;
		results.rem_attempt += tmp_stats.rem_attempt;
		results.rem_success += tmp_stats.rem_success;
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
	for (int ita = 0; ita < nstep; ita++) {
		printf("%lf ", averagesstep[ita]);
	}
	printf("\n");*/
	// Average struct initialization
	dynamic_stats_tuple results;
	// Calculate theta
	double meanmu = 0.0;
	for (int i = nstep/2; i < nstep; i++) {
		meanmu += averagesstep[i];
	}
	meanmu = meanmu/(nstep/2);
	// Calculate ntotal, nfirst, nsecond, 
	int nhydrogen = 0.0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (matrix[i][j] == 1) {
				nhydrogen++;
				int ver = matrix[(i - 1 + n) % n][j] + matrix[(i + 1) % n][j];
				int hor = matrix[i][(j - 1 + n) % n] + matrix[i][(j + 1) % n];
				int di1 = matrix[(i + 1) % n][(j + 1) % n] + matrix[(i + 1) % n][(j - 1 + n) % n];
				int di2 = matrix[(i - 1 + n) % n][(j + 1) % n] + matrix[(i - 1 + n) % n][(j - 1 + n) % n];
				results.ntotal += ver + hor + di1 + di2;
				results.nfirst += ver + hor;
				results.nsecond += di1 + di2;
			}
		}
	}
	if (nhydrogen != 0) {
		results.ntotal = results.ntotal/nhydrogen;
		results.nfirst = results.nfirst/nhydrogen;
		results.nsecond = results.nsecond/nhydrogen;
	}
	results.theta = meanmu;
	//printf("promedio de mui=%lf con (%d) pasos: %lf\n",mu,nstep/2,results.theta);
	//printf("promedio de vecinos: total %lf primeros %lf segundos %lf\n",results.ntotal,results.nfirst,results.nsecond);
	free(averagesstep);
	return results;
}

void mc_classic() {
	// Initialization of variables
	int n = 200; // 200
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
	int nstep = 100; // 100
	// File variable
	char filename[1024] = "out_files/mc_classic.json";
	snprintf(filename, sizeof(filename), "out_files/mc_classic (n=%d, muinfo=(%f,%f,%d), steps=%d).json", n, mustart, muend, mun, nstep);
	FILE *mc_classic_file;
	mc_classic_file = fopen(filename, "w");
	fprintf(mc_classic_file, "{\n");
	for (int index = 0; index < Tn; index++) {
		fprintf(mc_classic_file, "\t\"%f\":{\n", T[index]);
		fprintf(mc_classic_file, "\t\t\"n\": %d,\n", n);
		fprintf(mc_classic_file, "\t\t\"muinfo\": [%f,%f,%d],\n", mustart, muend, mun);
		fprintf(mc_classic_file, "\t\t\"nstep\": %d,\n", nstep);
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
			dynamic_stats_tuple results = mc_steps(matrix,n,mu[i],k0,T[index],nstep);
			printf("mc simulation T: %lf, mu: %d, mean value: %lf\n", T[index], i, results.theta);
			if (i!=mun-1) {
				fprintf(mc_classic_file, "[%e,%e,%e,%e,%e,%e,%e],", results.theta, results.ntotal, results.nfirst, results.nsecond, results.add_attempt, results.add_success, results.rem_attempt, results.rem_success);
			}
			else{
				fprintf(mc_classic_file, "[%e,%e,%e,%e,%e,%e,%e]", results.theta, results.ntotal, results.nfirst, results.nsecond, results.add_attempt, results.add_success, results.rem_attempt, results.rem_success);
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
	int n = 200; // 200
	int mun = 400; // 400
	double mustart = -0.69, muend = -0.655; // mustart = -1.0, muend = -0.25
	double mustep = (muend-mustart)/(mun-1);
	double mu[mun];
	for (int i = 0; i < mun; i++) {
		mu[i] = mustart + i * mustep;
	}
	double k0 = 8.617333262e-5;
	double T[] = {1.,5.,10.,15.,20.,30.,40.,50.,60.,70.,80.,90.,100.};
	int Tn = 13; // 13
	int nstep = 100; // 100
	// File variable
	char filename[1024] = "out_files/mc_hysteresis.json";
	snprintf(filename, sizeof(filename), "out_files/mc_hysteresis (n=%d, muinfo=(%f,%f,%d), steps=%d).json", n, mustart, muend, mun, nstep);
	FILE *mc_hysteresis_file;
	mc_hysteresis_file = fopen(filename, "w");
	fprintf(mc_hysteresis_file, "{\n");
	for (int index = 0; index < Tn; index++) {
		fprintf(mc_hysteresis_file, "\t\"%f\":{\n", T[index]);
		fprintf(mc_hysteresis_file, "\t\t\"n\": %d,\n", n);
		fprintf(mc_hysteresis_file, "\t\t\"muinfo\": [%f,%f,%d],\n", mustart, muend, mun);
		fprintf(mc_hysteresis_file, "\t\t\"nstep\": %d,\n", nstep);
		time_t tstart;
		time(&tstart);
		fprintf(mc_hysteresis_file, "\t\t\"averagesmu\": [");
		int **matrix = initialize_matrix(n);
		for (int i = 0; i < mun; i++) {
			dynamic_stats_tuple results = mc_steps(matrix,n,mu[i],k0,T[index],nstep);
			printf("mc simulation (->) T: %lf, mu: %f, mean value: %lf\n", T[index], mu[i], results.theta);
			fprintf(mc_hysteresis_file, "[%e,%e,%e,%e,%e,%e,%e],", results.theta, results.ntotal, results.nfirst, results.nsecond, results.add_attempt, results.add_success, results.rem_attempt, results.rem_success);
		}
		for (int i = mun-1; 0 <= i; i--) {
			dynamic_stats_tuple results = mc_steps(matrix,n,mu[i],k0,T[index],nstep);
			printf("mc simulation (<-) T: %lf, mu: %f, mean value: %lf\n", T[index], mu[i], results.theta);
			if (i!=0) {
				fprintf(mc_hysteresis_file, "[%e,%e,%e,%e,%e,%e,%e],", results.theta, results.ntotal, results.nfirst, results.nsecond, results.add_attempt, results.add_success, results.rem_attempt, results.rem_success);
			}
			else{
				fprintf(mc_hysteresis_file, "[%e,%e,%e,%e,%e,%e,%e]", results.theta, results.ntotal, results.nfirst, results.nsecond, results.add_attempt, results.add_success, results.rem_attempt, results.rem_success);
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
