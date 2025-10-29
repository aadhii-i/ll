#include <stdio.h>
#include <omp.h>

// print_array, swap, and partition functions are IDENTICAL
// to the serial version. (No need to repeat them).
void print_array(int arr[], int size) {
    for(int i=0; i<size; i++) printf("%d ", arr[i]);
    printf("\n");
}
void swap(int* a, int* b) { int t = *a; *a = *b; *b = t; }
int partition(int arr[], int low, int high) {
    int pivot = arr[high]; 
    int i = (low - 1);     
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++; 
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]); 
    return (i + 1);
}
// --- End of identical functions ---


// Parallel recursive quick_sort function
void quick_sort_parallel(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        // Create a task for the left half
        #pragma omp task
        quick_sort_parallel(arr, low, pi - 1);

        // Create a task for the right half
        #pragma omp task
        quick_sort_parallel(arr, pi + 1, high);
    }
}

// --- Main driver ---
int main() {
    int arr[] = {10, 55, 9, 4, 2, 7, 3, 5, 6, 1};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("--- Parallel Quick Sort (OpenMP) ---\n");
    printf("Original array:\n");
    print_array(arr, n);

    double start = omp_get_wtime();

    // Start the parallel region ONCE at the beginning.
    #pragma omp parallel
    {
        // Have only one thread (the master) kick off the sorting.
        #pragma omp single
        quick_sort_parallel(arr, 0, n - 1);
    } // All tasks must be finished before this parallel region ends.

    double end = omp_get_wtime();

    printf("\nSorted array:\n");
    print_array(arr, n);
    printf("Parallel Time: %f seconds\n", (end - start));

    return 0;
}
