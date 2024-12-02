#include <stdbool.h>
#include <stdio.h>
#include <omp.h>
#include "main.h"

#include <stdlib.h>
#include <tgmath.h>
#include <time.h>

///
int main(int argc, char **argv) {
    srand(time(NULL));
    if (argc != 2) {
        printf("Usage: ./main matrix_size\n");
        return 1;
    }
    int n = atoi(argv[1]);
    int matrix_size = pow(2, n);
    float **M = allocate_sqr_matrix(matrix_size);
    float **T = allocate_sqr_matrix(matrix_size);
    init_matrix(M, matrix_size);
    /*
     * Results of the matrix transpositions can be printed:
     * note that for huge matrix it could be convenient to
     * print only a small block, like 5x5, to assess correctness
    */
    //int elem_to_print = 5;
    //print_matrix(M, elem_to_print);

    //------------------------sequential--------------------------
    if (checkSym(M, n)) {
        printf("Matrix is symmetric (seq), no need to transpose\n");
    } else {
        double wt1 = omp_get_wtime();
        matTranspose(M, T, n);
        double wt2 = omp_get_wtime();
        double elapsed = wt2 - wt1;
        printf("mat transpose: %f\n", elapsed);
        //print_matrix(T, elem_to_print);
    }

    //------------------------implicit----------------------------
    if (checkSymImp(M, n)) {
        printf("Matrix is symmetric (imp), no need to transpose\n");
    } else {
        double wt1 = omp_get_wtime();
        matTransposeImp(M, T, n);
        double wt2 = omp_get_wtime();
        double elapsed = wt2 - wt1;
        printf("mat transpose implicit: %f\n", elapsed);
        //print_matrix(T, elem_to_print);
    }

    //--------------------------OMP-------------------------------
    if (checkSymOMP(M, n)) {
        printf("Matrix is symmetric (omp), no need to transpose\n");
    } else {
        double wt1 = omp_get_wtime();
        matTransposeOMP(M, T, n);
        double wt2 = omp_get_wtime();
        double elapsed = wt2 - wt1;
        printf("mat transpose OMP: %f\n", elapsed);
        //print_matrix(T, elem_to_print);
    }

    /*
     * The correctness of the functions can be checked through
     * the "test_functions()": it exectutes all the functions
     * implemented in main.c, over two [4x4] matrices, one is
     * symmetric and one is asymmetric.
    */
    //test_functions();

    return 0;
}


/// Naive matrix transposition
void matTranspose(float **M, float **T, int n) {
    int size = pow(2, n);
#pragma omp parallel num_threads(1)
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            T[j][i] = M[i][j];
        }
    }
}

/// Naive check symmetry
int checkSym(float **M, int n) {
    int size = pow(2, n);
    int is_sym = 1; // assumed symmetric
#pragma omp parallel num_threads(1)
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (M[i][j] != M[j][i]) {
                is_sym = 0; // non-symmetric
            }
        }
    }
    return is_sym;
}

/// Parallel OMP matrix transposition
void matTransposeOMP(float **M, float **T, int n) {
    int size = pow(2, n);
    int block_size = choose_block_size(size);

    /*
     * If you want to use a different number of threads
     * using export OMP_NUM_THREADS
     * you must comment the next two lines of code,
     * otherwise 'export' it is overwritten
    */
    int num_thr = choose_num_threads(size);
    omp_set_num_threads(num_thr);

#pragma omp parallel for collapse(2)
    for (int i = 0; i < size; i += block_size) {
        for (int j = 0; j < size; j += block_size) {
            for (int bi = i; bi < i + block_size && bi < size; bi += 2) {
                for (int bj = j; bj < j + block_size && bj < size; bj += 2) {
                    T[bj][bi] = M[bi][bj];
                    T[bj][bi + 1] = M[bi + 1][bj];
                    T[bj + 1][bi] = M[bi][bj + 1];
                    T[bj + 1][bi + 1] = M[bi + 1][bj + 1];
                }
            }
        }
    }
}

/// Parallel OMP check symmetry
int checkSymOMP(float **M, int n) {
    int size = pow(2, n);
    int is_sym = 1; // assumed symmetric

#pragma omp parallel for collapse(2) reduction(&&:is_sym)
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (M[i][j] != M[j][i]) {
                is_sym = 0; // non-symmetric
            }
        }
    }

    return is_sym;
}

#pragma GCC optimize ("O1")
/// Implicit parallellized transposition, block-based pattern
void matTransposeImp(float **M, float **T, int n) {
    int size = pow(2, n);
    int block_size = choose_block_size(size);

#pragma omp parallel num_threads(1)
    for (int i = 0; i < size; i += block_size) {
        for (int j = 0; j < size; j += block_size) {
            for (int bi = i; bi < i + block_size && bi < size; bi += 2) {
                for (int bj = j; bj < j + block_size && bj < size; bj += 2) {
                    T[bj][bi] = M[bi][bj];
                    T[bj][bi + 1] = M[bi + 1][bj];
                    T[bj + 1][bi] = M[bi][bj + 1];
                    T[bj + 1][bi + 1] = M[bi + 1][bj + 1];
                }
            }
        }
    }
}
/// Implicit parallel check symmetry, block-based pattern
bool checkSymImp(float **M, int n) {
    int size = pow(2, n);
    bool is_sym = true;
    int block_size = choose_block_size(size);

#pragma omp parallel num_threads(1)
    for (int i = 0; i < size; i += block_size) {
        for (int j = 0; j < size; j += block_size) {
            for (int bi = i; bi < i + block_size && bi < size; bi++) {
                for (int bj = j; bj < j + block_size && bj < size; bj++) {
                    if (M[bi][bj] != M[bj][bi]) {
                        is_sym = false;
                    }
                }
            }
        }
    }
    return is_sym;
}
