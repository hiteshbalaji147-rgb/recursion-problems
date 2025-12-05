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
vector<vector<int>> allPaths;
vector<vector<int>> allCycles;
bool stepByStep = false;

// Visualize current path state
void visualizePathState(int path[], int pos, string type = "Path") {
    cout << "\n--- Current " << type << " State ---" << endl;
    cout << "Position: " << pos << "/" << numVertices << endl;
    
    cout << "\nCurrent " << type << ": ";
    for (int i = 0; i < pos; i++) {
        cout << path[i];
        if (i < pos - 1) {
            cout << " -> ";
        }
    }
    if (pos > 0) {
        cout << " -> ?";
    }
    cout << endl;
    
    cout << "\nVisited vertices: ";
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
    
    if (stepByStep) {
        visualizePathState(path, pos, "Path");
    }
    
    // Base case: all vertices are included in the path
    if (pos == numVertices) {
        if (stepByStep) {
            cout << "\n✓ Complete Hamiltonian path found!" << endl;
        }
        return true;
    }
    
    // Try different vertices as next candidate in Hamiltonian path
    for (int v = 0; v < numVertices; v++) {
        if (stepByStep) {
            cout << "Trying vertex " << v << " at position " << pos << "..." << endl;
        }
        
        if (isSafe(v, graph, path, pos)) {
            if (stepByStep) {
                cout << "✓ Vertex " << v << " is safe" << endl;
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
                cout << "✗ Vertex " << v << " is not safe (not adjacent or already visited)" << endl;
            }
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
    
    if (stepByStep) {
        visualizePathState(path, pos, "Cycle");
    }
    
    // Base case: all vertices are included
    if (pos == numVertices) {
        // Check if there's an edge from last vertex to first vertex
        if (graph[path[pos - 1]][path[0]]) {
            if (stepByStep) {
                cout << "\n✓ Complete Hamiltonian cycle found!" << endl;
                cout << "Edge from " << path[pos - 1] << " to " << path[0] << " exists!" << endl;
            }
            return true;
        }
        if (stepByStep) {
            cout << "\n✗ No edge from " << path[pos - 1] << " to " << path[0] << endl;
        }
        return false;
    }
    
    // Try different vertices as next candidate
    for (int v = 1; v < numVertices; v++) {
        if (stepByStep) {
            cout << "Trying vertex " << v << " at position " << pos << "..." << endl;
        }
        
        if (isSafe(v, graph, path, pos)) {
            if (stepByStep) {
                cout << "✓ Vertex " << v << " is safe" << endl;
            }
            
            path[pos] = v;
            
            if (hamiltonianCycleUtil(graph, path, pos + 1)) {
                return true;
            }
            
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
        if (stepByStep) {
            cout << "\n=== Trying starting vertex: " << start << " ===" << endl;
        }
        
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

// Get degree of a vertex
int getDegree(bool graph[MAX_V][MAX_V], int v) {
    int degree = 0;
    for (int i = 0; i < numVertices; i++) {
        if (graph[v][i]) {
            degree++;
        }
    }
    return degree;
}

// Check if graph is connected
bool isConnected(bool graph[MAX_V][MAX_V]) {
    bool visited[MAX_V] = {false};
    
    // BFS from vertex 0
    vector<int> queue;
    queue.push_back(0);
    visited[0] = true;
    int visitedCount = 1;
    
    while (!queue.empty()) {
        int u = queue[0];
        queue.erase(queue.begin());
        
        for (int v = 0; v < numVertices; v++) {
            if (graph[u][v] && !visited[v]) {
                visited[v] = true;
                visitedCount++;
                queue.push_back(v);
            }
        }
    }
    
    return visitedCount == numVertices;
}

// Check Dirac's theorem (sufficient condition for Hamiltonian cycle)
bool checkDiracTheorem(bool graph[MAX_V][MAX_V]) {
    if (numVertices < 3) {
        return false;
    }
    
    for (int i = 0; i < numVertices; i++) {
        if (getDegree(graph, i) < numVertices / 2) {
            return false;
        }
    }
    
    return true;
}

// Check Ore's theorem (sufficient condition for Hamiltonian cycle)
bool checkOreTheorem(bool graph[MAX_V][MAX_V]) {
    if (numVertices < 3) {
        return false;
    }
    
    for (int i = 0; i < numVertices; i++) {
        for (int j = i + 1; j < numVertices; j++) {
            if (!graph[i][j]) {  // Non-adjacent vertices
                if (getDegree(graph, i) + getDegree(graph, j) < numVertices) {
                    return false;
                }
            }
        }
    }
    
    return true;
}

// Check graph properties
void checkGraphProperties(bool graph[MAX_V][MAX_V]) {
    cout << "\n========================================" << endl;
    cout << "      Graph Properties                 " << endl;
    cout << "========================================" << endl;
    
    bool connected = isConnected(graph);
    bool dirac = checkDiracTheorem(graph);
    bool ore = checkOreTheorem(graph);
    
    cout << "\nConnected: ";
    if (connected) {
        cout << "✓ YES" << endl;
        cout << "  → Graph is connected (necessary for Hamiltonian path/cycle)" << endl;
    } else {
        cout << "✗ NO" << endl;
        cout << "  → Graph is disconnected (no Hamiltonian path/cycle possible)" << endl;
    }
    
    cout << "\nDirac's Theorem (deg(v) ≥ n/2 for all v):" << endl;
    if (dirac) {
        cout << "✓ SATISFIED" << endl;
        cout << "  → Hamiltonian cycle GUARANTEED to exist!" << endl;
    } else {
        cout << "✗ NOT SATISFIED" << endl;
        cout << "  → Hamiltonian cycle may or may not exist" << endl;
    }
    
    cout << "\nOre's Theorem (deg(u) + deg(v) ≥ n for non-adjacent u,v):" << endl;
    if (ore) {
        cout << "✓ SATISFIED" << endl;
        cout << "  → Hamiltonian cycle GUARANTEED to exist!" << endl;
    } else {
        cout << "✗ NOT SATISFIED" << endl;
        cout << "  → Hamiltonian cycle may or may not exist" << endl;
    }
    
    int minDegree = numVertices;
    int maxDegree = 0;
    for (int i = 0; i < numVertices; i++) {
        int deg = getDegree(graph, i);
        minDegree = min(minDegree, deg);
        maxDegree = max(maxDegree, deg);
    }
    
    cout << "\nDegree Statistics:" << endl;
    cout << "Minimum degree: " << minDegree << endl;
    cout << "Maximum degree: " << maxDegree << endl;
    cout << "Required for Dirac: " << (numVertices / 2) << endl;
    
    cout << "========================================" << endl;
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

// Print vertex degrees
void printDegrees(bool graph[MAX_V][MAX_V]) {
    cout << "\nVertex Degrees:" << endl;
    for (int i = 0; i < numVertices; i++) {
        int degree = getDegree(graph, i);
        cout << "Vertex " << i << ": degree " << degree << endl;
    }
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

// Print complexity analysis
void printComplexityAnalysis() {
    cout << "\n========================================" << endl;
    cout << "      Complexity Analysis              " << endl;
    cout << "========================================" << endl;
    cout << "\n1. Hamiltonian Path/Cycle Problem:" << endl;
    cout << "   Classification: NP-complete" << endl;
    cout << "   Decision version: NP-complete" << endl;
    cout << "   Optimization version: NP-hard" << endl;
    
    cout << "\n2. Backtracking Approach:" << endl;
    cout << "   Time: O(V!) worst case" << endl;
    cout << "   Space: O(V) for recursion stack + path array" << endl;
    cout << "   Best case: O(V) if path found quickly" << endl;
    cout << "   Worst case: O(V!) tries all permutations" << endl;
    
    cout << "\n3. Hamiltonian Path vs Cycle:" << endl;
    cout << "   Path: Try V starting vertices" << endl;
    cout << "   Cycle: Fix starting vertex (avoid duplicates)" << endl;
    cout << "   Cycle requires edge back to start" << endl;
    
    cout << "\n4. Sufficient Conditions:" << endl;
    cout << "   Dirac's Theorem: deg(v) ≥ n/2 → cycle exists" << endl;
    cout << "   Ore's Theorem: deg(u)+deg(v) ≥ n → cycle exists" << endl;
    cout << "   (for non-adjacent vertices u,v)" << endl;
    
    cout << "\n5. Necessary Conditions:" << endl;
    cout << "   Graph must be connected" << endl;
    cout << "   For cycle: n ≥ 3 vertices required" << endl;
    
    cout << "\nNote: V = number of vertices" << endl;
    cout << "========================================" << endl;
}

// Print applications
void printApplications() {
    cout << "\n========================================" << endl;
    cout << "      Real-World Applications          " << endl;
    cout << "========================================" << endl;
    cout << "\n1. Traveling Salesman Problem (TSP)" << endl;
    cout << "   - Route optimization for delivery" << endl;
    cout << "   - Logistics and supply chain" << endl;
    cout << "   - Circuit board drilling" << endl;
    
    cout << "\n2. DNA Sequencing" << endl;
    cout << "   - Genome assembly" << endl;
    cout << "   - Fragment reconstruction" << endl;
    cout << "   - Sequence alignment" << endl;
    
    cout << "\n3. Network Routing" << endl;
    cout << "   - Packet routing in networks" << endl;
    cout << "   - Network topology design" << endl;
    cout << "   - Optimal path finding" << endl;
    
    cout << "\n4. Game Theory" << endl;
    cout << "   - Knight's tour problem (chess)" << endl;
    cout << "   - Puzzle solving" << endl;
    cout << "   - Game state exploration" << endl;
    
    cout << "\n5. Manufacturing" << endl;
    cout << "   - Assembly line optimization" << endl;
    cout << "   - Robot arm path planning" << endl;
    cout << "   - CNC machine tool paths" << endl;
    
    cout << "\n6. Computer Graphics" << endl;
    cout << "   - Polygon triangulation" << endl;
    cout << "   - Mesh generation" << endl;
    cout << "   - Path tracing" << endl;
    
    cout << "\n7. Scheduling Problems" << endl;
    cout << "   - Task sequencing" << endl;
    cout << "   - Job shop scheduling" << endl;
    cout << "   - Resource allocation" << endl;
    
    cout << "\n8. Bioinformatics" << endl;
    cout << "   - Protein folding paths" << endl;
    cout << "   - Metabolic pathway analysis" << endl;
    cout << "   - Gene regulatory networks" << endl;
    cout << "========================================" << endl;
}

// Input custom graph
void inputCustomGraph(bool graph[MAX_V][MAX_V]) {
    cout << "\nEnter number of vertices (max " << MAX_V << "): ";
    cin >> numVertices;
    
    if (numVertices > MAX_V) {
        cout << "Too many vertices! Using maximum: " << MAX_V << endl;
        numVertices = MAX_V;
    }
    
    // Initialize graph
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            graph[i][j] = false;
        }
    }
    
    cout << "\nEnter number of edges: ";
    int edges;
    cin >> edges;
    
    cout << "\nEnter edges (format: vertex1 vertex2):" << endl;
    for (int i = 0; i < edges; i++) {
        int u, v;
        cout << "Edge " << (i + 1) << ": ";
        cin >> u >> v;
        
        if (u >= 0 && u < numVertices && v >= 0 && v < numVertices && u != v) {
            graph[u][v] = true;
            graph[v][u] = true;
        } else {
            cout << "Invalid edge! Skipping..." << endl;
            i--;
        }
    }
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
    
    int inputMode;
    cout << "\nSelect input mode:" << endl;
    cout << "1. Use preset graph" << endl;
    cout << "2. Input custom graph" << endl;
    cout << "Enter choice (1-2): ";
    cin >> inputMode;
    
    bool graph[MAX_V][MAX_V];
    
    if (inputMode == 1) {
        int choice;
        cout << "\nSelect test graph:" << endl;
        cout << "1. Graph 1 (5 vertices)" << endl;
        cout << "2. Complete graph K4 (4 vertices)" << endl;
        cout << "3. Graph without Hamiltonian path (4 vertices)" << endl;
        cout << "4. Cycle graph C5 (5 vertices)" << endl;
        cout << "Enter choice (1-4): ";
        cin >> choice;
        
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
    } else if (inputMode == 2) {
        inputCustomGraph(graph);
        cout << "\n========================================" << endl;
        cout << "    Custom Graph                       " << endl;
        cout << "========================================" << endl;
    } else {
        cout << "Invalid choice!" << endl;
        return 1;
    }
    
    printGraph(graph);
    printEdges(graph);
    printDegrees(graph);
    checkGraphProperties(graph);
    
    int mode;
    cout << "\nSelect mode:" << endl;
    cout << "1. Find Hamiltonian path" << endl;
    cout << "2. Find Hamiltonian cycle" << endl;
    cout << "3. Find all Hamiltonian paths" << endl;
    cout << "4. Find all Hamiltonian cycles" << endl;
    cout << "Enter choice (1-4): ";
    cin >> mode;
    
    int path[MAX_V];
    
    if (mode == 1 || mode == 2) {
        int vizMode;
        cout << "\nSelect visualization mode:" << endl;
        cout << "1. Instant solution" << endl;
        cout << "2. Step-by-step visualization" << endl;
        cout << "Enter choice (1-2): ";
        cin >> vizMode;
        
        stepByStep = (vizMode == 2);
    }
    
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
    
    printComplexityAnalysis();
    printApplications();
    
    return 0;
}
