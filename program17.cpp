#include <iostream>
#include <vector>
using namespace std;

#define MAX_V 10  // Maximum number of vertices

int numVertices;  // Actual number of vertices

// Function to check if vertex v can be added to the path
bool isSafe(int v, bool graph[MAX_V][MAX_V], int path[], int pos) {
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
bool hamiltonianPathUtil(bool graph[MAX_V][MAX_V], int path[], int pos) {
    // Base case: all vertices are included in the path
    if (pos == numVertices) {
        return true;
    }
    
    // Try different vertices as the next candidate in path
    for (int v = 0; v < numVertices; v++) {
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

// Recursive function to find Hamiltonian cycle
bool hamiltonianCycleUtil(bool graph[MAX_V][MAX_V], int path[], int pos) {
    // Base case: all vertices are included in the path
    if (pos == numVertices) {
        // Check if there is an edge from the last vertex to the first vertex
        if (graph[path[pos - 1]][path[0]] == 1) {
            return true;
        }
        return false;
    }
    
    // Try different vertices as the next candidate in path
    for (int v = 1; v < numVertices; v++) {  // Start from 1 since 0 is already in path
        if (isSafe(v, graph, path, pos)) {
            path[pos] = v;
            
            // Recursively construct rest of the path
            if (hamiltonianCycleUtil(graph, path, pos + 1)) {
                return true;
            }
            
            // Backtrack if adding vertex v doesn't lead to solution
            path[pos] = -1;
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
    
    // Start from vertex 0
    path[0] = 0;
    
    if (!hamiltonianPathUtil(graph, path, 1)) {
        return false;
    }
    
    return true;
}

// Main function to find Hamiltonian cycle
bool hamiltonianCycle(bool graph[MAX_V][MAX_V], int path[]) {
    // Initialize path array
    for (int i = 0; i < numVertices; i++) {
        path[i] = -1;
    }
    
    // Start from vertex 0
    path[0] = 0;
    
    if (!hamiltonianCycleUtil(graph, path, 1)) {
        return false;
    }
    
    return true;
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

// Print path
void printPath(int path[], bool isCycle = false) {
    for (int i = 0; i < numVertices; i++) {
        cout << path[i];
        if (i < numVertices - 1) cout << " -> ";
    }
    if (isCycle) {
        cout << " -> " << path[0];  // Show cycle back to start
    }
    cout << endl;
}

int main() {
    cout << "========================================" << endl;
    cout << "  Hamiltonian Path & Cycle Problem     " << endl;
    cout << "========================================" << endl;
    
    // Test graphs
    bool graph1[MAX_V][MAX_V] = {
        {0, 1, 0, 1, 0},
        {1, 0, 1, 1, 1},
        {0, 1, 0, 0, 1},
        {1, 1, 0, 0, 1},
        {0, 1, 1, 1, 0}
    };
    
    bool graph2[MAX_V][MAX_V] = {
        {0, 1, 1, 1},
        {1, 0, 1, 1},
        {1, 1, 0, 1},
        {1, 1, 1, 0}
    };
    
    bool graph3[MAX_V][MAX_V] = {
        {0, 1, 1, 0, 0},
        {1, 0, 1, 1, 0},
        {1, 1, 0, 1, 1},
        {0, 1, 1, 0, 1},
        {0, 0, 1, 1, 0}
    };
    
    int choice;
    cout << "\nSelect test graph:" << endl;
    cout << "1. Graph 1 (5 vertices)" << endl;
    cout << "2. Complete graph K4 (4 vertices)" << endl;
    cout << "3. Graph 3 (5 vertices)" << endl;
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
        numVertices = 5;
        for (int i = 0; i < numVertices; i++) {
            for (int j = 0; j < numVertices; j++) {
                graph[i][j] = graph3[i][j];
            }
        }
        cout << "\n========================================" << endl;
        cout << "    Test Graph 3                       " << endl;
        cout << "========================================" << endl;
    } else {
        cout << "Invalid choice!" << endl;
        return 1;
    }
    
    printGraph(graph);
    printEdges(graph);
    
    int mode;
    cout << "\nSelect mode:" << endl;
    cout << "1. Find Hamiltonian Path" << endl;
    cout << "2. Find Hamiltonian Cycle" << endl;
    cout << "3. Find Both" << endl;
    cout << "Enter choice (1-3): ";
    cin >> mode;
    
    int path[MAX_V];
    
    if (mode == 1 || mode == 3) {
        cout << "\n========================================" << endl;
        cout << "    Searching for Hamiltonian Path     " << endl;
        cout << "========================================" << endl;
        
        if (hamiltonianPath(graph, path)) {
            cout << "✓ Hamiltonian path found:" << endl;
            printPath(path, false);
        } else {
            cout << "✗ No Hamiltonian path exists" << endl;
        }
    }
    
    if (mode == 2 || mode == 3) {
        cout << "\n========================================" << endl;
        cout << "    Searching for Hamiltonian Cycle    " << endl;
        cout << "========================================" << endl;
        
        if (hamiltonianCycle(graph, path)) {
            cout << "✓ Hamiltonian cycle found:" << endl;
            printPath(path, true);
        } else {
            cout << "✗ No Hamiltonian cycle exists" << endl;
        }
    }
    
    return 0;
}
