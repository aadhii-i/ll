#include <stdio.h>
#include <omp.h>
static long num_steps = 100000000;

int main () {
    double sum = 0.0;
    double step = 1.0 / (double)num_steps;
    double start_time = omp_get_wtime();

    // The 'reduction(+:sum)' gives each thread a private 'sum'
    // and adds them all at the end.
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < num_steps; i++) {
        double x = (i + 0.5) * step;
        sum = sum + 4.0 / (1.0 + x*x);
    }
    
    double pi = step * sum;
    double end_time = omp_get_wtime();
    printf("\n--- Fix 3: 'reduction' ---\n");
    printf("Calculated pi = %f\n", pi);
    printf("Time = %f seconds (Correct and FASTEST)\n", (end_time - start_time));
    return 0;
}
