#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include "aux_files/aux_constants.h"
#include "aux_files/aux_distribution.h"
#include "aux_files/aux_matrix.h"
#include "mc_files/mc_dynamic.h"
#include "mc_files/mc_static.h"

/* ---------------- Main function ---------------- */
int main() {
	srand(time(NULL));
	for (int i = 0; i < 1000; i++) {udiscrete(0,5);}
	int mode = 0;
    switch (mode) {
    case 0:
		// Gran canónico
		printf("Modo Clásico\n");
        mc_classic();
        break;
    case 1:
		// Gran canónico
        printf("Modo Histéresis\n");
		mc_hysteresis();
        break;
    case 2:
		// Canónico
        printf("Modo Kawasaki\n");
		mc_kawasaki();
        break;
    default:
        printf("Modo inválido\n");
    }
	printf("Fin.\n"); 
	return 0;
}
