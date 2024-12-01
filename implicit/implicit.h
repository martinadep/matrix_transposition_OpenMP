#ifndef IMPLICIT_H
#define IMPLICIT_H

// IMPLICIT.C
void block_based_transpose(float** source,float** dest, int matrix_size, int block_size);
double blockbasedTiming(float **M, float **T, int matrix_size);

// BLOCK_SIZE.C
float blockbasedsizeStoring(float **M, float **T, int matrix_size, int block_size);
#endif
