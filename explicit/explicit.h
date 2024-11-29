#ifndef EXPLICIT_H
#define EXPLICIT_H

void explicitTransposeBlock(float** source, float** dest, int size, int num_threads);
void explicitTransposeLoop(float** source, float** dest, int size, int num_threads);
void explicitTransposeSeq(float** source, float** dest, int size, int num_threads);
float explicitStoring(float **source, float **dest, int size, int num_threads,int mode);

#endif
