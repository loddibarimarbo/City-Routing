#include "graph.h"
#include <fstream>
#include <sstream>

void Graph::addEdge(const string& from, const string& to,
                    const string& transport,
                    double distance, double time, double cost) {
    // Add edge from -> to
    adjacencyList[from].push_back({to, transport, distance, time, cost});
    // Add edge to -> from (undirected graph)
    adjacencyList[to].push_back({from, transport, distance, time, cost});
}

void Graph::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Cannot open file " << filename << endl;
        return;
    }

    int numEdges;
    file >> numEdges;

    string from, to, transport;
    double distance, time, cost;

    for (int i = 0; i < numEdges; i++) {
        file >> from >> to >> transport >> distance >> time >> cost;
        addEdge(from, to, transport, distance, time, cost);
    }

    file.close();
    cout << "Graph loaded successfully with " << numEdges << " edges.\n";
}

void Graph::printGraph() {
    cout << "\n--- City Map ---\n";
    for (auto& node : adjacencyList) {
        cout << node.first << " -->\n";
        for (auto& edge : node.second) {
            cout << "   " << edge.destination
                 << " [" << edge.transport << "]"
                 << " dist:" << edge.distance
                 << " time:" << edge.time
                 << " cost:" << edge.cost << "\n";
        }
    }
}

vector<string> Graph::getNodes() {
    vector<string> nodes;
    for (auto& pair : adjacencyList)
        nodes.push_back(pair.first);
    return nodes;
}