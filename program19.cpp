#include <iostream>
#include <vector>
#include <chrono>
using namespace std;
using namespace chrono;

#define MAX_V 10  // Maximum number of vertices

int numVertices;  // Actual number of vertices

// Statistics structure
struct Stats {
    int recursionCalls = 0;
    int backtrackCount = 0;
    int pathsExplored = 0;
    long long executionTime = 0;
};

Stats stats;

// Check if vertex v can be added to the Hamiltonian path
bool isSafe(int v, bool graph[MAX_V][MAX_V], int path[], int pos) {
    // Check if this vertex is adjacent to the previously added vertex
    if (!graph[path[pos - 1]][v]) {
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
bool hamiltonianPathUtil(bool graph[MAX_V][MAX_V], int path[], int pos) {
    stats.recursionCalls++;
    stats.pathsExplored++;
    
    // Base case: all vertices are included in the path
    if (pos == numVertices) {
        return true;
    }
    
    // Try different vertices as next candidate in Hamiltonian path
    for (int v = 0; v < numVertices; v++) {
        if (isSafe(v, graph, path, pos)) {
            path[pos] = v;
            
            // Recursively construct rest of the path
            if (hamiltonianPathUtil(graph, path, pos + 1)) {
                return true;
            }
            
            // Backtrack if adding vertex v doesn't lead to solution
            path[pos] = -1;
            stats.backtrackCount++;
        }
    }
    
    return false;
}

// Main function to find Hamiltonian path
bool hamiltonianPath(bool graph[MAX_V][MAX_V], int path[]) {
    // Initialize path array
    for (int i = 0; i < numVertices; i++) {
        path[i] = -1;
    }
    
    // Try starting from each vertex
    for (int start = 0; start < numVertices; start++) {
        path[0] = start;
        
        if (hamiltonianPathUtil(graph, path, 1)) {
            return true;
        }
        
        // Reset for next starting vertex
        path[0] = -1;
    }
    
    return false;
}

// Print the graph
void printGraph(bool graph[MAX_V][MAX_V]) {
    cout << "\nGraph Adjacency Matrix:" << endl;
    cout << "   ";
    for (int i = 0; i < numVertices; i++) {
        cout << i << " ";
    }
    cout << endl;
    
    for (int i = 0; i < numVertices; i++) {
        cout << i << ": ";
        for (int j = 0; j < numVertices; j++) {
            cout << graph[i][j] << " ";
        }
        cout << endl;
    }
}

// Print graph as edge list
void printEdges(bool graph[MAX_V][MAX_V]) {
    cout << "\nEdge List:" << endl;
    int edgeCount = 0;
    for (int i = 0; i < numVertices; i++) {
        for (int j = i + 1; j < numVertices; j++) {
            if (graph[i][j]) {
                cout << "Edge " << ++edgeCount << ": " << i << " -- " << j << endl;
            }
        }
    }
    cout << "Total edges: " << edgeCount << endl;
}

// Print Hamiltonian path
void printPath(int path[]) {
    cout << "\nHamiltonian Path:" << endl;
    for (int i = 0; i < numVertices; i++) {
        cout << path[i];
        if (i < numVertices - 1) {
            cout << " -> ";
        }
    }
    cout << endl;
}

// Print statistics
void printStats() {
    cout << "\n========================================" << endl;
    cout << "           Statistics                  " << endl;
    cout << "========================================" << endl;
    cout << "Recursion calls: " << stats.recursionCalls << endl;
    cout << "Paths explored: " << stats.pathsExplored << endl;
    cout << "Backtrack operations: " << stats.backtrackCount << endl;
    cout << "Execution time: " << stats.executionTime << " μs" << endl;
    cout << "========================================" << endl;
}

int main() {
    cout << "========================================" << endl;
    cout << "     Hamiltonian Path Problem          " << endl;
    cout << "========================================" << endl;
    
    // Test graph 1: Graph with Hamiltonian path
    bool graph1[MAX_V][MAX_V] = {
        {0, 1, 0, 1, 0},
        {1, 0, 1, 1, 1},
        {0, 1, 0, 0, 1},
        {1, 1, 0, 0, 1},
        {0, 1, 1, 1, 0}
    };
    
    // Test graph 2: Complete graph K4 (has Hamiltonian path)
    bool graph2[MAX_V][MAX_V] = {
        {0, 1, 1, 1},
        {1, 0, 1, 1},
        {1, 1, 0, 1},
        {1, 1, 1, 0}
    };
    
    // Test graph 3: Graph without Hamiltonian path
    bool graph3[MAX_V][MAX_V] = {
        {0, 1, 0, 0},
        {1, 0, 1, 0},
        {0, 1, 0, 1},
        {0, 0, 1, 0}
    };
    
    int choice;
    cout << "\nSelect test graph:" << endl;
    cout << "1. Graph 1 (5 vertices)" << endl;
    cout << "2. Complete graph K4 (4 vertices)" << endl;
    cout << "3. Graph without Hamiltonian path (4 vertices)" << endl;
    cout << "Enter choice (1-3): ";
    cin >> choice;
    
    bool graph[MAX_V][MAX_V];
    
    if (choice == 1) {
        numVertices = 5;
        for (int i = 0; i < numVertices; i++) {
            for (int j = 0; j < numVertices; j++) {
                graph[i][j] = graph1[i][j];
            }
        }
        cout << "\n========================================" << endl;
        cout << "    Test Graph 1                       " << endl;
        cout << "========================================" << endl;
    } else if (choice == 2) {
        numVertices = 4;
        for (int i = 0; i < numVertices; i++) {
            for (int j = 0; j < numVertices; j++) {
                graph[i][j] = graph2[i][j];
            }
        }
        cout << "\n========================================" << endl;
        cout << "    Complete Graph K4                  " << endl;
        cout << "========================================" << endl;
    } else if (choice == 3) {
        numVertices = 4;
        for (int i = 0; i < numVertices; i++) {
            for (int j = 0; j < numVertices; j++) {
                graph[i][j] = graph3[i][j];
            }
        }
        cout << "\n========================================" << endl;
        cout << "    Graph without Hamiltonian Path    " << endl;
        cout << "========================================" << endl;
    } else {
        cout << "Invalid choice!" << endl;
        return 1;
    }
    
    printGraph(graph);
    printEdges(graph);
    
    int path[MAX_V];
    
    cout << "\n========================================" << endl;
    cout << "  Finding Hamiltonian Path             " << endl;
    cout << "========================================" << endl;
    
    stats = Stats();
    auto start = high_resolution_clock::now();
    
    bool found = hamiltonianPath(graph, path);
    
    auto end = high_resolution_clock::now();
    stats.executionTime = duration_cast<microseconds>(end - start).count();
    
    if (found) {
        cout << "\n✓ Hamiltonian path found!" << endl;
        printPath(path);
    } else {
        cout << "\n✗ No Hamiltonian path exists" << endl;
    }
    
    printStats();
    
    return 0;
}
