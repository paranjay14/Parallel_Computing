# MA 513 Parallel Computing Assignments

This repository contains assignments done for MA 513 course.

---

## Common Terminology

1. **data**: This folder contains all the result outputs of the various iterations run on the code and naming of the files is according to the parameters set for that iteration.
2. **masterScript.sh**: Main Script to loop over various iterations & calling runScript.sh for results corres. to each iteration.
3. **runScript.sh**: Generates the overall mean-value execution times result by running the corres. code files as per the given iteration size.
4. **readFiles.py**: Reads the results from all the iteration-output files and prints in a matrix form so as to be directly used for generating graphs (maybe using Google Sheets or any such graph generator).
5. **Assignment-<X> Report.pdf**: This is the report containing the theory, descriptions and results of the various experiments performed in each assignment.


## Assignment 1

To Study the effect of cache. This folder contains the following files:

1. **naive.cpp, opt1.cpp, opt2_0.cpp, opt2_1.cpp, opt2_2.cpp, opt2_3.cpp, opt3.cpp,** and **theo_calc.cpp** are the various files used to compute outputs corres. to the - Naive Algo, Optimisation 1, Optimisation 2 (having 4 different optimizations within), Optimisation 3 and theoretical calculations respectively.

---

## Assignment 2

This assignment was focussed to get us familiar with the MPI syntax. It contains the following files:

1. **mpi_bsend.c** - (Buffered) Asynchronous blocking send and receive.
2. **mpi_isend.c** - (Immediate Return) Non-blocking send and receive.
3. **mpi_rsend.c** - (Ready) Blocking send and receive.
4. **mpi_send.c** - (Standard) send and receive.
5. **mpi_sendrecv.c** - (Concurrent) send and receive.
6. **mpi_ssend.c** - (Synchronous) Blocking send and receive.

---

## Assignment 3

To implement addition of numbers through various parallel methods and compare the results among those. This folder contains the following files:

1. **addNum_Naive.c** - Conatins code to add numbers where P0 first sends n/p items to each processor, then each PE adds n/p items, finally each PE sends partial sum to P0 which adds all these elements and return the sum.
2. **addNum_Recur.c** - Contains the code to add numbers where the distribution and collection of numbers is done hierarchially as discussed in class.
3. **addNum_Reduce.c** - Contains the code to add numbers using inbuilt MPI_reduce to compute partial sum.
4. **odd_even_sorting.c** - Contains the code for odd even sorting.

---

## Assignment 4

To implement matrix-vector multiplication using row division, column division and block division methods. This folder has the following files:

1. **mtrxVecMult_row.c** - Row division method.
2. **mtrxVecMult_col.c** - Column division method.
2. **mtrxVecMult_block.c** - Block division method.

---

## Assignment 5

To perform gaussian elimination on matrix and also to implement its pipelined version. It contains the following files:

1. **gau_elmn_cyclic.c** - Normal Gaussian Elimination.
2. **gau_elmn_pipe.c** - Pipelined Gaussian Elimination.

---

## openMp_Tut

Contains the mergesort code (**mergeSort.c**) using openMP written during lab.

---

## Assignment 6

It contains the following files having 2 parallel code versions for calculation of PI:

1. **pi_critical.c** - Using critical section for shared variable.
2. **pi_reduce.c** - Using reduce addition on the shared variable.

---

## Assignment 7

Solving the problem of multiplying two polynomials in parallel paradigm using 3 different methods:
1. School Method
2. Karatsuba's Algorithm
3. FFT Divide and Conquer Algorithm
It contains the following files:

1. **poly_mul_naive.c** - Parallelising School Method.
2. **poly_mul_krtsb.c** - Parallelising Karatsuba's algorithm.
3. **poly_mul_fft.c** - Parallelising FFT divide and conquer algorithm.

---

## Assignment 8

Parallelising producer consumer problem with multiple consumers and multiple producers and a single bounded queue.
It contains the following files:

1. **prod_cons.cpp** - Main code for producer consumer problem.
2. **conc_queue.cpp** - Helper code for Concurrent Queue.
3. **conc_queue.h** - Header file used for Concurrent Queue.

---

## Assignment 9

Parallelising the code of Word acceptance on the reduced DFA.
It contains the following files:

1. **dfa.c** - Code for the given problem.
2. **dfa_input<X>.txt** - Various different DFA input files.

---