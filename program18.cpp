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
    int colorAssignments = 0;
    long long executionTime = 0;
};

Stats stats;

// Function to check if it's safe to color vertex v with color c
bool isSafe(int v, bool graph[MAX_V][MAX_V], int color[], int c) {
    // Check if any adjacent vertex has the same color
    for (int i = 0; i < numVertices; i++) {
        if (graph[v][i] && color[i] == c) {
            return false;
        }
    }
    return true;
}

// Recursive function to solve graph coloring
bool graphColoringUtil(bool graph[MAX_V][MAX_V], int m, int color[], int v) {
    stats.recursionCalls++;
    
    // Base case: all vertices are colored
    if (v == numVertices) {
        return true;
    }
    
    // Try different colors for vertex v
    for (int c = 1; c <= m; c++) {
        if (isSafe(v, graph, color, c)) {
            color[v] = c;
            stats.colorAssignments++;
            
            // Recursively color rest of the vertices
            if (graphColoringUtil(graph, m, color, v + 1)) {
                return true;
            }
            
            // Backtrack if coloring doesn't lead to solution
            color[v] = 0;
            stats.backtrackCount++;
        }
    }
    
    return false;
}

// Main function to solve graph coloring problem
bool graphColoring(bool graph[MAX_V][MAX_V], int m, int color[]) {
    // Initialize all colors as 0 (uncolored)
    for (int i = 0; i < numVertices; i++) {
        color[i] = 0;
    }
    
    if (!graphColoringUtil(graph, m, color, 0)) {
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

// Print coloring solution
void printColoring(int color[]) {
    cout << "\nVertex Coloring:" << endl;
    for (int i = 0; i < numVertices; i++) {
        cout << "Vertex " << i << " -> Color " << color[i] << endl;
    }
}

// Print statistics
void printStats() {
    cout << "\n========================================" << endl;
    cout << "           Statistics                  " << endl;
    cout << "========================================" << endl;
    cout << "Recursion calls: " << stats.recursionCalls << endl;
    cout << "Color assignments: " << stats.colorAssignments << endl;
    cout << "Backtrack operations: " << stats.backtrackCount << endl;
    cout << "Execution time: " << stats.executionTime << " μs" << endl;
    cout << "========================================" << endl;
}

int main() {
    cout << "========================================" << endl;
    cout << "     Graph Coloring Problem            " << endl;
    cout << "========================================" << endl;
    
    // Test graph 1: Simple graph requiring 3 colors
    bool graph1[MAX_V][MAX_V] = {
        {0, 1, 1, 1},
        {1, 0, 1, 0},
        {1, 1, 0, 1},
        {1, 0, 1, 0}
    };
    
    numVertices = 4;
    
    cout << "\n========================================" << endl;
    cout << "    Test Graph 1                       " << endl;
    cout << "========================================" << endl;
    
    bool graph[MAX_V][MAX_V];
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            graph[i][j] = graph1[i][j];
        }
    }
    
    printGraph(graph);
    printEdges(graph);
    
    int m;
    cout << "\nEnter number of colors: ";
    cin >> m;
    
    int color[MAX_V];
    
    cout << "\n========================================" << endl;
    cout << "    Solving Graph Coloring             " << endl;
    cout << "========================================" << endl;
    
    stats = Stats();
    auto start = high_resolution_clock::now();
    
    bool found = graphColoring(graph, m, color);
    
    auto end = high_resolution_clock::now();
    stats.executionTime = duration_cast<microseconds>(end - start).count();
    
    if (found) {
        cout << "\n✓ Solution found with " << m << " colors!" << endl;
        printColoring(color);
    } else {
        cout << "\n✗ No solution exists with " << m << " colors" << endl;
    }
    
    printStats();
    
    return 0;
}
