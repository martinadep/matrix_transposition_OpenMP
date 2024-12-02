# Introduction to Parallel Computing - First Deliverable
---

## Table of Contents

- [Table of Contents](#table-of-contents)
- [Project Structure](#project-structure)
- [How to Reproduce](#how-to-reproduce)
    - [On HPC Cluster](#on-hpc-cluster)
    - [On your local machine](#on-your-local-machine)
---

# Project Structure

```bash
├── explicit
│   ├── explicit.c
│   ├── explicit.h
├── implicit
│   ├── block_size.c
│   ├── implicit.c
│   ├── implicit.h
├── test_functions
│   ├── test_functions.c
├── src
│   ├── main.c
│   ├── main.h
│   ├── utils.c
├── .gitignore
├── CMakeLists.txt
├── midterm.pbs
├── README.md
```
---

# How to Reproduce

## On HPC Cluster

### Prerequisites
- Access to the HPC cluster (in this case of the University of Trento)
- SSH client

### Steps
1. Open an ssh session on University of Trento's HPC cluster, in your preferred terminal
```bash
$ ssh <username>@hpc.unitn.it
```
2. Clone the repository
```bash
$ git clone https://github.com/martinadep/parallel_midterm
```
3. Navigate the the repository folder
```bash
$ cd parallel_midterm/
```
3. Run the following command:
```bash
$ qsub midterm.pbs
```
## Additional tests
This section shows how to run additional tests:
- (1) testing different block sizes for block-based matrix transposition
  - Block sizes are:
    - 32x32
    - 64x64
    - 128x128
    - 256x256
- (2) testing different flags for implicit matrix transposition
  - Tested flags are:
    - O0
    - O1
    - O2
    - O1 -march=native
    - O2 -march=native
- (3) testing different number of threads for omp optimized matrix transposition
    - Tests are performed for:
      - naive matrix transposition
      - block-based matrix transposition
      - block-based loop-unrolled matrix transposition
  
Run the following command:
```bash
$ qsub test_block_size.pbs
```
Run the following command:
```bash
$ qsub test_implicit_flags.pbs
```
Run the following command:
```bash
$ qsub test_omp.pbs
```

## On your local machine

### Prerequisites
- gcc compiler

### Steps
1. Clone the repository
```bash
$ git clone https://github.com/martinadep/parallel_midterm
```
2. Navigate the the repository folder
```bash
$ cd parallel_midterm/
```
5. Run the following commands:
```bash
$ gcc main.c utils.c -std=c11 -lm -fopenmp -o bin/matrix_transposition
$ export OMP_NUM_THREADS=<select_num_threads>; ./matrix_transposition <select_matrix_size>
```

## Additional tests
This section shows how to run additional tests: 
- (1) testing different block sizes for block-based matrix transposition
- (2) testing different flags for implicit matrix transposition
- (3) testing different number of threads for omp optimized matrix transposition
1. Block-size, run the following commands:
```bash
$ cd scripts/
$ ./test_block_size.sh
```
2. Implicit flags, run the following commands:
```bash
$ cd scripts/
$ ./test_implicit_flags.sh
```
3. Number of threads, run the following commands:
```bash
$ cd scripts/
$ ./test_omp.sh
```
---
