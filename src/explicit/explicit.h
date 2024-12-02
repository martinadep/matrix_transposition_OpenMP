#ifndef EXPLICIT_H
#define EXPLICIT_H

void expTransposeBB(float** source, float** dest, int size, int num_threads);
void expTransposeBB_loop(float** source, float** dest, int size, int num_threads);
void expTransposeSeq(float** source, float** dest, int size, int num_threads);
double explicitTiming(float **source, float **dest, int size, int num_threads,int mode);

#endif
