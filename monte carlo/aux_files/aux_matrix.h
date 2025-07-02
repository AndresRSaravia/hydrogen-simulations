#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------------- Matrix functions ---------------- */
// Initialize a matrix
int** initialize_matrix(int n);

// Free a matrix
int** free_matrix(int **matrix, int n);

// Print matrix (cite as print_array(&matrix,n))
void print_array(int **a, int n);

// Flip array left to right
void flip_left_right(int dest[3][3], int src[3][3], int n);

// Rotate array 90 degrees
void rotate_90_degrees(int dest[3][3], int src[3][3], int n);

// Check if two arrays are equal
bool array_equal(int **a, int b[3][3], int n);

// Copy matrix from table
void copy_from_lookup_table(int dest[3][3], int neighbours, int index);

// Check if array is in a certain configuration
bool is_in_configuration(int **matrix, int neighbours, int index);

// Get value from the lookup table
double get_lookup_value(int **tmpmatrix, int neighbours);

// Get the total number of neighbours in a position
int get_numnei(int i, int j, int n, int** matrix);

// Copy the configuration to a temporal matrix
void copy_from_matrix(int i, int j, int n, int** tmpmatrix, int** matrix);

// Cover matrix with a proportion p between 0 and 1
void cover_matrix(int** matrix, int n, double p);
