#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "implicit.h"
#include "../main.h"

/// Execute this main with different <block_size>.
/// It computes matrix transposition with a block-based approach.
/// You can store the result in a .csv file with the following format:
///
/// "matrix_size, mean, block_size"
int main(int argc, char **argv) {
    srand(time(NULL));
    if (argc != 2) {
        printf("Usage: implicit_block_size <block_size>\n");
        return 1;
    }
    int block_size = atoi(argv[1]);

    float filtered_data[LOOP];
    float data[LOOP];
    float thrsd = 2.0;

    for (int matrix_size = MIN; matrix_size <= MAX; matrix_size *= 2) {
        if (matrix_size > block_size) {
            float **M = allocate_sqr_matrix(matrix_size);
            float **T = allocate_sqr_matrix(matrix_size);
            init_matrix(M, matrix_size);

            float time_sum = 0.0;
            for (int i = 0; i < LOOP; i++) {
                data[i] = blocksize_storing(M, T, matrix_size, block_size);
                time_sum += data[i];
            }

            int count_filtered = remove_outliers(data, filtered_data, LOOP, thrsd);
            if (count_filtered > 0) {
                double filtered_data_mean = calculate_mean(filtered_data, count_filtered);
                // READABLE FORMAT
                //printf("[%dx%d] mean: %.7f\n", matrix_size, matrix_size, filtered_data_mean);

                // .CSV FORMAT
                printf("%d,%.7f,%d\n", matrix_size, filtered_data_mean, block_size);
            } else {
                printf("All values considered outliers in a row\n");
            }
        }
    }
    return 0;
}

/// Block-based transposition given the block size.
/// Returns the elapsed time taken for the transposition.
/// Stores the result in a text file
///
/// "./blockbasedsize_out/<B>block_<N>matrix.txt"
///
/// where B is size of block and N is size of matrix
float blocksize_storing(float **M, float **T, int matrix_size, int block_size) {
    if (matrix_size <= block_size) {
        return 0.0;
    }
    // File settings to store results
    char filename[20];
    sprintf(filename, "./blockbasedsize_out/%dblock_%dmatrix.txt", block_size, matrix_size);
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    // Wall clock time storing
    double wt1= omp_get_wtime();

    block_based_transpose(M, T, matrix_size, block_size);

    double wt2 = omp_get_wtime();
    double elapsed = wt2 - wt1;
    fprintf(file, "%f s - %d block\n", elapsed, block_size);
    fclose(file);
    return elapsed;
}

/// Block-based transposition
void block_based_transpose(float **source, float **dest, int matrix_size, int block_size) {
    for (int i = 0; i < matrix_size; i += block_size) {
        for (int j = 0; j < matrix_size; j += block_size) {
            for (int bi = i; bi < i + block_size && bi < matrix_size; bi++) {
                for (int bj = j; bj < j + block_size && bj < matrix_size; bj++) {
                    dest[bj][bi] = source[bi][bj];
                }
            }
        }
    }
}
