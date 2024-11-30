#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "implicit.h"
#include "../main.h"
#include<omp.h>

/// Compile this main with different optimization flags.
/// It computes matrix transposition with a loop-unrolled block-based approach.
/// You can store the result in a .csv file with the following format:
///
/// "matrix_size, mean"
int main() {
    srand(time(NULL));
    float filtered_data[LOOP];
    float data[LOOP];
    float thrsd = 2.0;

    for (int matrix_size = MIN; matrix_size <= MAX; matrix_size *= 2) {
        float **M = allocate_sqr_matrix(matrix_size);
        float **T = allocate_sqr_matrix(matrix_size);
        init_matrix(M, matrix_size);


        for (int i = 0; i < LOOP; i++) {
            data[i] = blockbased_timing(M, T, matrix_size);
        }
        int count_filtered = remove_outliers(data, filtered_data, LOOP, thrsd);
        if (count_filtered > 0) {
            double filtered_data_mean = calculate_mean(filtered_data, count_filtered);
            // READABLE FORMAT
            //printf("[%dx%d] mean: %.7f\n", matrix_size, matrix_size, filtered_data_mean);

            // .CSV FORMAT
            printf("%d,%.7f\n", matrix_size, filtered_data_mean);
        } else {
            printf("All values considered outliers in a row\n");
        }
    }
    return 0;
}

/// Returns the elapsed time taken for the block-based loop-unrolled transposition.
///
/// The block size is chosen according to @param[in] matrix_size
///
double blockbased_timing(float **M, float **T, int matrix_size) {
    double wt1= omp_get_wtime();
    int block_size = choose_block_size(matrix_size);
    block_based_transpose(M, T, matrix_size, block_size);
    double wt2 = omp_get_wtime();
    double elapsed = wt2 - wt1;
    return elapsed;
}

/// Block-based transposition with loop-unrolling
void block_based_transpose(float **source, float **dest, int matrix_size, int block_size) {
    for (int i = 0; i < matrix_size; i += block_size) {
        for (int j = 0; j < matrix_size; j += block_size) {
            for (int bi = i; bi < i + block_size && bi < matrix_size; bi+=2) {
                for (int bj = j; bj < j + block_size && bj < matrix_size; bj+=2) {
                    dest[bj][bi] = source[bi][bj];
                    dest[bj][bi+1] = source[bi+1][bj];
                    dest[bj+1][bi] = source[bi][bj+1];
                    dest[bj+1][bi+1] = source[bi+1][bj+1];
                }
            }
        }
    }
}
