#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <chrono>
using namespace std;
using namespace chrono;

#define MAX_VERTICES 20

// Statistics structure
struct Stats {
    int recursionCalls = 0;
    int backtrackCount = 0;
    int safetyChecks = 0;
    int colorAssignments = 0;
    long long executionTime = 0;
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
    for (int i = 0; i < 5; i++) {
        g.addEdge(i, (i + 1) % 5);
    }
    for (int i = 0; i < 5; i++) {
        g.addEdge(i + 5, (i + 2) % 5 + 5);
    }
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

// Count colors used
int countColors(int colors[], int n) {
    int maxColor = 0;
    for (int i = 0; i < n; i++) {
        maxColor = max(maxColor, colors[i]);
    }
    return maxColor;
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

// Recursive graph coloring (backtracking)
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

// Backtracking coloring
bool graphColoring(Graph &graph, int m, int colors[]) {
    for (int i = 0; i < graph.vertices; i++) {
        colors[i] = 0;
    }
    
    if (!graphColoringUtil(graph, m, colors, 0)) {
        return false;
    }
    
    return true;
}

// Greedy coloring
int greedyColoring(Graph &graph, int colors[]) {
    for (int i = 0; i < graph.vertices; i++) {
        colors[i] = 0;
    }
    
    colors[0] = 1;
    
    for (int u = 1; u < graph.vertices; u++) {
        bool available[MAX_VERTICES];
        for (int i = 0; i < MAX_VERTICES; i++) {
            available[i] = true;
        }
        
        for (int i = 0; i < graph.vertices; i++) {
            if (graph.adjMatrix[u][i] && colors[i] != 0) {
                available[colors[i]] = false;
            }
        }
        
        int color;
        for (color = 1; color < MAX_VERTICES; color++) {
            if (available[color]) break;
        }
        
        colors[u] = color;
    }
    
    return countColors(colors, graph.vertices);
}

// Welsh-Powell algorithm
int welshPowell(Graph &graph, int colors[]) {
    vector<pair<int, int>> degreeVertex;
    
    for (int i = 0; i < graph.vertices; i++) {
        degreeVertex.push_back({graph.getDegree(i), i});
    }
    
    sort(degreeVertex.rbegin(), degreeVertex.rend());
    
    for (int i = 0; i < graph.vertices; i++) {
        colors[i] = 0;
    }
    
    int currentColor = 1;
    
    for (int i = 0; i < graph.vertices; i++) {
        int vertex = degreeVertex[i].second;
        
        if (colors[vertex] == 0) {
            colors[vertex] = currentColor;
            
            for (int j = i + 1; j < graph.vertices; j++) {
                int v = degreeVertex[j].second;
                
                if (colors[v] == 0) {
                    bool canColor = true;
                    
                    for (int k = 0; k < graph.vertices; k++) {
                        if (graph.adjMatrix[v][k] && colors[k] == currentColor) {
                            canColor = false;
                            break;
                        }
                    }
                    
                    if (canColor) {
                        colors[v] = currentColor;
                    }
                }
            }
            
            currentColor++;
        }
    }
    
    return countColors(colors, graph.vertices);
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
void printStats(string algorithm = "") {
    cout << "\n========================================" << endl;
    cout << "           Statistics";
    if (!algorithm.empty()) cout << " (" << algorithm << ")";
    cout << endl;
    cout << "========================================" << endl;
    cout << "Recursion calls: " << stats.recursionCalls << endl;
    cout << "Backtrack operations: " << stats.backtrackCount << endl;
    cout << "Safety checks: " << stats.safetyChecks << endl;
    cout << "Color assignments: " << stats.colorAssignments << endl;
    if (stats.executionTime > 0) {
        cout << "Execution time: " << stats.executionTime << " μs" << endl;
    }
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
    cout << "\nSelect algorithm:" << endl;
    cout << "1. Backtracking (exact, finds chromatic number)" << endl;
    cout << "2. Greedy coloring (fast, approximate)" << endl;
    cout << "3. Welsh-Powell (degree-based, approximate)" << endl;
    cout << "4. Compare all algorithms" << endl;
    cout << "Enter choice (1-4): ";
    cin >> mode;
    
    int colors[MAX_VERTICES];
    
    if (mode == 1) {
        cout << "\nFinding chromatic number..." << endl;
        
        auto start = high_resolution_clock::now();
        int chromatic = findChromaticNumber(graph);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        
        stats = Stats();
        stats.executionTime = duration.count();
        graphColoring(graph, chromatic, colors);
        
        cout << "\n✓ Chromatic number: " << chromatic << endl;
        cout << "Upper bound (max degree + 1): " << graph.getMaxDegree() + 1 << endl;
        printColoredGraph(graph, colors);
        printStats("Backtracking");
    } else if (mode == 2) {
        cout << "\nApplying greedy coloring..." << endl;
        
        auto start = high_resolution_clock::now();
        int colorsUsed = greedyColoring(graph, colors);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        
        cout << "\n✓ Colors used: " << colorsUsed << endl;
        printColoredGraph(graph, colors);
        
        stats.executionTime = duration.count();
        printStats("Greedy");
    } else if (mode == 3) {
        cout << "\nApplying Welsh-Powell algorithm..." << endl;
        
        auto start = high_resolution_clock::now();
        int colorsUsed = welshPowell(graph, colors);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        
        cout << "\n✓ Colors used: " << colorsUsed << endl;
        printColoredGraph(graph, colors);
        
        stats.executionTime = duration.count();
        printStats("Welsh-Powell");
    } else if (mode == 4) {
        cout << "\n========================================" << endl;
        cout << "    Algorithm Comparison               " << endl;
        cout << "========================================" << endl;
        
        int colors1[MAX_VERTICES], colors2[MAX_VERTICES], colors3[MAX_VERTICES];
        
        auto start1 = high_resolution_clock::now();
        int chromatic = findChromaticNumber(graph);
        auto end1 = high_resolution_clock::now();
        auto duration1 = duration_cast<microseconds>(end1 - start1);
        
        stats = Stats();
        graphColoring(graph, chromatic, colors1);
        
        auto start2 = high_resolution_clock::now();
        int greedy = greedyColoring(graph, colors2);
        auto end2 = high_resolution_clock::now();
        auto duration2 = duration_cast<microseconds>(end2 - start2);
        
        auto start3 = high_resolution_clock::now();
        int welsh = welshPowell(graph, colors3);
        auto end3 = high_resolution_clock::now();
        auto duration3 = duration_cast<microseconds>(end3 - start3);
        
        cout << "\nResults:" << endl;
        cout << left << setw(20) << "Algorithm" << setw(15) << "Colors Used" << "Time (μs)" << endl;
        cout << string(50, '-') << endl;
        cout << setw(20) << "Backtracking" << setw(15) << chromatic << duration1.count() << endl;
        cout << setw(20) << "Greedy" << setw(15) << greedy << duration2.count() << endl;
        cout << setw(20) << "Welsh-Powell" << setw(15) << welsh << duration3.count() << endl;
        
        cout << "\nOptimality:" << endl;
        cout << "  Backtracking: Optimal (chromatic number)" << endl;
        cout << "  Greedy: " << (greedy == chromatic ? "Optimal!" : to_string(greedy - chromatic) + " extra colors") << endl;
        cout << "  Welsh-Powell: " << (welsh == chromatic ? "Optimal!" : to_string(welsh - chromatic) + " extra colors") << endl;
        cout << "========================================" << endl;
    }
    
    return 0;
}
