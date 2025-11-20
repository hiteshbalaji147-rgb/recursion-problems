#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

#define MAX_VERTICES 20

// Statistics structure
struct Stats {
    int recursionCalls = 0;
    int backtrackCount = 0;
    int safetyChecks = 0;
    int colorAssignments = 0;
};

Stats stats;

// Graph structure
struct Graph {
    int vertices;
    bool adjMatrix[MAX_VERTICES][MAX_VERTICES];
    
    Graph(int v) : vertices(v) {
        for (int i = 0; i < MAX_VERTICES; i++) {
            for (int j = 0; j < MAX_VERTICES; j++) {
                adjMatrix[i][j] = false;
            }
        }
    }
    
    void addEdge(int u, int v) {
        adjMatrix[u][v] = true;
        adjMatrix[v][u] = true;
    }
    
    int getDegree(int vertex) {
        int degree = 0;
        for (int i = 0; i < vertices; i++) {
            if (adjMatrix[vertex][i]) degree++;
        }
        return degree;
    }
};

// Print adjacency matrix
void printAdjMatrix(Graph &graph) {
    cout << "\nAdjacency Matrix:" << endl;
    cout << "   ";
    for (int i = 0; i < graph.vertices; i++) {
        cout << setw(3) << i;
    }
    cout << endl;
    
    for (int i = 0; i < graph.vertices; i++) {
        cout << setw(3) << i;
        for (int j = 0; j < graph.vertices; j++) {
            cout << setw(3) << (graph.adjMatrix[i][j] ? "1" : ".");
        }
        cout << endl;
    }
}

// Print graph edges
void printEdges(Graph &graph) {
    cout << "\nGraph Edges:" << endl;
    for (int i = 0; i < graph.vertices; i++) {
        cout << "Vertex " << i << " -> ";
        bool first = true;
        for (int j = 0; j < graph.vertices; j++) {
            if (graph.adjMatrix[i][j]) {
                if (!first) cout << ", ";
                cout << j;
                first = false;
            }
        }
        if (first) cout << "No connections";
        cout << " (degree: " << graph.getDegree(i) << ")" << endl;
    }
}

// Print colored graph
void printColoredGraph(Graph &graph, int colors[]) {
    cout << "\nColored Graph:" << endl;
    for (int i = 0; i < graph.vertices; i++) {
        cout << "Vertex " << setw(2) << i << " [Color " << colors[i] << "] -> ";
        bool first = true;
        for (int j = 0; j < graph.vertices; j++) {
            if (graph.adjMatrix[i][j]) {
                if (!first) cout << ", ";
                cout << j << "(C" << colors[j] << ")";
                first = false;
            }
        }
        cout << endl;
    }
}

// Check if color assignment is safe
bool isSafe(Graph &graph, int vertex, int color, int colors[]) {
    stats.safetyChecks++;
    
    for (int i = 0; i < graph.vertices; i++) {
        if (graph.adjMatrix[vertex][i] && colors[i] == color) {
            return false;
        }
    }
    return true;
}

// Recursive graph coloring
bool graphColoringUtil(Graph &graph, int m, int colors[], int vertex) {
    stats.recursionCalls++;
    
    if (vertex == graph.vertices) {
        return true;
    }
    
    for (int c = 1; c <= m; c++) {
        if (isSafe(graph, vertex, c, colors)) {
            colors[vertex] = c;
            stats.colorAssignments++;
            
            if (graphColoringUtil(graph, m, colors, vertex + 1)) {
                return true;
            }
            
            colors[vertex] = 0;
            stats.backtrackCount++;
        }
    }
    
    return false;
}

// Main graph coloring function
bool graphColoring(Graph &graph, int m, int colors[]) {
    for (int i = 0; i < graph.vertices; i++) {
        colors[i] = 0;
    }
    
    if (!graphColoringUtil(graph, m, colors, 0)) {
        return false;
    }
    
    return true;
}

// Print statistics
void printStats() {
    cout << "\n========================================" << endl;
    cout << "           Statistics                  " << endl;
    cout << "========================================" << endl;
    cout << "Recursion calls: " << stats.recursionCalls << endl;
    cout << "Backtrack operations: " << stats.backtrackCount << endl;
    cout << "Safety checks: " << stats.safetyChecks << endl;
    cout << "Color assignments: " << stats.colorAssignments << endl;
    cout << "========================================" << endl;
}

int main() {
    cout << "========================================" << endl;
    cout << "       Graph Coloring Problem          " << endl;
    cout << "========================================" << endl;
    
    int vertices, edges;
    cout << "\nEnter number of vertices: ";
    cin >> vertices;
    
    if (vertices > MAX_VERTICES) {
        cout << "Maximum " << MAX_VERTICES << " vertices allowed!" << endl;
        return 1;
    }
    
    Graph graph(vertices);
    
    cout << "Enter number of edges: ";
    cin >> edges;
    
    cout << "\nEnter edges (u v):" << endl;
    for (int i = 0; i < edges; i++) {
        int u, v;
        cin >> u >> v;
        
        if (u >= vertices || v >= vertices || u < 0 || v < 0) {
            cout << "Invalid edge! Vertices must be 0 to " << vertices - 1 << endl;
            i--;
            continue;
        }
        
        graph.addEdge(u, v);
    }
    
    printAdjMatrix(graph);
    printEdges(graph);
    
    int m;
    cout << "\nEnter number of colors: ";
    cin >> m;
    
    int colors[MAX_VERTICES];
    stats = Stats();
    
    cout << "\nSolving..." << endl;
    
    if (graphColoring(graph, m, colors)) {
        cout << "\n✓ Solution found with " << m << " colors!" << endl;
        printColoredGraph(graph, colors);
        printStats();
    } else {
        cout << "\n✗ No solution exists with " << m << " colors!" << endl;
        printStats();
    }
    
    return 0;
}
