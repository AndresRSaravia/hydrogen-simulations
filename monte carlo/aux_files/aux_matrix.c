#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include "aux_constants.h"
#include "aux_distribution.h"

/* ---------------- Matrix functions ---------------- */
// Initialize a matrix
int** initialize_matrix(int n) {
	int** matrix = (int**)malloc(n * sizeof(int*));
	for (int i = 0; i < n; i++) {
		matrix[i] = (int*)calloc(n, sizeof(int));
	}
	return matrix;
}

// Free a matrix
int** free_matrix(int **matrix, int n) {
	for (int i = 0; i < n; i++) {
		free(matrix[i]);
	}
	free(matrix);
	return NULL;
}

// Print matrix (cite as print_array(&matrix,n))
void print_array(int **a, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			printf("%d ",a[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

// Flip array left to right
void flip_left_right(int dest[3][3], int src[3][3], int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			dest[i][j] = src[i][n - 1 - j];
		}
	}
}

// Rotate array 90 degrees
void rotate_90_degrees(int dest[3][3], int src[3][3], int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			dest[j][n - 1 - i] = src[i][j];
		}
	}
}

// Check if two arrays are equal
bool array_equal(int **a, int b[3][3], int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (a[i][j] != b[i][j]) {
				return false;
			}
		}
	}
	return true;
}

// Copy matrix from table
void copy_from_lookup_table(int dest[3][3], int neighbours, int index) {
	int n = 3;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			dest[i][j] = configuration_table[neighbours][index][i][j];
		}
	}
}

// Check if array is in a certain configuration
bool is_in_configuration(int **matrix, int neighbours, int index) {
	int n = 3;
	int r0[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
	int r1[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
	int r2[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
	int r3[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
	int r0f[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
	int r1f[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
	int r2f[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
	int r3f[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
	copy_from_lookup_table(r0,neighbours,index);
	rotate_90_degrees(r1,r0,n);
	rotate_90_degrees(r2,r1,n);
	rotate_90_degrees(r3,r2,n);
	flip_left_right(r0f,r0,n);
	flip_left_right(r1f,r1,n);
	flip_left_right(r2f,r2,n);
	flip_left_right(r3f,r3,n);
	/*
	printf("array candidate with %d %d\n", neighbours, index);
	print_array(matrix,n);
	printf("r0\n");
	print_array(r0,n);
	printf("r1\n");
	print_array(r1,n);
	printf("r2\n");
	print_array(r2,n);
	printf("r3\n");
	print_array(r3,n);
	printf("r0f\n");
	print_array(r0f,n);
	printf("r1f\n");
	print_array(r1f,n);
	printf("r2f\n");
	print_array(r2f,n);
	printf("r3f\n");
	print_array(r3f,n);
	*/
	bool part = array_equal(matrix,r0,n) || array_equal(matrix,r1,n) || array_equal(matrix,r2,n) || array_equal(matrix,r3,n) \
				|| array_equal(matrix,r0f,n) || array_equal(matrix,r1f,n) || array_equal(matrix,r2f,n) || array_equal(matrix,r3f,n);
	return part;
}

// Get value from the lookup table
double get_lookup_value(int **tmpmatrix, int neighbours) {
	int k = 0;
	bool flag = true;
	while (flag) {
		if (neighbours==70) {
			printf("k del while: %d\n", k);
		}
		if (is_in_configuration(tmpmatrix, neighbours, k)) {
			flag = false;
		} else {
			k++;
		}
	} // k < configuration_total[neighbours] && flag
	return diff_energy_table[neighbours][k];
}

// Get the total number of neighbours in a position
int get_numnei(int i, int j, int n, int** matrix) {
	int ver = matrix[(i - 1 + n) % n][j] + matrix[(i + 1) % n][j];
	int hor = matrix[i][(j - 1 + n) % n] + matrix[i][(j + 1) % n];
	int di1 = matrix[(i + 1) % n][(j + 1) % n] + matrix[(i + 1) % n][(j - 1 + n) % n];
	int di2 = matrix[(i - 1 + n) % n][(j + 1) % n] + matrix[(i - 1 + n) % n][(j - 1 + n) % n];
	//printf("ver and hor values %d %d\n",ver,hor);
	int numnei = ver + hor + di1 + di2;
	return numnei;
}

// Copy the configuration to a temporal matrix
void copy_from_matrix(int i, int j, int n, int** tmpmatrix, int** matrix) {
	tmpmatrix[0][0] = matrix[(i - 1 + n) % n][(j - 1 + n) % n];
	tmpmatrix[0][1] = matrix[(i - 1 + n) % n][j];
	tmpmatrix[0][2] = matrix[(i - 1 + n) % n][(j + 1) % n];
	tmpmatrix[1][0] = matrix[i][(j - 1 + n) % n];
	tmpmatrix[1][1] = 0;
	tmpmatrix[1][2] = matrix[i][(j + 1) % n];
	tmpmatrix[2][0] = matrix[(i + 1) % n][(j - 1 + n) % n];
	tmpmatrix[2][1] = matrix[(i + 1) % n][j];
	tmpmatrix[2][2] = matrix[(i + 1) % n][(j + 1) % n];
}

// Cover matrix with a proportion p between 0 and 1
void cover_matrix(int** matrix, int n, double p) {
	int coverage = n*n*p;
	int progress = 0;
	while (progress<coverage) {
		int i = udiscrete(0,n-1);
		int j = udiscrete(0,n-1);
		if (matrix[i][j] == 0) {
			matrix[i][j] = 1;
			progress++;
		}
	}
}
