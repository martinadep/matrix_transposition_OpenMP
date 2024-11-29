#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "implicit.h"
#include "../main.h"

int main(int argc, char **argv) {
    srand(time(NULL));
    if (argc != 2) {
        printf("Usage: implicit_block_size <block_size>\n");
        return 1;
    }
    int block_size = atoi(argv[1]);

    float dati_filtrati[LOOP];
    float dati[LOOP];
    float soglia = 2.0;

    for (int matrix_size = MIN; matrix_size <= MAX; matrix_size *= 2) {
        if (matrix_size > block_size) {
            float **M = allocate_sqr_matrix(matrix_size);
            float **T = allocate_sqr_matrix(matrix_size);
            init_matrix(M, matrix_size);

            float time_sum = 0.0;
            for (int i = 0; i < LOOP; i++) {
                dati[i] = blockbasedsizeStoring(M, T, matrix_size, block_size);
                time_sum += dati[i];
            }

            int count_filtrati = rimuovi_outliers(dati, dati_filtrati, LOOP, soglia);
            if (count_filtrati > 0) {
                double media_filtrata = calcola_media(dati_filtrati, count_filtrati);
                //printf("Media dei dati filtrati matrice [%d]x[%d]: %.7fs\n", matrix_size, matrix_size, media_filtrata);
                printf("%d,%.7f,%d\n", matrix_size, media_filtrata, block_size);
            } else {
                printf("Tutti i valori considerati outliers in una riga\n");
            }
        }
    }
    return 0;
}

/// Block-based transposition given the block size.
/// Returns the elapsed time taken for the transposition.
/// Stores the result in a text file
float blockbasedsizeStoring(float **M, float **T, int size, int block_size) {
    if (size <= block_size) {
        return 0.0;
    }
    // File settings to store results
    char filename[20];
    sprintf(filename, "./blockbasedsize_out/%dblock_%dmatrix.txt", block_size, size);
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    // Wall clock time storing
    double wt1= omp_get_wtime();

    blockBasedTranspose(M, T, size, block_size);

    double wt2 = omp_get_wtime();
    double elapsed = wt2 - wt1;
    fprintf(file, "%f s - %d block\n", elapsed, block_size);
    fclose(file);
    return elapsed;
}

/// Block-based transpose
void blockBasedTranspose(float **source, float **dest, int size, int block_size) {
    for (int i = 0; i < size; i += block_size) {
        for (int j = 0; j < size; j += block_size) {
            for (int bi = i; bi < i + block_size && bi < size; bi++) {
                for (int bj = j; bj < j + block_size && bj < size; bj++) {
                    dest[bj][bi] = source[bi][bj];
                }
            }
        }
    }
}
