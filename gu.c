#include <stdio.h>
#include <stdlib.h>
#include <math.h> // For fabs()
#include <omp.h>  // For OpenMP and the timer

#define N 3 // Number of equations

// Helper function to swap two rows in the augmented matrix
void swap_rows(double A[N][N + 1], int row1, int row2) {
    for (int j = 0; j < N + 1; j++) {
        double temp = A[row1][j];
        A[row1][j] = A[row2][j];
        A[row2][j] = temp;
    }
}

// Helper function to print the final solution
void print_solution(double x[N]) {
    printf("Solution: (");
    for (int i = 0; i < N; i++) {
        // Print as fraction for clarity, matching the PDF
        // Note: This is a simple fraction guesser, not a full GCD.
        if (fabs(x[i] - (5.0/3.0)) < 1e-5) printf("5/3");
        else if (fabs(x[i] - (-5.0/6.0)) < 1e-5) printf("-5/6");
        else if (fabs(x[i] - (3.0/2.0)) < 1e-5) printf("3/2");
        else printf("%.3f", x[i]); // Fallback
        
        if (i < N - 1) printf(", ");
    }
    printf(")\n");
}

int main() {
    // Augmented matrix A|b for Set 1:
    // x - y + z = 4
    // x - 4y + 2z = 8
    // x + 2y + 8z = 12
    double A[N][N + 1] = {
        {1, -1, 1, 4},
        {1, -4, 2, 8},
        {1, 2, 8, 12}
    };
    
    double x[N]; // Solution vector
    int num_threads = 0;
    
    double start_time = omp_get_wtime();

    // --- 1. Forward Elimination (with Pivoting) ---
    for (int k = 0; k < N - 1; k++) {
        // --- Find Pivot (Serial) ---
        // Find the row with the largest absolute value in column k
        int max_row = k;
        double max_val = fabs(A[k][k]);
        for (int i = k + 1; i < N; i++) {
            if (fabs(A[i][k]) > max_val) {
                max_val = fabs(A[i][k]);
                max_row = i;
            }
        }
        
        // --- Swap Rows (Serial) ---
        if (max_row != k) {
            swap_rows(A, k, max_row);
        }

        // --- Parallel Elimination ---
        // This is the main parallel part.
        // We parallelize the 'i' loop, which iterates over rows.
        // Each thread takes one or more 'i' rows and processes them.
        
        #pragma omp parallel for
        for (int i = k + 1; i < N; i++) {
            // Get thread ID and total thread count (once)
            int tid = omp_get_thread_num();
            if (k == 0) { // Only capture this on the first pass
                #pragma omp single
                { num_threads = omp_get_num_threads(); }
            }

            // This printf shows which thread is doing which row
            printf("Thread %d is processing row %d (for pivot k=%d)\n", tid, i, k);

            double factor = A[i][k] / A[k][k];
            
            // This inner 'j' loop is run by the thread
            for (int j = k; j < N + 1; j++) {
                A[i][j] = A[i][j] - factor * A[k][j];
            }
        }
        // Implicit barrier: All threads wait here before next 'k'
    }

    // --- 2. Backward Substitution (Serial) ---
    // This part is very fast (O(N^2)) and sequential,
    // so it is not parallelized.
    for (int i = N - 1; i >= 0; i--) {
        double sum = 0;
        for (int j = i + 1; j < N; j++) {
            sum += A[i][j] * x[j];
        }
        x[i] = (A[i][N] - sum) / A[i][i];
    }

    double end_time = omp_get_wtime();

    // --- 3. Print Results ---
    printf("\n--- Parallel Version ---\n");
    if (num_threads == 0) num_threads = 1; // Handle N<2 case
    printf("Program ran with %d threads.\n", num_threads);
    print_solution(x);
    printf("Execution Time: %f seconds\n", (end_time - start_time));

    return 0;
}
