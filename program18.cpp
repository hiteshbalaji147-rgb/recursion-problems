#include <iostream>
#include <vector>
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
vector<vector<int>> allSolutions;

// Visualize current coloring state
void visualizeColoringState(int color[], int v, int m) {
    cout << "\n--- Current Coloring State ---" << endl;
    cout << "Coloring vertex: " << v << "/" << numVertices << endl;
    cout << "Available colors: 1 to " << m << endl;
    
    cout << "\nCurrent coloring:" << endl;
    for (int i = 0; i < v; i++) {
        cout << "Vertex " << i << " -> Color " << color[i] << endl;
    }
    
    cout << "\nUncolored vertices: ";
    for (int i = v; i < numVertices; i++) {
        cout << i << " ";
    }
    cout << endl;
    
    usleep(500000);  // 0.5 second delay
}

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
    
    if (stepByStep) {
        visualizeColoringState(color, v, m);
    }
    
    // Base case: all vertices are colored
    if (v == numVertices) {
        if (stepByStep) {
            cout << "\n✓ Complete coloring found!" << endl;
        }
        return true;
    }
    
    // Try different colors for vertex v
    for (int c = 1; c <= m; c++) {
        if (stepByStep) {
            cout << "Trying color " << c << " for vertex " << v << "..." << endl;
        }
        
        if (isSafe(v, graph, color, c)) {
            if (stepByStep) {
                cout << "✓ Color " << c << " is safe for vertex " << v << endl;
            }
            
            color[v] = c;
            stats.colorAssignments++;
            
            // Recursively color rest of the vertices
            if (graphColoringUtil(graph, m, color, v + 1)) {
                return true;
            }
            
            // Backtrack if coloring doesn't lead to solution
            if (stepByStep) {
                cout << "✗ Backtracking from vertex " << v << ", color " << c << endl;
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

// Recursive function to find all coloring solutions
void findAllSolutionsUtil(bool graph[MAX_V][MAX_V], int m, int color[], int v) {
    stats.recursionCalls++;
    
    // Base case: all vertices are colored
    if (v == numVertices) {
        vector<int> solution;
        for (int i = 0; i < numVertices; i++) {
            solution.push_back(color[i]);
        }
        allSolutions.push_back(solution);
        return;
    }
    
    // Try different colors for vertex v
    for (int c = 1; c <= m; c++) {
        if (isSafe(v, graph, color, c)) {
            color[v] = c;
            stats.colorAssignments++;
            
            // Recursively color rest of the vertices
            findAllSolutionsUtil(graph, m, color, v + 1);
            
            // Backtrack
            color[v] = 0;
            stats.backtrackCount++;
        }
    }
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

// Find all coloring solutions
void findAllSolutions(bool graph[MAX_V][MAX_V], int m) {
    int color[MAX_V];
    allSolutions.clear();
    
    // Initialize all colors as 0 (uncolored)
    for (int i = 0; i < numVertices; i++) {
        color[i] = 0;
    }
    
    findAllSolutionsUtil(graph, m, color, 0);
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

// Greedy coloring algorithm
int greedyColoring(bool graph[MAX_V][MAX_V], int color[]) {
    // Initialize all colors as 0 (uncolored)
    for (int i = 0; i < numVertices; i++) {
        color[i] = 0;
    }
    
    // Color first vertex with color 1
    color[0] = 1;
    
    // Array to track available colors
    bool available[MAX_V];
    
    // Color remaining vertices
    for (int v = 1; v < numVertices; v++) {
        // Initialize all colors as available
        for (int c = 0; c < numVertices; c++) {
            available[c] = true;
        }
        
        // Mark colors of adjacent vertices as unavailable
        for (int i = 0; i < numVertices; i++) {
            if (graph[v][i] && color[i] != 0) {
                available[color[i] - 1] = false;
            }
        }
        
        // Find first available color
        int c;
        for (c = 0; c < numVertices; c++) {
            if (available[c]) {
                break;
            }
        }
        
        color[v] = c + 1;
    }
    
    // Find maximum color used
    int maxColor = 0;
    for (int i = 0; i < numVertices; i++) {
        maxColor = max(maxColor, color[i]);
    }
    
    return maxColor;
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

// Print vector coloring
void printVectorColoring(vector<int>& color) {
    for (int i = 0; i < color.size(); i++) {
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

// Visualize vector coloring
void visualizeVectorColoring(vector<int>& color, int m) {
    cout << "\nColor Groups:" << endl;
    for (int c = 1; c <= m; c++) {
        cout << "Color " << c << ": ";
        bool first = true;
        for (int i = 0; i < color.size(); i++) {
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
void printStats(string type = "") {
    cout << "\n========================================" << endl;
    cout << "           Statistics";
    if (!type.empty()) cout << " (" << type << ")";
    cout << endl;
    cout << "========================================" << endl;
    cout << "Recursion calls: " << stats.recursionCalls << endl;
    cout << "Color assignments: " << stats.colorAssignments << endl;
    cout << "Backtrack operations: " << stats.backtrackCount << endl;
    cout << "Execution time: " << stats.executionTime << " μs" << endl;
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
        cout << "1. Graph 1 (4 vertices)" << endl;
        cout << "2. Bipartite graph (4 vertices)" << endl;
        cout << "3. Complete graph K5 (5 vertices)" << endl;
        cout << "4. Cycle graph C5 (5 vertices)" << endl;
        cout << "Enter choice (1-4): ";
        cin >> choice;
        
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
    
    int mode;
    cout << "\nSelect mode:" << endl;
    cout << "1. Find chromatic number (minimum colors)" << endl;
    cout << "2. Try with specific number of colors" << endl;
    cout << "3. Find all coloring solutions" << endl;
    cout << "4. Compare greedy vs optimal coloring" << endl;
    cout << "Enter choice (1-4): ";
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
        
        int vizMode;
        cout << "\nSelect visualization mode:" << endl;
        cout << "1. Instant solution" << endl;
        cout << "2. Step-by-step visualization" << endl;
        cout << "Enter choice (1-2): ";
        cin >> vizMode;
        
        stepByStep = (vizMode == 2);
        
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
        
    } else if (mode == 3) {
        int m;
        cout << "\nEnter number of colors: ";
        cin >> m;
        
        cout << "\n========================================" << endl;
        cout << "  Finding All Coloring Solutions       " << endl;
        cout << "========================================" << endl;
        
        stats = Stats();
        auto start = high_resolution_clock::now();
        
        findAllSolutions(graph, m);
        
        auto end = high_resolution_clock::now();
        stats.executionTime = duration_cast<microseconds>(end - start).count();
        
        if (allSolutions.size() > 0) {
            cout << "\n✓ Found " << allSolutions.size() << " solution(s) with " << m << " colors:" << endl;
            for (int i = 0; i < allSolutions.size(); i++) {
                cout << "\n--- Solution " << (i + 1) << " ---" << endl;
                printVectorColoring(allSolutions[i]);
                visualizeVectorColoring(allSolutions[i], m);
            }
        } else {
            cout << "\n✗ No solutions exist with " << m << " colors" << endl;
        }
        
        printStats("All Solutions");
        
    } else if (mode == 4) {
        cout << "\n========================================" << endl;
        cout << "  Greedy vs Optimal Comparison         " << endl;
        cout << "========================================" << endl;
        
        // Greedy coloring
        int greedyColor[MAX_V];
        auto start = high_resolution_clock::now();
        int greedyColors = greedyColoring(graph, greedyColor);
        auto end = high_resolution_clock::now();
        long long greedyTime = duration_cast<microseconds>(end - start).count();
        
        cout << "\n--- Greedy Coloring ---" << endl;
        cout << "Colors used: " << greedyColors << endl;
        cout << "Time: " << greedyTime << " μs" << endl;
        printColoring(greedyColor);
        visualizeColoring(greedyColor, greedyColors);
        
        // Optimal coloring
        start = high_resolution_clock::now();
        int chromaticNum = findChromaticNumber(graph);
        end = high_resolution_clock::now();
        long long optimalTime = duration_cast<microseconds>(end - start).count();
        
        stats = Stats();
        graphColoring(graph, chromaticNum, color);
        
        cout << "\n--- Optimal Coloring ---" << endl;
        cout << "Colors used (χ): " << chromaticNum << endl;
        cout << "Time: " << optimalTime << " μs" << endl;
        printColoring(color);
        visualizeColoring(color, chromaticNum);
        
        // Comparison
        cout << "\n--- Comparison ---" << endl;
        cout << "Greedy colors: " << greedyColors << endl;
        cout << "Optimal colors: " << chromaticNum << endl;
        cout << "Difference: " << (greedyColors - chromaticNum) << endl;
        cout << "Greedy efficiency: " << (100.0 * chromaticNum / greedyColors) << "%" << endl;
        cout << "Time speedup (greedy): " << (optimalTime / (double)greedyTime) << "x faster" << endl;
    }
    
    return 0;
}
