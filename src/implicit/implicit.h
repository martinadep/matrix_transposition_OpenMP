#ifndef IMPLICIT_H
#define IMPLICIT_H

// IMPLICIT.C
void block_based_transpose(float** source,float** dest, int matrix_size, int block_size);
double blockbased_timing(float **M, float **T, int matrix_size);

// BLOCK_SIZE.C
float blocksize_storing(float **M, float **T, int matrix_size, int block_size);
#endif
