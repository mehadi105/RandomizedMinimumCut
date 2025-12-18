#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <fstream>

using namespace std;

struct Edge {
    int src, dest;
};

struct Graph {
    int V, E;
    vector<Edge> edge;
};

int kargerMinCut(Graph* graph) {
    int V = graph->V, E = graph->E;
    vector<Edge> edges = graph->edge;

    vector<int> parent(V);
    for (int i = 0; i < V; ++i) {
        parent[i] = i;
    }

    int vertices = V;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, E - 1);

    while (vertices > 2) {
        int i = dis(gen);
        
        int src = edges[i].src;
        int dest = edges[i].dest;
        
        while (parent[src] != src) src = parent[src];
        while (parent[dest] != dest) dest = parent[dest];

        if (src == dest) continue;

        for (int j = 0; j < V; ++j) {
            if (parent[j] == dest) {
                parent[j] = src;
            }
        }

        vector<Edge> newEdges;
        for (int j = 0; j < E; ++j) {
            int u = edges[j].src;
            int v = edges[j].dest;
            
            while (parent[u] != u) u = parent[u];
            while (parent[v] != v) v = parent[v];
            
            if (u != v) {
                newEdges.push_back({u, v});
            }
        }
        edges = newEdges;
        E = edges.size();
        
        if (E > 0) {
            dis = uniform_int_distribution<>(0, E - 1);
        }

        --vertices;
    }

    int cutEdges = 0;
    for (int i = 0; i < E; ++i) {
        if (parent[edges[i].src] != parent[edges[i].dest]) {
            cutEdges++;
        }
    }

    return cutEdges;
}

Graph* createGraph(int V, int E) {
    Graph* graph = new Graph;
    graph->V = V;
    graph->E = E;
    return graph;
}

int main() {
    ifstream inFile("input.txt");
    ofstream outFile("output.txt");
    
    if (!inFile.is_open()) {
        cerr << "Error: Cannot open input.txt" << endl;
        return 1;
    }
    
    if (!outFile.is_open()) {
        cerr << "Error: Cannot open output.txt" << endl;
        return 1;
    }
    
    int V, E;
    inFile >> V >> E;
    
    Graph* graph = createGraph(V, E);
    
    for (int i = 0; i < E; i++) {
        int src, dest;
        inFile >> src >> dest;
        graph->edge.push_back({src, dest});
    }
    
    inFile.close();
    
    for (int run = 1; run <= 10; run++) {
        int result = kargerMinCut(graph);
        outFile << "Run " << run << ": " << result << endl;
    }
    
    outFile.close();

    return 0;
}
