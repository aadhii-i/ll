#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <limits.h> // For INT_MAX

#define V 6 // Number of vertices (A, B, C, D, E, F)
#define INF INT_MAX

// Helper function to find the unvisited node with the minimum distance
// This part is serial
int find_min_dist(int dist[], int visited[]) {
    int min_val = INF;
    int min_index = -1;

    for (int v = 0; v < V; v++) {
        if (visited[v] == 0 && dist[v] <= min_val) {
            min_val = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

// Helper function to print the final distances
void print_solution(int dist[], int src) {
    printf("Vertex \t\t Distance from Source '%c'\n", src + 'A');
    for (int i = 0; i < V; i++) {
        if (dist[i] == INF) {
            printf("%c \t\t %s\n", i + 'A', "INF");
        } else {
            printf("%c \t\t %d\n", i + 'A', dist[i]);
        }
    }
}

int main() {
    // Test Case 1 Graph (A=0, B=1, C=2, D=3, E=4, F=5)
    // 0 means no direct edge
    int graph[V][V] = {
      //A  B  C  D   E   F
        {0, 1, 2, 0,  0,  0}, // A
        {0, 0, 0, 7,  0,  0}, // B
        {0, 0, 0, 0,  5, 12}, // C
        {0, 0, 0, 0,  0,  4}, // D
        {0, 0, 0, 10, 0,  9}, // E
        {0, 0, 0, 0,  0,  0}  // F
    };
    
    int dist[V];    // Final distance from source
    int visited[V]; // 1 if visited, 0 if not
    int source = 0; // Source node 'A'

    // --- 1. Initialize ---
    for (int i = 0; i < V; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }
    dist[source] = 0;

    double start_time = omp_get_wtime();

    // --- 2. Main Loop (Serial) ---
    // We loop V times (once for each node)
    for (int count = 0; count < V; count++) {
        
        // --- 3. Find Closest Node (Serial) ---
        int u = find_min_dist(dist, visited);
        
        if (u == -1) {
            break; // No more reachable nodes
        }
        
        // --- 4. Mark Node ---
        visited[u] = 1;

        // --- 5. Update Neighbors (Parallel) ---
        // This 'v' loop is parallelized
        #pragma omp parallel for
        for (int v = 0; v < V; v++) {
            int weight = graph[u][v];
            
            // Check: 1. Is there an edge?
            //        2. Is the neighbor unvisited?
            //        3. Is this new path shorter?
            if (weight > 0 && visited[v] == 0) {
                int new_dist = dist[u] + weight;
                if (new_dist < dist[v]) {
                    // This update is safe because each thread 'v'
                    // is writing to a different 'dist[v]'
                    dist[v] = new_dist;
                }
            }
        }
    }
    
    double end_time = omp_get_wtime();

    printf("--- Parallel Dijkstra's (Test Case 1) ---\n");
    print_solution(dist, source);
    printf("\nExecution Time: %f seconds\n", (end_time - start_time));

    return 0;
}
