#include <iostream>
#include <vector>
using namespace std;

#define V 5  // Number of vertices

// Function to check if it's safe to color vertex v with color c
bool isSafe(int v, bool graph[V][V], int color[], int c) {
    // Check if any adjacent vertex has the same color
    for (int i = 0; i < V; i++) {
        if (graph[v][i] && c == color[i]) {
            return false;
        }
    }
    return true;
}

// Recursive function to solve graph coloring problem
bool graphColoringUtil(bool graph[V][V], int m, int color[], int v) {
    // Base case: all vertices are colored
    if (v == V) {
        return true;
    }
    
    // Try different colors for vertex v
    for (int c = 1; c <= m; c++) {
        // Check if assignment of color c to v is safe
        if (isSafe(v, graph, color, c)) {
            color[v] = c;
            
            // Recursively assign colors to rest of vertices
            if (graphColoringUtil(graph, m, color, v + 1)) {
                return true;
            }
            
            // Backtrack if coloring doesn't lead to solution
            color[v] = 0;
        }
    }
    
    return false;
}

// Main function to solve graph coloring problem
bool graphColoring(bool graph[V][V], int m) {
    int color[V];
    
    // Initialize all vertices as uncolored
    for (int i = 0; i < V; i++) {
        color[i] = 0;
    }
    
    // Start coloring from vertex 0
    if (!graphColoringUtil(graph, m, color, 0)) {
        cout << "Solution does not exist with " << m << " colors" << endl;
        return false;
    }
    
    // Print the solution
    cout << "Solution exists with " << m << " colors:" << endl;
    for (int i = 0; i < V; i++) {
        cout << "Vertex " << i << " -> Color " << color[i] << endl;
    }
    
    return true;
}

// Print the graph
void printGraph(bool graph[V][V]) {
    cout << "\nGraph Adjacency Matrix:" << endl;
    cout << "   ";
    for (int i = 0; i < V; i++) {
        cout << i << " ";
    }
    cout << endl;
    
    for (int i = 0; i < V; i++) {
        cout << i << ": ";
        for (int j = 0; j < V; j++) {
            cout << graph[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    cout << "========================================" << endl;
    cout << "      Graph Coloring Problem           " << endl;
    cout << "========================================" << endl;
    
    // Example graph (5 vertices)
    // Graph represented as adjacency matrix
    bool graph[V][V] = {
        {0, 1, 1, 1, 0},
        {1, 0, 1, 0, 1},
        {1, 1, 0, 1, 1},
        {1, 0, 1, 0, 1},
        {0, 1, 1, 1, 0}
    };
    
    printGraph(graph);
    
    int m = 3;  // Number of colors
    cout << "\nTrying to color graph with " << m << " colors..." << endl;
    cout << "========================================" << endl;
    
    graphColoring(graph, m);
    
    return 0;
}
