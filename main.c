#include <stdbool.h>
#include <stdio.h>
#include <omp.h>
#include "main.h"

#include <stdlib.h>
#include <tgmath.h>
#include <time.h>

int main(int argc, char **argv) {
    srand(time(NULL));
    if (argc != 2) {
        printf("Usage: ./main matrix_size\n");
        return 1;
    }
    int n = atoi(argv[1]);
    float** M = allocate_sqr_matrix(pow(2,n));
    float** T = allocate_sqr_matrix(pow(2,n));
    init_matrix(M, pow(2,n));
    //print_matrix(M, pow(2,n));

    //----sequential----
    double wt1= omp_get_wtime();
    matTranspose(M, T, n);
    double wt2 = omp_get_wtime();
    double elapsed = wt2 - wt1;
    printf("mat transpose: %f\n", elapsed);
    //print_matrix(T, n);

    //----implicit----
    wt1= omp_get_wtime();
    matTransposeImp(M, T , n);
    wt2 = omp_get_wtime();
    elapsed = wt2 - wt1;
    printf("mat transpose implicit: %f\n", elapsed);
    //print_matrix(T, n);

    //----OMP----
    wt1= omp_get_wtime();
    matTransposeOMP(M, T , n);
    wt2 = omp_get_wtime();
    elapsed = wt2 - wt1;
    printf("mat transpose openMP: %f\n", elapsed);
    //print_matrix(T, n);

    //test_functions();

    return 0;
}


/// Naive matrix transposition approach
void matTranspose(float** M, float** T, int n) {
    int size = pow(2, n);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            T[j][i] = M[i][j];
        }
    }
}

void matTransposeOMP(float** M, float** T, int n) {
    int size = pow(2, n);
    int block_size = choose_block_size(size);
#pragma omp num_threads(32) parallel for collapse(2)
    for (int i = 0; i < size; i += block_size) {
        for (int j = 0; j < size; j += block_size) {
            for (int bi = i; bi < i + block_size && bi < size; bi+=2) {
                for (int bj = j; bj < j + block_size && bj < size; bj+=2) {
                    T[bj][bi] = M[bi][bj];
                    T[bj][bi+1] = M[bi+1][bj];
                    T[bj+1][bi] = M[bi][bj+1];
                    T[bj+1][bi+1] = M[bi+1][bj+1];
                }
            }
        }
    }
}

bool checkSym(float** M, int n) {
    int size = pow(2, n);
    bool is_sym = true;
    for (int i = 0; i < size / 2; i++) {
        for (int j = 0; j < size / 2; j++) {
            if (M[j][i] != M[i][j]) {
                is_sym = false;
            }
        }
    }
    return is_sym;
}

bool checkSymOMP(float** M, int n) {
    int size = pow(2, n);
    bool is_sym = true;
#pragma omp num_threads(32) parallel for collapse(2)
    for (int i = 0; i < size / 2; i++) {
        for (int j = 0; j < size / 2; j++) {
            if (M[j][i] != M[i][j]) {
                is_sym = false;
            }
        }
    }
    return is_sym;
}

#pragma GCC optimize ("O1")
/// Implicit parallelization, block-based transposition
void matTransposeImp(float** M, float** T, int n) {
    int size = pow(2, n);
    int block_size = choose_block_size(size);
    for (int i = 0; i < size; i += block_size) {
        for (int j = 0; j < size; j += block_size) {
            for (int bi = i; bi < i + block_size && bi < size; bi+=2) {
                for (int bj = j; bj < j + block_size && bj < size; bj+=2) {
                    T[bj][bi] = M[bi][bj];
                    T[bj][bi+1] = M[bi+1][bj];
                    T[bj+1][bi] = M[bi][bj+1];
                    T[bj+1][bi+1] = M[bi+1][bj+1];
                }
            }
        }
    }
}
bool checkSymImp(float** M, int n) {
    int size = pow(2, n);
    bool is_sym = true;
    int block_size = choose_block_size(size);
    for (int i = 0; i < size; i += block_size) {
        for (int j = 0; j < size; j += block_size) {
            for (int bi = i; bi < i + block_size && bi < size; bi++) {
                for (int bj = j; bj < j + block_size && bj < size; bj++) {
                    if (M[bj][bi] != M[bi][bj]) {
                        is_sym = false;
                    }
                }
            }
        }
    }
    return is_sym;
}
