#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "implicit.h"
#include "../main.h"
#include<omp.h>

/// compile this main and store the result in a .csv file if needed
int main() {
    srand(time(NULL));
    float dati_filtrati[LOOP];
    float dati[LOOP];
    float soglia = 2.0;

    for (int matrix_size = MIN; matrix_size <= MAX; matrix_size *= 2) {
        float **M = allocate_sqr_matrix(matrix_size);
        float **T = allocate_sqr_matrix(matrix_size);
        init_matrix(M, matrix_size);

        float time_sum = 0.0;
        for (int i = 0; i < LOOP; i++) {
            dati[i] = blockbasedStoring(M, T, matrix_size);
            time_sum += dati[i];
        }
        int count_filtrati = rimuovi_outliers(dati, dati_filtrati, LOOP, soglia);
        if (count_filtrati > 0) {
            double media_filtrata = calcola_media(dati_filtrati, count_filtrati);
            printf("%d,%.7f\n", matrix_size, media_filtrata);
        } else {
            printf("Tutti i valori considerati outliers in una riga\n");
        }
    }
    return 0;
}

/// Returns the elapsed time taken for the transposition.
float blockbasedStoring(float **M, float **T, int size) {
    double wt1= omp_get_wtime();
    int block_size = choose_block_size(size);
    blockBasedTranspose(M, T, size, block_size);
    double wt2 = omp_get_wtime();
    double elapsed = wt2 - wt1;
    return elapsed;
}

/// Block-based transpose
void blockBasedTranspose(float **source, float **dest, int size, int block_size) {
    for (int i = 0; i < size; i += block_size) {
        for (int j = 0; j < size; j += block_size) {
            for (int bi = i; bi < i + block_size && bi < size; bi+=2) {
                for (int bj = j; bj < j + block_size && bj < size; bj+=2) {
                    dest[bj][bi] = source[bi][bj];
                    dest[bj][bi+1] = source[bi+1][bj];
                    dest[bj+1][bi] = source[bi][bj+1];
                    dest[bj+1][bi+1] = source[bi+1][bj+1];
                }
            }
        }
    }
}

/*
// flag migliore 01 + march=native + funroll-loops
int main(int argc, char **argv) {
    srand(time(NULL));
    if (argc != 2) {
        printf("Usage: implicit_block_based <opt_flag_name>\n");
        return 1;
    }
    char *flag_name = argv[1];
    FILE *output = fopen("./blockbased_out/medie.csv", "a");
    if (output == NULL) {
        printf("Error opening medie.csv file\n");
        return 1;
    }

    float dati_filtrati[LOOP];
    float dati[LOOP];
    float soglia = 2.0;

    for (int matrix_size = MIN; matrix_size <= MAX; matrix_size *= 2) {
        float **M = allocate_sqr_matrix(matrix_size);
        float **T = allocate_sqr_matrix(matrix_size);
        init_matrix(M, matrix_size);

        float time_sum = 0.0;
        for (int i = 0; i < LOOP; i++) {
            dati[i] = blockbasedStoring(M, T, matrix_size, flag_name);
            time_sum += dati[i];
        }
        //printf("Block-based %s [%d x % d]:%.7fs\n", flag_name, matrix_size, matrix_size, time_sum/LOOP);

        int count_filtrati = rimuovi_outliers(dati, dati_filtrati, LOOP, soglia);
        if (count_filtrati > 0) {
            double media_filtrata = calcola_media(dati_filtrati, count_filtrati);
            printf("Media dei dati filtrati matrice [%d]x[%d]: %.7fs\n", matrix_size, matrix_size, media_filtrata);
            fprintf(output, "%d,%.7f,%s\n", matrix_size, media_filtrata, flag_name);
        } else {
            printf("Tutti i valori considerati outliers in una riga\n");
        }
    }
    fclose(output);
    printf("Medie calcolate e salvate in 'medie.csv'\n");
    return 0;
}

/// Block-based transposition given the compiler flag.
/// Returns the elapsed time taken for the transposition.
/// Stores the result in a text file
float blockbasedStoring(float **M, float **T, int size, char *flag) {
    // File settings to store results
    char filename[20];
    sprintf(filename, "./blockbased_out/%s_%dmatrix.txt", flag, size);
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    double wt1= omp_get_wtime();
    int block_size = choose_block_size(size);
    blockBasedTranspose(M, T, size, block_size);
    double wt2 = omp_get_wtime();
    double elapsed = wt2 - wt1;
    fprintf(file, "%f s - %s\n", elapsed, flag);
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

/// Block-based transpose
void blockBasedTranspose(float **source, float **dest, int size, int block_size) {
    for (int i = 0; i < size; i += block_size) {
        for (int j = 0; j < size; j += block_size) {
            for (int bi = i; bi < i + block_size && bi < size; bi+=2) {
                for (int bj = j; bj < j + block_size && bj < size; bj+=2) {
                    dest[bj][bi] = source[bi][bj];
                    dest[bj][bi+1] = source[bi+1][bj];
                    dest[bj+1][bi] = source[bi][bj+1];
                    dest[bj+1][bi+1] = source[bi+1][bj+1];
                }
            }
        }
    }
}*/