#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
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
    string name;
    
    Graph(int v, string n = "Custom") : vertices(v), name(n) {
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
    
    int getMaxDegree() {
        int maxDeg = 0;
        for (int i = 0; i < vertices; i++) {
            maxDeg = max(maxDeg, getDegree(i));
        }
        return maxDeg;
    }
    
    int countEdges() {
        int count = 0;
        for (int i = 0; i < vertices; i++) {
            for (int j = i + 1; j < vertices; j++) {
                if (adjMatrix[i][j]) count++;
            }
        }
        return count;
    }
};

// Predefined graphs
Graph createCompleteGraph(int n) {
    Graph g(n, "Complete Graph K" + to_string(n));
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            g.addEdge(i, j);
        }
    }
    return g;
}

Graph createCycleGraph(int n) {
    Graph g(n, "Cycle Graph C" + to_string(n));
    for (int i = 0; i < n; i++) {
        g.addEdge(i, (i + 1) % n);
    }
    return g;
}

Graph createBipartiteGraph(int m, int n) {
    Graph g(m + n, "Bipartite Graph K" + to_string(m) + "," + to_string(n));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            g.addEdge(i, m + j);
        }
    }
    return g;
}

Graph createPetersenGraph() {
    Graph g(10, "Petersen Graph");
    // Outer pentagon
    for (int i = 0; i < 5; i++) {
        g.addEdge(i, (i + 1) % 5);
    }
    // Inner pentagram
    for (int i = 0; i < 5; i++) {
        g.addEdge(i + 5, (i + 2) % 5 + 5);
    }
    // Connections
    for (int i = 0; i < 5; i++) {
        g.addEdge(i, i + 5);
    }
    return g;
}

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

// Print graph info
void printGraphInfo(Graph &graph) {
    cout << "\n========================================" << endl;
    cout << "         Graph Information             " << endl;
    cout << "========================================" << endl;
    cout << "Name: " << graph.name << endl;
    cout << "Vertices: " << graph.vertices << endl;
    cout << "Edges: " << graph.countEdges() << endl;
    cout << "Max degree: " << graph.getMaxDegree() << endl;
    cout << "========================================" << endl;
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

// Find chromatic number
int findChromaticNumber(Graph &graph) {
    int colors[MAX_VERTICES];
    
    for (int m = 1; m <= graph.vertices; m++) {
        stats = Stats();
        if (graphColoring(graph, m, colors)) {
            return m;
        }
    }
    
    return graph.vertices;
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
    
    int choice;
    cout << "\nSelect graph type:" << endl;
    cout << "1. Custom graph" << endl;
    cout << "2. Complete graph (Kn)" << endl;
    cout << "3. Cycle graph (Cn)" << endl;
    cout << "4. Bipartite graph (Km,n)" << endl;
    cout << "5. Petersen graph" << endl;
    cout << "Enter choice (1-5): ";
    cin >> choice;
    
    Graph graph(0);
    
    if (choice == 1) {
        int vertices, edges;
        cout << "\nEnter number of vertices: ";
        cin >> vertices;
        
        if (vertices > MAX_VERTICES) {
            cout << "Maximum " << MAX_VERTICES << " vertices allowed!" << endl;
            return 1;
        }
        
        graph = Graph(vertices);
        
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
    } else if (choice == 2) {
        int n;
        cout << "\nEnter n for complete graph Kn: ";
        cin >> n;
        graph = createCompleteGraph(n);
    } else if (choice == 3) {
        int n;
        cout << "\nEnter n for cycle graph Cn: ";
        cin >> n;
        graph = createCycleGraph(n);
    } else if (choice == 4) {
        int m, n;
        cout << "\nEnter m and n for bipartite graph Km,n: ";
        cin >> m >> n;
        graph = createBipartiteGraph(m, n);
    } else if (choice == 5) {
        graph = createPetersenGraph();
    } else {
        cout << "Invalid choice!" << endl;
        return 1;
    }
    
    printGraphInfo(graph);
    printAdjMatrix(graph);
    printEdges(graph);
    
    int mode;
    cout << "\nSelect mode:" << endl;
    cout << "1. Find chromatic number (minimum colors)" << endl;
    cout << "2. Try with specific number of colors" << endl;
    cout << "Enter choice (1-2): ";
    cin >> mode;
    
    int colors[MAX_VERTICES];
    
    if (mode == 1) {
        cout << "\nFinding chromatic number..." << endl;
        int chromatic = findChromaticNumber(graph);
        
        stats = Stats();
        graphColoring(graph, chromatic, colors);
        
        cout << "\n✓ Chromatic number: " << chromatic << endl;
        cout << "Upper bound (max degree + 1): " << graph.getMaxDegree() + 1 << endl;
        printColoredGraph(graph, colors);
        printStats();
    } else {
        int m;
        cout << "\nEnter number of colors: ";
        cin >> m;
        
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
    }
    
    return 0;
}
