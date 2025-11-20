#include <iostream>
#include <vector>
using namespace std;

#define MAX_VERTICES 20

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
};

// Check if color assignment is safe
bool isSafe(Graph &graph, int vertex, int color, int colors[]) {
    for (int i = 0; i < graph.vertices; i++) {
        if (graph.adjMatrix[vertex][i] && colors[i] == color) {
            return false;
        }
    }
    return true;
}

// Recursive graph coloring
bool graphColoringUtil(Graph &graph, int m, int colors[], int vertex) {
    if (vertex == graph.vertices) {
        return true;
    }
    
    for (int c = 1; c <= m; c++) {
        if (isSafe(graph, vertex, c, colors)) {
            colors[vertex] = c;
            
            if (graphColoringUtil(graph, m, colors, vertex + 1)) {
                return true;
            }
            
            colors[vertex] = 0;
        }
    }
    
    return false;
}

// Main graph coloring function
bool graphColoring(Graph &graph, int m) {
    int colors[MAX_VERTICES] = {0};
    
    if (!graphColoringUtil(graph, m, colors, 0)) {
        return false;
    }
    
    cout << "\nSolution found with " << m << " colors:" << endl;
    for (int i = 0; i < graph.vertices; i++) {
        cout << "Vertex " << i << " -> Color " << colors[i] << endl;
    }
    
    return true;
}

int main() {
    cout << "========================================" << endl;
    cout << "       Graph Coloring Problem          " << endl;
    cout << "========================================" << endl;
    
    int vertices, edges;
    cout << "\nEnter number of vertices: ";
    cin >> vertices;
    
    Graph graph(vertices);
    
    cout << "Enter number of edges: ";
    cin >> edges;
    
    cout << "\nEnter edges (u v):" << endl;
    for (int i = 0; i < edges; i++) {
        int u, v;
        cin >> u >> v;
        graph.addEdge(u, v);
    }
    
    int m;
    cout << "\nEnter number of colors: ";
    cin >> m;
    
    if (!graphColoring(graph, m)) {
        cout << "\nNo solution exists with " << m << " colors!" << endl;
    }
    
    return 0;
}
