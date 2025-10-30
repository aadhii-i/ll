#include <stdio.h>
#include <stdlib.h>
#include <omp.h> // Include OpenMP

// --- Serial Merge Function (Helper) ---
// Merges two sorted subarrays arr[l...m] and arr[m+1...r]
void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    // Create temp arrays
    int* L = (int*)malloc(n1 * sizeof(int));
    int* R = (int*)malloc(n2 * sizeof(int));

    // Copy data to temp arrays L[] and R[]
    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    // Merge the temp arrays back into arr[l...r]
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy remaining elements of L[]
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    // Copy remaining elements of R[]
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
    
    free(L);
    free(R);
}

// --- Parallel Merge Sort Function ---
void parallel_merge_sort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        // Create a task to sort the left half
        #pragma omp task
        parallel_merge_sort(arr, l, m);

        // Create a task to sort the right half
        #pragma omp task
        parallel_merge_sort(arr, m + 1, r);

        // Wait for both tasks to finish
        #pragma omp taskwait

        // Merge the two sorted halves
        merge(arr, l, m, r);
    }
}

// Helper function to print an array
void print_array(int arr[], int size) {
    for (int i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

// --- Main Driver ---
int main() {
    int arr[] = {10, 55, 9, 4, 2, 7, 3, 5, 6, 1};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Original array:\n");
    print_array(arr, n);

    double start = omp_get_wtime();

    // Start the parallel region ONCE at the beginning
    #pragma omp parallel
    {
        // Have only ONE thread (the master) kick off the sorting
        #pragma omp single
        parallel_merge_sort(arr, 0, n - 1);
    } // All tasks are guaranteed to be finished by this point

    double end = omp_get_wtime();

    printf("\nSorted array:\n");
    print_array(arr, n);

    printf("\nParallel Execution Time: %f seconds\n", (end - start));

    return 0;
}
