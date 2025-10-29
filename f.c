#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// V = Number of vertices
#define V 4
// INF = A large value (infinity)
#define INF 99999

// Helper function to print the solution matrix
void print_matrix(int dist[V][V]) {
    printf("Shortest path matrix (All-Pairs):\n");
    printf("      A    B    C    E\n");
    printf("   -------------------\n");
    for (int i = 0; i < V; i++) {
        printf(" %c | ", 'A' + i);
        for (int j = 0; j < V; j++) {
            if (dist[i][j] == INF) {
                printf("%4s ", "INF");
            } else {
                printf("%4d ", dist[i][j]);
            }
        }
        printf("\n");
    }
}

int main() {
    // dist[V][V] will be our solution matrix
    // Test Case 1: (A=0, B=1, C=2, E=3)
    int dist[V][V] = {
      //A    B    C    E
        {0,   INF, 3,   9},   // A
        {5,   0,   1,   INF}, // B
        {INF, INF, 0,   3},   // C
        {INF, 2,   INF, 0}    // E
    };

    double start_time = omp_get_wtime();

    // Loop 'k' is the intermediate vertex
    // This loop MUST be serial
    for (int k = 0; k < V; k++) {
        
        // Parallelize the 'i' and 'j' loops.
        // 'collapse(2)' turns the 2D loop (i*j) into a 1D work list.
        #pragma omp parallel for collapse(2)
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                
                // Read values (safe in parallel for a given k)
                int dist_ik = dist[i][k];
                int dist_kj = dist[k][j];
                
                // Check if the path through 'k' is shorter
                if (dist_ik != INF && dist_kj != INF && 
                    dist_ik + dist_kj < dist[i][j]) {
                    
                    // Write to dist[i][j]
                    // This is safe because each thread has a unique (i, j) pair
                    dist[i][j] = dist_ik + dist_kj;
                }
            }
        }
        // Implicit barrier: all threads wait here before starting the next 'k'
    }

    double end_time = omp_get_wtime();

    printf("--- Parallel Floyd-Warshall (Test Case 1) ---\n");
    print_matrix(dist);
    printf("\nExecution Time: %f seconds\n", (end_time - start_time));

    return 0;
}
