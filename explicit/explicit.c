#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "explicit.h"
#include "../main.h"
// usa anche praagma static per assegnare lo stesso lavoro ai thread
// dimensioni ottimali 16 per matrix<2048, 64 per matrix>=2048
int main(int argc, char **argv) {
    srand(time(NULL));
    if (argc != 2) {
        printf("Usage: ./explicit <num_threads> \n");
        return 1;
    }
    int num_threads = atoi(argv[1]);

    float dati_filtratiSeq[LOOP];
    float datiSeq[LOOP];

    float dati_filtratiBlock[LOOP];
    float datiBlock[LOOP];

    float dati_filtratiLoop[LOOP];
    float datiLoop[LOOP];
    float soglia = 2.0;

    for (int matrix_size = MIN; matrix_size <= MAX; matrix_size *= 2) {
        if (matrix_size > num_threads) {
            float **M = allocate_sqr_matrix(matrix_size);
            float **T = allocate_sqr_matrix(matrix_size);
            init_matrix(M, matrix_size);

            float time_sumS = 0.0, time_sumB = 0.0, time_sumL = 0.0;
            for (int i = 0; i < LOOP; i++) {
                datiSeq[i] = explicitStoring(M, T, matrix_size, num_threads, 0);
                time_sumS += datiSeq[i];

                datiBlock[i] = explicitStoring(M, T, matrix_size, num_threads, 1);
                time_sumB += datiBlock[i];

                datiLoop[i] = explicitStoring(M, T, matrix_size, num_threads, 2);
                time_sumL += datiLoop[i];
            }

            int count_filtratiS = rimuovi_outliers(datiSeq, dati_filtratiSeq, LOOP, soglia);
            if (count_filtratiS > 0) {
                double media_filtrata = calcola_media(dati_filtratiSeq, count_filtratiS);
                //printf("Media dei dati filtrati sequential matrice [%d]x[%d]: %.7fs\n", matrix_size, matrix_size, media_filtrata);
                printf("%d,%.7f,%d,naive\n", matrix_size, media_filtrata, num_threads);
            } else {
                printf("Tutti i valori considerati outliers in una riga\n");
            }

            int count_filtratiB = rimuovi_outliers(datiBlock, dati_filtratiBlock, LOOP, soglia);
            if (count_filtratiB > 0) {
                double media_filtrata = calcola_media(dati_filtratiBlock, count_filtratiB);
                //printf("Media dei dati filtrati block matrice [%d]x[%d]: %.7fs\n", matrix_size, matrix_size, media_filtrata);
                printf("%d,%.7f,%d,block-based\n", matrix_size, media_filtrata, num_threads);
            } else {
                printf("Tutti i valori considerati outliers in una riga\n");
            }

            int count_filtratiL = rimuovi_outliers(datiLoop, dati_filtratiLoop, LOOP, soglia);
            if (count_filtratiL > 0) {
                double media_filtrata = calcola_media(dati_filtratiLoop, count_filtratiL);
                //printf("Media dei dati filtrati loop matrice [%d]x[%d]: %.7fs\n", matrix_size, matrix_size, media_filtrata);
                printf("%d,%.7f,%d,block-based_LU\n", matrix_size, media_filtrata, num_threads);
            } else {
                printf("Tutti i valori considerati outliers in una riga\n");
            }
        }
        printf("\n");
    }

    printf("Medie calcolate e salvate in 'medie.csv'\n");
    return 0;
}

/// Block-based transposition given the block size.
/// Returns the elapsed time taken for the transposition.
/// Stores the result in a text file
float explicitStoring(float **source, float **dest, int size, int num_threads, int mode) {
    if (size <= num_threads) {
        return 0.0;
    }
    // File settings to store results
    char filename[20];
    sprintf(filename, "./explicit_out/%dthreads_%dmatrix.txt", num_threads, size);
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    double wt1, wt2, elapsed = 0.0;
    if (mode == 0) {
        // Wall clock time storing explicitTranspose
        wt1 = omp_get_wtime();
        explicitTransposeSeq(source, dest, size, num_threads);
        wt2 = omp_get_wtime();
        elapsed = wt2 - wt1;
        fprintf(file, "%f s - %d threads explicitTransposeSeq\n", elapsed, num_threads);
    }

    if (mode == 1) {
        // Wall clock time storing explicitTranspose
        wt1 = omp_get_wtime();
        explicitTransposeBlock(source, dest, size, num_threads);
        wt2 = omp_get_wtime();
        elapsed = wt2 - wt1;
        fprintf(file, "%f s - %d threads explicitTransposeBlock\n", elapsed, num_threads);
    }
    if (mode == 2) {
        // Wall clock time storing explicitTranspose
        wt1 = omp_get_wtime();
        explicitTransposeLoop(source, dest, size, num_threads);
        wt2 = omp_get_wtime();
        elapsed = wt2 - wt1;
        fprintf(file, "%f s - %d threads explicitTransposeLoop\n", elapsed, num_threads);
    }
    fclose(file);
    return elapsed;
}

void explicitTransposeBlock(float **source, float **dest, int size, int num_threads) {
    int block_size = choose_block_size(size);
    omp_set_num_threads(num_threads);
#pragma omp parallel for collapse(2)
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

void explicitTransposeLoop(float **source, float **dest, int size, int num_threads) {
    int block_size = choose_block_size(size);
    omp_set_num_threads(num_threads);
#pragma omp parallel for collapse(2)
    for (int i = 0; i < size; i += block_size) {
        for (int j = 0; j < size; j += block_size) {
            for (int bi = i; bi < i + block_size && bi < size; bi += 2) {
                for (int bj = j; bj < j + block_size && bj < size; bj += 2) {
                    dest[bj][bi] = source[bi][bj];
                    dest[bj][bi + 1] = source[bi + 1][bj];
                    dest[bj + 1][bi] = source[bi][bj + 1];
                    dest[bj + 1][bi + 1] = source[bi + 1][bj + 1];
                }
            }
        }
    }
}

void explicitTransposeSeq(float **source, float **dest, int size, int num_threads) {
    omp_set_num_threads(num_threads);
#pragma omp parallel for collapse(2)
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            dest[i][j] = source[j][i];
        }
    }
}
