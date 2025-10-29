#include <stdio.h>
#include <omp.h>

static long num_steps = 10000000;
double step;

int main () {
    double sum = 0.0;
    double pi;
    
    step = 1.0 / (double)num_steps;
    
    double start_time = omp_get_wtime();

    // The 'reduction(+:sum)' clause fixes the race condition.
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < num_steps; i++) {
        double x = (i + 0.5) * step;
        // Each thread adds to its own private 'sum'
        sum = sum + 4.0 / (1.0 + x*x);
    }
    // OpenMP combines all private 'sum's here
    
    pi = step * sum;
    double end_time = omp_get_wtime();
    double fixed_time = end_time - start_time;

    printf("\n--- Parallel Version (Corrected with reduction) ---\n");
    printf("Calculated pi = %f (This is CORRECT)\n", pi);
    printf("Corrected Time= %f seconds\n", fixed_time);
    
    return 0;
}
