#ifndef MAIN_H
#define MAIN_H
#include <stdbool.h>

// N is power of 2, size of squared matrix
// evaluate from N=16 to N=4096
#define MIN 16
#define MAX 4096
#define LOOP 10

// MAIN.C
int checkSym(float** M, int n);
bool checkSymImp(float** M, int n);
int checkSymOMP(float** M, int n);

void matTranspose(float** M, float** T, int n);
void matTransposeImp(float** M, float** T, int n);
void matTransposeOMP(float** M, float** T, int n);

// UTILS.C
void free_matrix(float** matrix, int size);
float** allocate_sqr_matrix(int size);
void init_matrix(float** matrix, int size);
void print_matrix(float** matrix, int size);
int choose_block_size(int matrix_size);
int choose_num_threads(int matrix_size);

int remove_outliers(float data[], float filtered_data[], int data_size, float threshold);
float calculate_std_dev(float arr[], int array_size, float mean);
float calculate_mean(float arr[], int array_size);

// TEST_FUNCTIONS.C
bool test_functions();
#endif
