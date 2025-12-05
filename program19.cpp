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
vector<vector<int>> allPaths;
vector<vector<int>> allCycles;

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

// Recursive function to find all Hamiltonian paths
void findAllPathsUtil(bool graph[MAX_V][MAX_V], int path[], int pos) {
    stats.recursionCalls++;
    
    // Base case: all vertices are included in the path
    if (pos == numVertices) {
        vector<int> currentPath;
        for (int i = 0; i < numVertices; i++) {
            currentPath.push_back(path[i]);
        }
        allPaths.push_back(currentPath);
        return;
    }
    
    // Try different vertices as next candidate
    for (int v = 0; v < numVertices; v++) {
        if (isSafe(v, graph, path, pos)) {
            path[pos] = v;
            stats.pathsExplored++;
            
            findAllPathsUtil(graph, path, pos + 1);
            
            path[pos] = -1;
            stats.backtrackCount++;
        }
    }
}

// Recursive function to find Hamiltonian cycle
bool hamiltonianCycleUtil(bool graph[MAX_V][MAX_V], int path[], int pos) {
    stats.recursionCalls++;
    stats.pathsExplored++;
    
    // Base case: all vertices are included
    if (pos == numVertices) {
        // Check if there's an edge from last vertex to first vertex
        if (graph[path[pos - 1]][path[0]]) {
            return true;
        }
        return false;
    }
    
    // Try different vertices as next candidate
    for (int v = 1; v < numVertices; v++) {
        if (isSafe(v, graph, path, pos)) {
            path[pos] = v;
            
            if (hamiltonianCycleUtil(graph, path, pos + 1)) {
                return true;
            }
            
            path[pos] = -1;
            stats.backtrackCount++;
        }
    }
    
    return false;
}

// Recursive function to find all Hamiltonian cycles
void findAllCyclesUtil(bool graph[MAX_V][MAX_V], int path[], int pos) {
    stats.recursionCalls++;
    
    // Base case: all vertices are included
    if (pos == numVertices) {
        // Check if there's an edge from last vertex to first vertex
        if (graph[path[pos - 1]][path[0]]) {
            vector<int> currentCycle;
            for (int i = 0; i < numVertices; i++) {
                currentCycle.push_back(path[i]);
            }
            allCycles.push_back(currentCycle);
        }
        return;
    }
    
    // Try different vertices as next candidate
    for (int v = 1; v < numVertices; v++) {
        if (isSafe(v, graph, path, pos)) {
            path[pos] = v;
            stats.pathsExplored++;
            
            findAllCyclesUtil(graph, path, pos + 1);
            
            path[pos] = -1;
            stats.backtrackCount++;
        }
    }
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

// Find all Hamiltonian paths
void findAllPaths(bool graph[MAX_V][MAX_V]) {
    int path[MAX_V];
    allPaths.clear();
    
    for (int i = 0; i < numVertices; i++) {
        path[i] = -1;
    }
    
    // Try starting from each vertex
    for (int start = 0; start < numVertices; start++) {
        path[0] = start;
        findAllPathsUtil(graph, path, 1);
        path[0] = -1;
    }
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

// Find all Hamiltonian cycles
void findAllCycles(bool graph[MAX_V][MAX_V]) {
    int path[MAX_V];
    allCycles.clear();
    
    for (int i = 0; i < numVertices; i++) {
        path[i] = -1;
    }
    
    // Start from vertex 0 to avoid duplicates
    path[0] = 0;
    findAllCyclesUtil(graph, path, 1);
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

// Print vector path
void printVectorPath(vector<int>& path) {
    for (int i = 0; i < path.size(); i++) {
        cout << path[i];
        if (i < path.size() - 1) {
            cout << " -> ";
        }
    }
    cout << endl;
}

// Print Hamiltonian cycle
void printCycle(int path[]) {
    cout << "\nHamiltonian Cycle:" << endl;
    for (int i = 0; i < numVertices; i++) {
        cout << path[i] << " -> ";
    }
    cout << path[0] << endl;
}

// Print vector cycle
void printVectorCycle(vector<int>& path) {
    for (int i = 0; i < path.size(); i++) {
        cout << path[i] << " -> ";
    }
    cout << path[0] << endl;
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
    
    // Test graph 4: Cycle graph C5 (has Hamiltonian cycle)
    bool graph4[MAX_V][MAX_V] = {
        {0, 1, 0, 0, 1},
        {1, 0, 1, 0, 0},
        {0, 1, 0, 1, 0},
        {0, 0, 1, 0, 1},
        {1, 0, 0, 1, 0}
    };
    
    int choice;
    cout << "\nSelect test graph:" << endl;
    cout << "1. Graph 1 (5 vertices)" << endl;
    cout << "2. Complete graph K4 (4 vertices)" << endl;
    cout << "3. Graph without Hamiltonian path (4 vertices)" << endl;
    cout << "4. Cycle graph C5 (5 vertices)" << endl;
    cout << "Enter choice (1-4): ";
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
    } else if (choice == 4) {
        numVertices = 5;
        for (int i = 0; i < numVertices; i++) {
            for (int j = 0; j < numVertices; j++) {
                graph[i][j] = graph4[i][j];
            }
        }
        cout << "\n========================================" << endl;
        cout << "    Cycle Graph C5                     " << endl;
        cout << "========================================" << endl;
    } else {
        cout << "Invalid choice!" << endl;
        return 1;
    }
    
    printGraph(graph);
    printEdges(graph);
    
    int mode;
    cout << "\nSelect mode:" << endl;
    cout << "1. Find Hamiltonian path" << endl;
    cout << "2. Find Hamiltonian cycle" << endl;
    cout << "3. Find all Hamiltonian paths" << endl;
    cout << "4. Find all Hamiltonian cycles" << endl;
    cout << "Enter choice (1-4): ";
    cin >> mode;
    
    int path[MAX_V];
    
    if (mode == 1) {
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
        
    } else if (mode == 2) {
        cout << "\n========================================" << endl;
        cout << "  Finding Hamiltonian Cycle            " << endl;
        cout << "========================================" << endl;
        
        stats = Stats();
        auto start = high_resolution_clock::now();
        
        bool found = hamiltonianCycle(graph, path);
        
        auto end = high_resolution_clock::now();
        stats.executionTime = duration_cast<microseconds>(end - start).count();
        
        if (found) {
            cout << "\n✓ Hamiltonian cycle found!" << endl;
            printCycle(path);
        } else {
            cout << "\n✗ No Hamiltonian cycle exists" << endl;
        }
        
        printStats();
        
    } else if (mode == 3) {
        cout << "\n========================================" << endl;
        cout << "  Finding All Hamiltonian Paths        " << endl;
        cout << "========================================" << endl;
        
        stats = Stats();
        auto start = high_resolution_clock::now();
        
        findAllPaths(graph);
        
        auto end = high_resolution_clock::now();
        stats.executionTime = duration_cast<microseconds>(end - start).count();
        
        if (allPaths.size() > 0) {
            cout << "\n✓ Found " << allPaths.size() << " Hamiltonian path(s):" << endl;
            for (int i = 0; i < allPaths.size(); i++) {
                cout << "\nPath " << (i + 1) << ": ";
                printVectorPath(allPaths[i]);
            }
        } else {
            cout << "\n✗ No Hamiltonian paths exist" << endl;
        }
        
        printStats("All Paths");
        
    } else if (mode == 4) {
        cout << "\n========================================" << endl;
        cout << "  Finding All Hamiltonian Cycles       " << endl;
        cout << "========================================" << endl;
        
        stats = Stats();
        auto start = high_resolution_clock::now();
        
        findAllCycles(graph);
        
        auto end = high_resolution_clock::now();
        stats.executionTime = duration_cast<microseconds>(end - start).count();
        
        if (allCycles.size() > 0) {
            cout << "\n✓ Found " << allCycles.size() << " Hamiltonian cycle(s):" << endl;
            for (int i = 0; i < allCycles.size(); i++) {
                cout << "\nCycle " << (i + 1) << ": ";
                printVectorCycle(allCycles[i]);
            }
        } else {
            cout << "\n✗ No Hamiltonian cycles exist" << endl;
        }
        
        printStats("All Cycles");
    }
    
    return 0;
}
