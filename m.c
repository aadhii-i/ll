#include <stdio.h>
#include <stdlib.h>
#include <omp.h> // For OpenMP and the timer

#define N 3 

void print_matrix(int matrix[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
}

int A[N][N];
int B[N][N];
int C[N][N];

int main() {
    // --- 1. Initialize Matrices (Test Case 1) ---
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = (i + 1) * (j + 1); 
            B[i][j] = (i + j + 1);       
        }
    }
    
    // --- 2. Parallel Computation ---
    printf("\n--- Parallel Run (N=%d) ---\n", N);
    double start_time = omp_get_wtime(); // Start timer

    // Parallelize the outer loop (i)
    // Each thread will take a "chunk" of rows to compute.
    // Variables i, j, k, and sum are private to each thread by default.
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        // printf("Thread %d working on row %d\n", omp_get_thread_num(), i);
        for (int j = 0; j < N; j++) {
            int sum = 0; // Local sum for each C[i][j]
            for (int k = 0; k < N; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }

    double end_time = omp_get_wtime(); // Stop timer
    double parallel_time = end_time - start_time;
    
    printf("Result Matrix C:\n");
    print_matrix(C);
    printf("Parallel Execution Time: %f seconds\n", parallel_time);
    
    return 0;
}
