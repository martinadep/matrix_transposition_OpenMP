#ifndef IMPLICIT_H
#define IMPLICIT_H

void blockBasedTranspose(float** source,float** dest, int size, int block_size);
void recursiveTranspose(float** source, float** dest, int row_start, int col_start, int size);

float blockbasedStoring(float **M, float **T, int size);
float blockbasedsizeStoring(float **M, float **T, int size, int block_size);
float recursiveStoring(float** M, float** T, int size);
#endif
