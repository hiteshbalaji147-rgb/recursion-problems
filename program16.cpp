#include <iostream>
#include <vector>
#include <string>
using namespace std;

#define MAX_V 10  // Maximum number of vertices

int numVertices;  // Actual number of vertices

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

// Recursive function to solve graph coloring problem
bool graphColoringUtil(bool graph[MAX_V][MAX_V], int m, int color[], int v) {
    // Base case: all vertices are colored
    if (v == numVertices) {
        return true;
    }
    
    // Try different colors for vertex v
    for (int c = 1; c <= m; c++) {
        // Check if assignment of color c to v is safe
        if (isSafe(v, graph, color, c)) {
            color[v] = c;
            
            // Recursively assign colors to rest of vertices
            if (graphColoringUtil(graph, m, color, v + 1)) {
                return true;
            }
            
            // Backtrack if coloring doesn't lead to solution
            color[v] = 0;
        }
    }
    
    return false;
}

// Main function to solve graph coloring problem
bool graphColoring(bool graph[MAX_V][MAX_V], int m, int color[]) {
    // Initialize all vertices as uncolored
    for (int i = 0; i < numVertices; i++) {
        color[i] = 0;
    }
    
    // Start coloring from vertex 0
    if (!graphColoringUtil(graph, m, color, 0)) {
        return false;
    }
    
    return true;
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

// Find chromatic number (minimum colors needed)
int findChromaticNumber(bool graph[MAX_V][MAX_V]) {
    int color[MAX_V];
    
    // Try with increasing number of colors
    for (int m = 1; m <= numVertices; m++) {
        if (graphColoring(graph, m, color)) {
            return m;
        }
    }
    
    return numVertices;
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
    
    int choice;
    cout << "\nSelect test graph:" << endl;
    cout << "1. Pentagon graph (5 vertices)" << endl;
    cout << "2. Square graph (4 vertices)" << endl;
    cout << "3. Star graph (6 vertices)" << endl;
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
    
    printGraph(graph);
    printEdges(graph);
    
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
    
    cout << "\nTrying to color graph with " << m << " colors..." << endl;
    cout << "========================================" << endl;
    
    int color[MAX_V];
    if (graphColoring(graph, m, color)) {
        cout << "\n✓ Solution exists with " << m << " colors!" << endl;
        printSolution(color);
        visualizeColoring(color);
    } else {
        cout << "\n✗ Solution does not exist with " << m << " colors!" << endl;
        cout << "Try using at least " << chromaticNumber << " colors." << endl;
    }
    
    return 0;
}
