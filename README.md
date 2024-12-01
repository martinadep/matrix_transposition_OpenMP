# Introduction to Parallel Computing - First Deliverable
---

## Table of Contents

- [Introduction to Parallel Computing - First Deliverable](#introduction-to-parallel-computing---deliverable-1)
- [Table of Contents](#table-of-contents)
- [Project Structure](#project-structure)
- [How to Reproduce](#how-to-reproduce)
    - [On HPC Cluster](#on-hpc-cluster)
    - [On your local machine](#on-your-local-machine)
    - [Data Analysis](#data-analysis)

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
---
