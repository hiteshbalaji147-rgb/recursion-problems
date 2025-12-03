#include <iostream>
#include <vector>
using namespace std;

#define V 5  // Number of vertices

// Function to check if vertex v can be added to the path
bool isSafe(int v, bool graph[V][V], int path[], int pos) {
    // Check if this vertex is adjacent to the previously added vertex
    if (graph[path[pos - 1]][v] == 0) {
        return false;
    }
    
    // Check if the vertex has already been included in the path
    for (int i = 0; i < pos; i++) {
        if (path[i] == v) {
            return false;
        }
    }
    
    return true;
}

// Recursive function to find Hamiltonian path
bool hamiltonianPathUtil(bool graph[V][V], int path[], int pos) {
    // Base case: all vertices are included in the path
    if (pos == V) {
        return true;
    }
    
    // Try different vertices as the next candidate in path
    for (int v = 0; v < V; v++) {
        if (isSafe(v, graph, path, pos)) {
            path[pos] = v;
            
            // Recursively construct rest of the path
            if (hamiltonianPathUtil(graph, path, pos + 1)) {
                return true;
            }
            
            // Backtrack if adding vertex v doesn't lead to solution
            path[pos] = -1;
        }
    }
    
    return false;
}

// Main function to find Hamiltonian path
bool hamiltonianPath(bool graph[V][V]) {
    int path[V];
    
    // Initialize path array
    for (int i = 0; i < V; i++) {
        path[i] = -1;
    }
    
    // Start from vertex 0
    path[0] = 0;
    
    if (!hamiltonianPathUtil(graph, path, 1)) {
        cout << "No Hamiltonian path exists" << endl;
        return false;
    }
    
    // Print the solution
    cout << "Hamiltonian path found:" << endl;
    for (int i = 0; i < V; i++) {
        cout << path[i];
        if (i < V - 1) cout << " -> ";
    }
    cout << endl;
    
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
    cout << "    Hamiltonian Path Problem           " << endl;
    cout << "========================================" << endl;
    
    // Example graph (5 vertices)
    // Graph represented as adjacency matrix
    bool graph[V][V] = {
        {0, 1, 0, 1, 0},
        {1, 0, 1, 1, 1},
        {0, 1, 0, 0, 1},
        {1, 1, 0, 0, 1},
        {0, 1, 1, 1, 0}
    };
    
    printGraph(graph);
    
    cout << "\nSearching for Hamiltonian path..." << endl;
    cout << "========================================" << endl;
    
    hamiltonianPath(graph);
    
    return 0;
}
