#include <iostream>
#include <vector>
#include <chrono>
#include <unistd.h>
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
bool stepByStep = false;

// Visualize current path state
void visualizePathState(int path[], int pos) {
    cout << "\n--- Current Path State ---" << endl;
    cout << "Position: " << pos << "/" << numVertices << endl;
    cout << "Path so far: ";
    
    for (int i = 0; i < pos; i++) {
        cout << path[i];
        if (i < pos - 1) cout << " -> ";
    }
    
    if (pos < numVertices) {
        cout << " -> ?";
    }
    
    cout << endl;
    
    cout << "Visited vertices: ";
    for (int i = 0; i < pos; i++) {
        cout << path[i] << " ";
    }
    cout << endl;
    
    cout << "Remaining vertices: ";
    for (int v = 0; v < numVertices; v++) {
        bool visited = false;
        for (int i = 0; i < pos; i++) {
            if (path[i] == v) {
                visited = true;
                break;
            }
        }
        if (!visited) {
            cout << v << " ";
        }
    }
    cout << endl;
    
    usleep(500000);  // 0.5 second delay
}

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
    stats.recursionCalls++;
    stats.pathsExplored++;
    
    if (stepByStep) {
        visualizePathState(path, pos);
    }
    
    // Base case: all vertices are included in the path
    if (pos == numVertices) {
        if (stepByStep) {
            cout << "\n✓ Complete path found!" << endl;
        }
        return true;
    }
    
    // Try different vertices as the next candidate in path
    for (int v = 0; v < numVertices; v++) {
        if (stepByStep) {
            cout << "Trying vertex " << v << "..." << endl;
        }
        
        if (isSafe(v, graph, path, pos)) {
            if (stepByStep) {
                cout << "✓ Vertex " << v << " is safe, adding to path" << endl;
            }
            
            path[pos] = v;
            
            // Recursively construct rest of the path
            if (hamiltonianPathUtil(graph, path, pos + 1)) {
                return true;
            }
            
            // Backtrack if adding vertex v doesn't lead to solution
            if (stepByStep) {
                cout << "✗ Backtracking from vertex " << v << endl;
            }
            
            path[pos] = -1;
            stats.backtrackCount++;
        } else {
            if (stepByStep) {
                cout << "✗ Vertex " << v << " is not safe" << endl;
            }
        }
    }
    
    return false;
}

// Recursive function to find Hamiltonian cycle
bool hamiltonianCycleUtil(bool graph[MAX_V][MAX_V], int path[], int pos) {
    stats.recursionCalls++;
    stats.pathsExplored++;
    
    if (stepByStep) {
        visualizePathState(path, pos);
    }
    
    // Base case: all vertices are included in the path
    if (pos == numVertices) {
        // Check if there is an edge from the last vertex to the first vertex
        if (graph[path[pos - 1]][path[0]] == 1) {
            if (stepByStep) {
                cout << "\n✓ Complete cycle found! Edge exists from " 
                     << path[pos - 1] << " back to " << path[0] << endl;
            }
            return true;
        }
        if (stepByStep) {
            cout << "\n✗ No edge from " << path[pos - 1] << " back to " << path[0] << endl;
        }
        return false;
    }
    
    // Try different vertices as the next candidate in path
    for (int v = 1; v < numVertices; v++) {  // Start from 1 since 0 is already in path
        if (stepByStep) {
            cout << "Trying vertex " << v << "..." << endl;
        }
        
        if (isSafe(v, graph, path, pos)) {
            if (stepByStep) {
                cout << "✓ Vertex " << v << " is safe, adding to path" << endl;
            }
            
            path[pos] = v;
            
            // Recursively construct rest of the path
            if (hamiltonianCycleUtil(graph, path, pos + 1)) {
                return true;
            }
            
            // Backtrack if adding vertex v doesn't lead to solution
            if (stepByStep) {
                cout << "✗ Backtracking from vertex " << v << endl;
            }
            
            path[pos] = -1;
            stats.backtrackCount++;
        } else {
            if (stepByStep) {
                cout << "✗ Vertex " << v << " is not safe" << endl;
            }
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

// Print statistics
void printStats(string type = "") {
    cout << "\n========================================" << endl;
    cout << "           Statistics";
    if (!type.empty()) cout << " (" << type << ")";
    cout << endl;
    cout << "========================================" << endl;
    cout << "Recursion calls: " << stats.recursionCalls << endl;
    cout << "Paths explored: " << stats.pathsExplored << endl;
    cout << "Backtrack operations: " << stats.backtrackCount << endl;
    cout << "Execution time: " << stats.executionTime << " μs" << endl;
    cout << "========================================" << endl;
}

// Count degree of each vertex
void printDegrees(bool graph[MAX_V][MAX_V]) {
    cout << "\nVertex Degrees:" << endl;
    for (int i = 0; i < numVertices; i++) {
        int degree = 0;
        for (int j = 0; j < numVertices; j++) {
            if (graph[i][j]) {
                degree++;
            }
        }
        cout << "Vertex " << i << ": degree " << degree << endl;
    }
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
    printDegrees(graph);
    
    int mode;
    cout << "\nSelect mode:" << endl;
    cout << "1. Find Hamiltonian Path" << endl;
    cout << "2. Find Hamiltonian Cycle" << endl;
    cout << "3. Find Both" << endl;
    cout << "Enter choice (1-3): ";
    cin >> mode;
    
    int vizMode;
    cout << "\nSelect visualization mode:" << endl;
    cout << "1. Instant solution" << endl;
    cout << "2. Step-by-step visualization" << endl;
    cout << "Enter choice (1-2): ";
    cin >> vizMode;
    
    stepByStep = (vizMode == 2);
    
    int path[MAX_V];
    
    if (mode == 1 || mode == 3) {
        cout << "\n========================================" << endl;
        cout << "    Searching for Hamiltonian Path     " << endl;
        cout << "========================================" << endl;
        
        stats = Stats();
        auto start = high_resolution_clock::now();
        
        bool found = hamiltonianPath(graph, path);
        
        auto end = high_resolution_clock::now();
        stats.executionTime = duration_cast<microseconds>(end - start).count();
        
        if (found) {
            cout << "\n✓ Hamiltonian path found:" << endl;
            printPath(path, false);
        } else {
            cout << "\n✗ No Hamiltonian path exists" << endl;
        }
        
        printStats("Path");
    }
    
    if (mode == 2 || mode == 3) {
        cout << "\n========================================" << endl;
        cout << "    Searching for Hamiltonian Cycle    " << endl;
        cout << "========================================" << endl;
        
        stats = Stats();
        auto start = high_resolution_clock::now();
        
        bool found = hamiltonianCycle(graph, path);
        
        auto end = high_resolution_clock::now();
        stats.executionTime = duration_cast<microseconds>(end - start).count();
        
        if (found) {
            cout << "\n✓ Hamiltonian cycle found:" << endl;
            printPath(path, true);
        } else {
            cout << "\n✗ No Hamiltonian cycle exists" << endl;
        }
        
        printStats("Cycle");
    }
    
    return 0;
}
