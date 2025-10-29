#include <stdio.h>
#include <omp.h> // For OpenMP and the timer

// Helper function to print an array
void print_array(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// Helper function to swap two elements
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main() {
    // Test Case: N=10 Unsorted Array
    int arr[] = {10, 55, 9, 4, 2, 7, 3, 5, 6, 1};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("--- Parallel Odd-Even Sort (in C) ---\n");
    printf("Original array:\n");
    print_array(arr, n);

    double start_time = omp_get_wtime();

    // The outer 'phase' loop must be serial.
    for (int phase = 0; phase < n; phase++) {
        
        if (phase % 2 == 0) {
            // --- Parallel Even Phase ---
            // Parallelize the 'for' loop for even indices
            #pragma omp parallel for
            for (int i = 0; i < n - 1; i += 2) {
                if (arr[i] > arr[i + 1]) {
                    swap(&arr[i], &arr[i + 1]);
                }
            }
        } else {
            // --- Parallel Odd Phase ---
            // Parallelize the 'for' loop for odd indices
            #pragma omp parallel for
            for (int i = 1; i < n - 1; i += 2) {
                if (arr[i] > arr[i + 1]) {
                    swap(&arr[i], &arr[i + 1]);
                }
            }
        }
    }

    double end_time = omp_get_wtime();

    printf("\nSorted array:\n");
    print_array(arr, n);
    printf("Parallel Time: %f seconds\n", (end_time - start_time));

    return 0;
}
