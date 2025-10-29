#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define N 1000000000

int main() {
    long long circle_count = 0; // Main shared count
    
    double start_time = omp_get_wtime();

    // 'reduction(+:circle_count)' gives each thread a private 'circle_count'
    // and adds them all at the end.
    #pragma omp parallel reduction(+:circle_count)
    {
        unsigned int seed = omp_get_thread_num();
        
        // No 'for' needed here, 'parallel' on its own
        // But we need to split the work manually
        
        long long num_threads = omp_get_num_threads();
        long long my_tid = omp_get_thread_num();
        long long my_n = N / num_threads;
        if (my_tid == num_threads - 1) { // Last thread takes remainder
            my_n += N % num_threads;
        }

        // Each thread runs its own loop, adding to its private count
        for (long long i = 0; i < my_n; i++) {
            double x = (double)rand_r(&seed) / RAND_MAX;
            double y = (double)rand_r(&seed) / RAND_MAX;
            
            if (x * x + y * y <= 1.0) {
                circle_count++; // Adds to the private count
            }
        }
    } // OpenMP combines all private 'circle_count's here

    double end_time = omp_get_wtime();
    double pi = 4.0 * (double)circle_count / N;

    printf("\n--- Fix 3: 'reduction' (Best) ---\n");
    printf("Calculated pi = %f\n", pi);
    printf("Parallel Time = %f seconds\n", (end_time - start_time));
    
    return 0;
}
