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

// Find chromatic number (minimum colors needed)
int findChromaticNumber(bool graph[MAX_V][MAX_V]) {
    int color[MAX_V];
    
    // Try with increasing number of colors
    for (int m = 1; m <= numVertices; m++) {
        Stats tempStats = stats;  // Save current stats
        
        if (graphColoring(graph, m, color)) {
            stats = tempStats;  // Restore stats for final run
            return m;
        }
        
        stats = tempStats;  // Restore stats for next iteration
    }
    
    return numVertices;  // Worst case
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
    int maxDegree = 0;
    for (int i = 0; i < numVertices; i++) {
        int degree = getDegree(graph, i);
        cout << "Vertex " << i << ": degree " << degree << endl;
        maxDegree = max(maxDegree, degree);
    }
    cout << "Maximum degree (Δ): " << maxDegree << endl;
    cout << "Upper bound (Brooks): " << maxDegree << " colors" << endl;
}

// Print coloring solution
void printColoring(int color[]) {
    cout << "\nVertex Coloring:" << endl;
    for (int i = 0; i < numVertices; i++) {
        cout << "Vertex " << i << " -> Color " << color[i] << endl;
    }
}

// Visualize coloring with color groups
void visualizeColoring(int color[], int m) {
    cout << "\nColor Groups:" << endl;
    for (int c = 1; c <= m; c++) {
        cout << "Color " << c << ": ";
        bool first = true;
        for (int i = 0; i < numVertices; i++) {
            if (color[i] == c) {
                if (!first) cout << ", ";
                cout << i;
                first = false;
            }
        }
        cout << endl;
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
    
    // Test graph 2: Bipartite graph (2 colors)
    bool graph2[MAX_V][MAX_V] = {
        {0, 1, 0, 1},
        {1, 0, 1, 0},
        {0, 1, 0, 1},
        {1, 0, 1, 0}
    };
    
    // Test graph 3: Complete graph K5 (5 colors)
    bool graph3[MAX_V][MAX_V] = {
        {0, 1, 1, 1, 1},
        {1, 0, 1, 1, 1},
        {1, 1, 0, 1, 1},
        {1, 1, 1, 0, 1},
        {1, 1, 1, 1, 0}
    };
    
    // Test graph 4: Cycle graph C5 (3 colors)
    bool graph4[MAX_V][MAX_V] = {
        {0, 1, 0, 0, 1},
        {1, 0, 1, 0, 0},
        {0, 1, 0, 1, 0},
        {0, 0, 1, 0, 1},
        {1, 0, 0, 1, 0}
    };
    
    int choice;
    cout << "\nSelect test graph:" << endl;
    cout << "1. Graph 1 (4 vertices)" << endl;
    cout << "2. Bipartite graph (4 vertices)" << endl;
    cout << "3. Complete graph K5 (5 vertices)" << endl;
    cout << "4. Cycle graph C5 (5 vertices)" << endl;
    cout << "Enter choice (1-4): ";
    cin >> choice;
    
    bool graph[MAX_V][MAX_V];
    
    if (choice == 1) {
        numVertices = 4;
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
        cout << "    Bipartite Graph                    " << endl;
        cout << "========================================" << endl;
    } else if (choice == 3) {
        numVertices = 5;
        for (int i = 0; i < numVertices; i++) {
            for (int j = 0; j < numVertices; j++) {
                graph[i][j] = graph3[i][j];
            }
        }
        cout << "\n========================================" << endl;
        cout << "    Complete Graph K5                  " << endl;
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
    printDegrees(graph);
    
    int mode;
    cout << "\nSelect mode:" << endl;
    cout << "1. Find chromatic number (minimum colors)" << endl;
    cout << "2. Try with specific number of colors" << endl;
    cout << "Enter choice (1-2): ";
    cin >> mode;
    
    int color[MAX_V];
    
    if (mode == 1) {
        cout << "\n========================================" << endl;
        cout << "  Finding Chromatic Number             " << endl;
        cout << "========================================" << endl;
        
        auto start = high_resolution_clock::now();
        
        int chromaticNum = findChromaticNumber(graph);
        
        auto end = high_resolution_clock::now();
        long long searchTime = duration_cast<microseconds>(end - start).count();
        
        cout << "\n✓ Chromatic number (χ): " << chromaticNum << endl;
        cout << "Search time: " << searchTime << " μs" << endl;
        
        // Now solve with chromatic number to get the coloring
        stats = Stats();
        start = high_resolution_clock::now();
        
        graphColoring(graph, chromaticNum, color);
        
        end = high_resolution_clock::now();
        stats.executionTime = duration_cast<microseconds>(end - start).count();
        
        cout << "\nOptimal coloring with " << chromaticNum << " colors:" << endl;
        printColoring(color);
        visualizeColoring(color, chromaticNum);
        printStats();
        
    } else if (mode == 2) {
        int m;
        cout << "\nEnter number of colors: ";
        cin >> m;
        
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
            visualizeColoring(color, m);
        } else {
            cout << "\n✗ No solution exists with " << m << " colors" << endl;
        }
        
        printStats();
    }
    
    return 0;
}
