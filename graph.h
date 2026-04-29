#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <limits>

using namespace std;

// Represents one edge (connection between two locations)
struct Edge {
    string destination;
    string transport;   // bus, metro, walk
    double distance;
    double time;
    double cost;
};

// Represents one node in the path result
struct PathNode {
    string location;
    string transport;
    double distance;
    double time;
    double cost;
};

class Graph {
public:
    // adjacencyList["Mirpur"] = list of edges from Mirpur
    unordered_map<string, vector<Edge>> adjacencyList;

    void addEdge(const string& from, const string& to,
                 const string& transport,
                 double distance, double time, double cost);

    void loadFromFile(const string& filename);
    void printGraph();
    vector<string> getNodes();
};

#endif