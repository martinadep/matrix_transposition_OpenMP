#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include "implicit.h"
#include "../main.h"

//march=native è la flag migliore
int main(int argc, char **argv) {
    srand(time(NULL));
    FILE *output = fopen("./recursive_out/medie.csv", "a");
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
            dati[i] = recursiveStoring(M, T, matrix_size);
            time_sum += dati[i];
        }
        //printf("Recursive [%d x % d]:%.7fs\n", matrix_size, matrix_size, time_sum/LOOP);

        const int count_filtrati = rimuovi_outliers(dati, dati_filtrati, LOOP, soglia);

        if (count_filtrati > 0) {
            const double media_filtrata = calcola_media(dati_filtrati, count_filtrati);
            //printf("Media dei dati filtrati: %.7fs\n", media_filtrata);
            printf("%d,%.7f\n", matrix_size, media_filtrata);
        } else {
            printf("Tutti i valori considerati outliers in una riga\n");
        }
        printf("\n");
    }
    fclose(output);
    printf("Medie calcolate e salvate in 'medie.csv'\n");
    return 0;
}

/// transpose the matrix recursively given the flag and returns
/// the elapsed time taken for the transposition.
/// Stores the result in a text file
float recursiveStoring(float **M, float **T, int size) {
    // Wall clock time storing
    struct timeval start_tv, end_tv;
    gettimeofday(&start_tv, NULL); // Start time

    recursiveTranspose(M, T, 0, 0, size);
    gettimeofday(&end_tv, NULL); // End time
    long seconds_tv = end_tv.tv_sec - start_tv.tv_sec;
    long microseconds_tv = end_tv.tv_usec - start_tv.tv_usec;
    double elapsed = seconds_tv + microseconds_tv * 1e-6;
    return elapsed;
}

/// Recursive transpose
void recursiveTranspose(float **source, float **dest, int row_start, int col_start, int size) {
    // Base case: if the submatrix is 1x1, simply transpose the single element
    if (size == 1) {
        dest[col_start][row_start] = source[row_start][col_start];
        return;
    }
    // Recursive case: split the matrix into 4 submatrices
    int half_size = size / 2;
    recursiveTranspose(source, dest, row_start, col_start, half_size); // top-left
    recursiveTranspose(source, dest, row_start, col_start + half_size, half_size); // top-right
    recursiveTranspose(source, dest, row_start + half_size, col_start, half_size); // bottom-left
    recursiveTranspose(source, dest, row_start + half_size, col_start + half_size, half_size); // bottom-right
}
