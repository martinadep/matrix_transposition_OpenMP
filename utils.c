#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <sys/time.h>
#include "main.h"

int choose_block_size(int matrix_size) {
    if (matrix_size < 256) return 16; // [0, 256)
    if (matrix_size < 1024) return 64; // [512, 1024)
    return 256; // [1024, inf)
}
/// Dynamic allocation of a square [size] x [size] matrix
float **allocate_sqr_matrix(int size) {
    float **matrix = malloc(size * sizeof(float *));
    if (matrix == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);
    }
    for (int i = 0; i < size; i++) {
        matrix[i] = (float *) malloc(size * sizeof(float));
        if (matrix[i] == NULL) {
            fprintf(stderr, "Memory allocation failed!\n");
            exit(1);
        }
    }
    return matrix;
}

/// Dynamic deallocation of a square [size] x [size] matrix
void free_matrix(float **matrix, int size) {
    if (matrix != NULL) {
        for (int i = 0; i < size; i++) {
            free(matrix[i]);
        }
        free(matrix);
    }
}

/// Random floats initialization of a  square [size] x [size] matrix
void init_matrix(float **matrix, int size) {
    // Initialize the matrix with random values
    //printf("Initializing random %d x %d matrix...\n", size, size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = (float) rand() / (float) RAND_MAX;
        }
    }
    // printf("Initializaton done\n");
}
void print_matrix(float **matrix, int size) {
    for (int i = 0; i < size; i++) {
        printf("\t");
        for(int j = 0; j < size; j++)
            printf("%.2f ", matrix[i][j]);
        printf("\n");
    }
}

// ------ FUNZIONI PER ELIMINARE OUTLIERS E CALCOLARE MEDIE -------



float calcola_media(float arr[], int n) {
    float somma = 0.0;
    for (int i = 0; i < n; i++) {
        somma += arr[i];
    }
    return somma / n;
}

/// Funzione per calcolare la deviazione standard
float calcola_deviazione_standard(float arr[], int n, float media) {
    float somma_quadrata = 0.0;
    for (int i = 0; i < n; i++) {
        somma_quadrata += (arr[i] - media) * (arr[i] - media);
    }
    return sqrt(somma_quadrata / n);
}

/// Funzione per filtrare gli outliers basandosi su una soglia di deviazione standard
int rimuovi_outliers(float dati[], float dati_filtrati[], int dati_size, float soglia) {
    float media = calcola_media(dati, dati_size);
    float deviazione_standard = calcola_deviazione_standard(dati, dati_size, media);

    int indice_output = 0;
    for (int i = 0; i < dati_size; i++) {
        if (fabs(dati[i] - media) <= soglia * deviazione_standard) {
            dati_filtrati[indice_output++] = dati[i];
        }
    }
    return indice_output;
}
