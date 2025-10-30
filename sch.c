#include <stdio.h>
#include <omp.h>

// Using a large N to make the loop take time
#define N 100000000

int main() {
    long long sum = 0;
    
    double start_time = omp_get_wtime();

    for (int i = 1; i <= N; i++) {
        sum += (long long)i * i;
    }

    double end_time = omp_get_wtime();
    double serial_time = end_time - start_time;

    printf("--- Serial Version ---\n");
    printf("Sum of squares (1 to %d) = %lld\n", N, sum);
    printf("Serial Time = %f seconds\n", serial_time);

    return 0;
}
