#include "graph.h"
#include <queue>
#include <unordered_map>
#include <chrono>
#include <algorithm>    // ← add this (for reverse)
#include <tuple>        // ← add this (for tuple return)
#include <limits>       // ← add this (for numeric_limits)

// Mode: 0=distance, 1=time, 2=cost, 3=custom
double getWeight(const Edge& e, int mode, double w1=1, double w2=1, double w3=1) {
    if (mode == 0) return e.distance;
    if (mode == 1) return e.time;
    if (mode == 2) return e.cost;
    // Custom: weighted combination
    return w1 * e.distance + w2 * e.time + w3 * e.cost;
}

// Returns {total_weight, execution_time_ms, path_details}
tuple<double, double, vector<PathNode>> dijkstra(
    Graph& graph,
    const string& source,
    const string& destination,
    int mode,
    const string& avoidTransport = "",  // e.g. "bus" to avoid buses
    const string& avoidNode = "",        // e.g. "Shahbag" to avoid
    double w1=1, double w2=1, double w3=1)
{
    // Start timer
    auto start = chrono::high_resolution_clock::now();

    // dist[node] = best known weight to reach node
    unordered_map<string, double> dist;
    // prev[node] = {previous_node, edge used}
    unordered_map<string, pair<string, Edge>> prev;

    // Initialize all distances to infinity
    for (auto& node : graph.adjacencyList)
        dist[node.first] = numeric_limits<double>::infinity();
    dist[source] = 0;

    // Priority queue: {weight, node_name}
    priority_queue<pair<double,string>,
                   vector<pair<double,string>>,
                   greater<>> pq;
    pq.push({0, source});

    while (!pq.empty()) {
        auto [currDist, currNode] = pq.top();
        pq.pop();

        // Skip if we found a better path already
        if (currDist > dist[currNode]) continue;
        // Stop early if destination reached
        if (currNode == destination) break;

        for (auto& edge : graph.adjacencyList[currNode]) {
            // Skip avoided transport type
            if (!avoidTransport.empty() && edge.transport == avoidTransport) continue;
            // Skip avoided node
            if (!avoidNode.empty() && edge.destination == avoidNode) continue;

            double weight = getWeight(edge, mode, w1, w2, w3);
            double newDist = dist[currNode] + weight;

            if (newDist < dist[edge.destination]) {
                dist[edge.destination] = newDist;
                prev[edge.destination] = {currNode, edge};
                pq.push({newDist, edge.destination});
            }
        }
    }

    // Stop timer
    auto end = chrono::high_resolution_clock::now();
    double elapsed = chrono::duration<double, milli>(end - start).count();

    // Reconstruct path
    vector<PathNode> path;
    if (dist[destination] == numeric_limits<double>::infinity()) {
        return {-1, elapsed, path}; // No path found
    }

    string curr = destination;
    while (curr != source) {
        auto& [parent, edge] = prev[curr];
        path.push_back({curr, edge.transport, edge.distance, edge.time, edge.cost});
        curr = parent;
    }
    path.push_back({source, "", 0, 0, 0});
    reverse(path.begin(), path.end());

    return {dist[destination], elapsed, path};
}