#ifndef MAIN_H
#define MAIN_H
#include <stdbool.h>

//N is power of 2, size of squared matrix
//evaluate from N=16 to N=4096
#define MIN 256
#define MAX 4096
#define LOOP 10

// MAIN.C
bool checkSym(float** M, int size);
bool checkSymImp(float** M, int size);
bool checkSymOMP(float** M, int size);

void matTranspose(float** M, float** T, int size);
void matTransposeImplicit(float** M, float** T, int size);// __attribute__((optimize("O1, march=native, funroll-loops")));
void matTransposeImp(float** M, float** T, int size);
void matTransposeOMP(float** M, float** T, int size);

// UTILS.C
void free_matrix(float** matrix, int size);
float** allocate_sqr_matrix(int size);
void init_matrix(float** matrix, int size);
void print_matrix(float** matrix, int size);
int choose_block_size(int matrix_size);

// CALCOLA_MEDIE.C
int rimuovi_outliers(float dati[], float dati_filtrati[], int dati_size, float soglia);
float calcola_deviazione_standard(float arr[], int n, float media);
float calcola_media(float arr[], int n);

// TEST_FUNCTIONS.C
bool test_functions();
#endif
