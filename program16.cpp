#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <unistd.h>
#include <algorithm>
using namespace std;
using namespace chrono;

#define MAX_V 10  // Maximum number of vertices

int numVertices;  // Actual number of vertices

// Statistics structure
struct Stats {
    int recursionCalls = 0;
    int backtrackCount = 0;
    int colorAssignments = 0;
    long long executionTime = 0;
};

Stats stats;
bool stepByStep = false;

// Function to check if it's safe to color vertex v with color c
bool isSafe(int v, bool graph[MAX_V][MAX_V], int color[], int c) {
    // Check if any adjacent vertex has the same color
    for (int i = 0; i < numVertices; i++) {
        if (graph[v][i] && c == color[i]) {
            return false;
        }
    }
    return true;
}

// Visualize current state
void visualizeState(int color[], int currentVertex, int attemptedColor) {
    string colors[] = {"Uncolored", "Red", "Green", "Blue", "Yellow", "Orange", "Purple", "Pink", "Brown", "Gray", "Cyan"};
    
    cout << "\n--- Current State ---" << endl;
    cout << "Trying to color vertex " << currentVertex << " with " << colors[attemptedColor] << endl;
    
    for (int i = 0; i < numVertices; i++) {
        if (i == currentVertex) {
            cout << "→ Vertex " << i << ": " << colors[attemptedColor] << " (attempting)" << endl;
        } else if (color[i] != 0) {
            cout << "  Vertex " << i << ": " << colors[color[i]] << " (assigned)" << endl;
        } else {
            cout << "  Vertex " << i << ": Uncolored" << endl;
        }
    }
    
    usleep(500000);  // 0.5 second delay
}

// Basic backtracking algorithm
bool graphColoringBasic(bool graph[MAX_V][MAX_V], int m, int color[], int v) {
    stats.recursionCalls++;
    
    // Base case: all vertices are colored
    if (v == numVertices) {
        return true;
    }
    
    // Try different colors for vertex v
    for (int c = 1; c <= m; c++) {
        if (stepByStep) {
            visualizeState(color, v, c);
        }
        
        // Check if assignment of color c to v is safe
        if (isSafe(v, graph, color, c)) {
            color[v] = c;
            stats.colorAssignments++;
            
            if (stepByStep) {
                cout << "✓ Color " << c << " is safe for vertex " << v << endl;
            }
            
            // Recursively assign colors to rest of vertices
            if (graphColoringBasic(graph, m, color, v + 1)) {
                return true;
            }
            
            // Backtrack if coloring doesn't lead to solution
            if (stepByStep) {
                cout << "✗ Backtracking from vertex " << v << endl;
            }
            
            color[v] = 0;
            stats.backtrackCount++;
        } else {
            if (stepByStep) {
                cout << "✗ Color " << c << " conflicts with adjacent vertices" << endl;
            }
        }
    }
    
    return false;
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

// Optimized algorithm using largest degree first heuristic
bool graphColoringOptimized(bool graph[MAX_V][MAX_V], int m, int color[]) {
    // Create array of vertices sorted by degree (descending)
    vector<pair<int, int>> vertexDegrees;
    for (int i = 0; i < numVertices; i++) {
        vertexDegrees.push_back({getDegree(graph, i), i});
    }
    sort(vertexDegrees.rbegin(), vertexDegrees.rend());
    
    // Initialize all vertices as uncolored
    for (int i = 0; i < numVertices; i++) {
        color[i] = 0;
    }
    
    // Color vertices in order of decreasing degree
    for (int i = 0; i < numVertices; i++) {
        stats.recursionCalls++;
        int v = vertexDegrees[i].second;
        
        // Try colors starting from 1
        for (int c = 1; c <= m; c++) {
            if (isSafe(v, graph, color, c)) {
                color[v] = c;
                stats.colorAssignments++;
                break;
            }
        }
        
        // If no color could be assigned
        if (color[v] == 0) {
            return false;
        }
    }
    
    return true;
}

// Main function to solve graph coloring problem
bool graphColoring(bool graph[MAX_V][MAX_V], int m, int color[], bool useOptimized = false) {
    // Initialize all vertices as uncolored
    for (int i = 0; i < numVertices; i++) {
        color[i] = 0;
    }
    
    if (useOptimized) {
        return graphColoringOptimized(graph, m, color);
    } else {
        return graphColoringBasic(graph, m, color, 0);
    }
}

// Print the graph as adjacency matrix
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

// Visualize colored graph
void visualizeColoring(int color[]) {
    string colors[] = {"", "Red", "Green", "Blue", "Yellow", "Orange", "Purple", "Pink", "Brown", "Gray", "Cyan"};
    
    cout << "\n========================================" << endl;
    cout << "         Colored Graph                 " << endl;
    cout << "========================================" << endl;
    
    for (int i = 0; i < numVertices; i++) {
        cout << "Vertex " << i << " -> " << colors[color[i]] << " (Color " << color[i] << ")" << endl;
    }
}

// Print solution with color codes
void printSolution(int color[]) {
    cout << "\nSolution:" << endl;
    for (int i = 0; i < numVertices; i++) {
        cout << "Vertex " << i << " -> Color " << color[i] << endl;
    }
}

// Print statistics
void printStats(string algorithm = "") {
    cout << "\n========================================" << endl;
    cout << "           Statistics";
    if (!algorithm.empty()) cout << " (" << algorithm << ")";
    cout << endl;
    cout << "========================================" << endl;
    cout << "Recursion calls: " << stats.recursionCalls << endl;
    cout << "Color assignments: " << stats.colorAssignments << endl;
    cout << "Backtrack operations: " << stats.backtrackCount << endl;
    cout << "Execution time: " << stats.executionTime << " μs" << endl;
    cout << "========================================" << endl;
}

// Find chromatic number (minimum colors needed)
int findChromaticNumber(bool graph[MAX_V][MAX_V]) {
    int color[MAX_V];
    Stats tempStats = stats;
    
    // Try with increasing number of colors
    for (int m = 1; m <= numVertices; m++) {
        stats = Stats();  // Reset stats for each attempt
        if (graphColoring(graph, m, color, false)) {
            stats = tempStats;  // Restore original stats
            return m;
        }
    }
    
    stats = tempStats;  // Restore original stats
    return numVertices;
}

// Count degree of each vertex
void printDegrees(bool graph[MAX_V][MAX_V]) {
    cout << "\nVertex Degrees:" << endl;
    for (int i = 0; i < numVertices; i++) {
        int degree = getDegree(graph, i);
        cout << "Vertex " << i << ": degree " << degree << endl;
    }
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
    cout << "      Graph Coloring Problem           " << endl;
    cout << "========================================" << endl;
    
    // Test graphs
    bool graph1[MAX_V][MAX_V] = {
        {0, 1, 1, 1, 0},
        {1, 0, 1, 0, 1},
        {1, 1, 0, 1, 1},
        {1, 0, 1, 0, 1},
        {0, 1, 1, 1, 0}
    };
    
    bool graph2[MAX_V][MAX_V] = {
        {0, 1, 0, 1},
        {1, 0, 1, 0},
        {0, 1, 0, 1},
        {1, 0, 1, 0}
    };
    
    bool graph3[MAX_V][MAX_V] = {
        {0, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0}
    };
    
    int mode;
    cout << "\nSelect mode:" << endl;
    cout << "1. Use preset graph" << endl;
    cout << "2. Input custom graph" << endl;
    cout << "Enter choice (1-2): ";
    cin >> mode;
    
    bool graph[MAX_V][MAX_V];
    
    if (mode == 1) {
        int choice;
        cout << "\nSelect test graph:" << endl;
        cout << "1. Pentagon graph (5 vertices)" << endl;
        cout << "2. Square graph (4 vertices)" << endl;
        cout << "3. Star graph (6 vertices)" << endl;
        cout << "Enter choice (1-3): ";
        cin >> choice;
        
        if (choice == 1) {
            numVertices = 5;
            for (int i = 0; i < numVertices; i++) {
                for (int j = 0; j < numVertices; j++) {
                    graph[i][j] = graph1[i][j];
                }
            }
            cout << "\n========================================" << endl;
            cout << "    Pentagon Graph (Cycle C5)          " << endl;
            cout << "========================================" << endl;
        } else if (choice == 2) {
            numVertices = 4;
            for (int i = 0; i < numVertices; i++) {
                for (int j = 0; j < numVertices; j++) {
                    graph[i][j] = graph2[i][j];
                }
            }
            cout << "\n========================================" << endl;
            cout << "    Square Graph (Cycle C4)            " << endl;
            cout << "========================================" << endl;
        } else if (choice == 3) {
            numVertices = 6;
            for (int i = 0; i < numVertices; i++) {
                for (int j = 0; j < numVertices; j++) {
                    graph[i][j] = graph3[i][j];
                }
            }
            cout << "\n========================================" << endl;
            cout << "    Star Graph (K1,5)                  " << endl;
            cout << "========================================" << endl;
        } else {
            cout << "Invalid choice!" << endl;
            return 1;
        }
    } else if (mode == 2) {
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
    
    cout << "\nFinding chromatic number..." << endl;
    int chromaticNumber = findChromaticNumber(graph);
    cout << "Chromatic number (minimum colors needed): " << chromaticNumber << endl;
    
    int m;
    cout << "\nEnter number of colors to use: ";
    cin >> m;
    
    if (m < chromaticNumber) {
        cout << "\nWarning: " << m << " colors may not be sufficient!" << endl;
        cout << "Minimum required: " << chromaticNumber << " colors" << endl;
    }
    
    int algorithm;
    cout << "\nSelect algorithm:" << endl;
    cout << "1. Basic backtracking" << endl;
    cout << "2. Optimized (largest degree first)" << endl;
    cout << "Enter choice (1-2): ";
    cin >> algorithm;
    
    if (algorithm < 1 || algorithm > 2) {
        cout << "Invalid choice!" << endl;
        return 1;
    }
    
    if (algorithm == 1) {
        int vizMode;
        cout << "\nSelect visualization mode:" << endl;
        cout << "1. Instant solution" << endl;
        cout << "2. Step-by-step visualization" << endl;
        cout << "Enter choice (1-2): ";
        cin >> vizMode;
        
        stepByStep = (vizMode == 2);
    }
    
    cout << "\nTrying to color graph with " << m << " colors..." << endl;
    cout << "========================================" << endl;
    
    stats = Stats();
    
    auto start = high_resolution_clock::now();
    
    int color[MAX_V];
    bool solved = graphColoring(graph, m, color, algorithm == 2);
    
    auto end = high_resolution_clock::now();
    stats.executionTime = duration_cast<microseconds>(end - start).count();
    
    if (solved) {
        cout << "\n✓ Solution exists with " << m << " colors!" << endl;
        printSolution(color);
        visualizeColoring(color);
    } else {
        cout << "\n✗ Solution does not exist with " << m << " colors!" << endl;
        cout << "Try using at least " << chromaticNumber << " colors." << endl;
    }
    
    printStats(algorithm == 1 ? "Basic" : "Optimized");
    
    return 0;
}
