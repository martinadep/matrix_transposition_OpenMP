#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "explicit.h"
#include "../main.h"

/// Execute this main with different <num_threads>.
/// It computes matrix transposition with three different approaches
/// all of them optimized with OpenMP:
///
/// - naive approach
///
/// - block-based approach
///
/// - block-based loop-unrolled approach
///
/// You can store the result in a .csv file with the following format:
///
/// "matrix_size, mean, num_threads, approach"
int main(int argc, char **argv) {
    srand(time(NULL));
    if (argc != 2) {
        printf("Usage: ./explicit <num_threads> \n");
        return 1;
    }
    int num_threads = atoi(argv[1]);

    float filtered_data_S[LOOP];
    float data_S[LOOP];

    float filtered_data_B[LOOP];
    float data_B[LOOP];

    float filtered_data_L[LOOP];
    float data_L[LOOP];
    float thrsd = 2.0;

    for (int matrix_size = MIN; matrix_size <= MAX; matrix_size *= 2) {
        if (matrix_size > num_threads) {
            float **M = allocate_sqr_matrix(matrix_size);
            float **T = allocate_sqr_matrix(matrix_size);
            init_matrix(M, matrix_size);

            for (int i = 0; i < LOOP; i++) {
                data_S[i] = explicit_timing(M, T, matrix_size, num_threads, 0);
                data_B[i] = explicit_timing(M, T, matrix_size, num_threads, 1);
                data_L[i] = explicit_timing(M, T, matrix_size, num_threads, 2);
            }

            int count_filtered_S = remove_outliers(data_S, filtered_data_S, LOOP, thrsd);
            if (count_filtered_S > 0) {
                double filtered_data_mean = calculate_mean(filtered_data_S, count_filtered_S);
                // READABLE FORMAT
                //printf("[%dx%d] mean for %d threads: %.7f (naive)\n", matrix_size, matrix_size, num_threads, filtered_data_mean);

                // .CSV FORMAT
                printf("%d,%.7f,%d,naive\n", matrix_size, filtered_data_mean, num_threads);
            } else {
                printf("All values considered outliers in a row\n");
            }

            int count_filtered_B = remove_outliers(data_B, filtered_data_B, LOOP, thrsd);
            if (count_filtered_B > 0) {
                double filtered_data_mean = calculate_mean(filtered_data_B, count_filtered_B);
                // READABLE FORMAT
                //printf("[%dx%d] mean for %d threads: %.7f (block-based)\n", matrix_size, matrix_size, num_threads, filtered_data_mean);

                // .CSV FORMAT
                printf("%d,%.7f,%d,block-based\n", matrix_size, filtered_data_mean, num_threads);
            } else {
                printf("All values considered outliers in a row\n");
            }

            int count_filtered_L = remove_outliers(data_L, filtered_data_L, LOOP, thrsd);
            if (count_filtered_L > 0) {
                double filtered_data_mean = calculate_mean(filtered_data_L, count_filtered_L);
                // READABLE FORMAT
                //printf("[%dx%d] mean for %d threads: %.7f (block-based loop-unrolled)\n", matrix_size, matrix_size, num_threads, filtered_data_mean);

                // .CSV FORMAT
                printf("%d,%.7f,%d,block-basedLU\n", matrix_size, filtered_data_mean, num_threads);
            } else {
                printf("All values considered outliers in a row\n");
            }
        }
        printf("\n");
    }
    return 0;
}

/// Block-based transposition given the block size.
/// Returns the elapsed time taken for the transposition.
/// Stores the result in a text file
double explicit_timing(float **source, float **dest, int size, int num_threads, int mode) {
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
        expTransposeSeq(source, dest, size, num_threads);
        wt2 = omp_get_wtime();
        elapsed = wt2 - wt1;
        fprintf(file, "%f s - %d threads explicitTransposeSeq\n", elapsed, num_threads);
    }

    if (mode == 1) {
        // Wall clock time storing explicitTranspose
        wt1 = omp_get_wtime();
        expTransposeBB(source, dest, size, num_threads);
        wt2 = omp_get_wtime();
        elapsed = wt2 - wt1;
        fprintf(file, "%f s - %d threads explicitTransposeBlock\n", elapsed, num_threads);
    }
    if (mode == 2) {
        // Wall clock time storing explicitTranspose
        wt1 = omp_get_wtime();
        expTransposeBB_loop(source, dest, size, num_threads);
        wt2 = omp_get_wtime();
        elapsed = wt2 - wt1;
        fprintf(file, "%f s - %d threads explicitTransposeLoop\n", elapsed, num_threads);
    }
    fclose(file);
    return elapsed;
}
/// naive matrix transposition, parallelized with OMP
void expTransposeSeq(float **source, float **dest, int size, int num_threads) {
    omp_set_num_threads(num_threads);
#pragma omp parallel for collapse(2)
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            dest[i][j] = source[j][i];
        }
    }
}

/// block-based matrix transposition, parallelized with OMP
void expTransposeBB(float **source, float **dest, int size, int num_threads) {
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

/// block-based loop-unrolled matrix transposition, parallelized with OMP
void expTransposeBB_loop(float **source, float **dest, int size, int num_threads) {
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