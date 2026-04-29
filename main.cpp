#include "graph.h"
#include <tuple>
#include <iomanip>
#include <algorithm>
#include <string>

// Forward declaration
tuple<double, double, vector<PathNode>> dijkstra(
    Graph&, const string&, const string&, int,
    const string&, const string&,
    double, double, double);

void printPath(const vector<PathNode>& path, int mode) {
    string unit;
    if (mode == 0) unit = "km";
    else if (mode == 1) unit = "min";
    else unit = "taka";

    cout << "\n--- Route ---\n";
    double totalDist = 0, totalTime = 0, totalCost = 0;

    for (int i = 0; i < (int)path.size(); i++) {
        cout << path[i].location;
        if (i < (int)path.size() - 1) {
            cout << " --[" << path[i+1].transport << "]--> ";
            totalDist += path[i+1].distance;
            totalTime += path[i+1].time;
            totalCost += path[i+1].cost;
        }
    }
    cout << "\n\nTotal Distance : " << fixed << setprecision(1) << totalDist << " km";
    cout << "\nTotal Time     : " << totalTime << " min";
    cout << "\nTotal Cost     : " << totalCost << " taka\n";
}

int main() {
    Graph graph;
    graph.loadFromFile("city_map.txt");

    cout << "\n=== Adaptive City Routing System ===\n";

    int choice;
    do {
        cout << "\n1. Find Route\n2. Show Map\n3. Compare All Modes\n0. Exit\nChoice: ";
        cin >> choice;

        if (choice == 1) {

            string src, dst, avoidT, avoidN;
            cout << "Source: ";
            cin >> src;
            cout << "Destination: ";
            cin >> dst;
            cout << "Avoid transport (bus/metro/walk or none): ";
            cin >> avoidT;
            cout << "Avoid node (location name or none): ";
            cin >> avoidN;

            if (avoidT == "none") avoidT = "";
            if (avoidN == "none") avoidN = "";

            cout << "\nMode: 1.Distance  2.Time  3.Cost  4.Custom\nMode: ";
            int mode;
            cin >> mode;
            mode--;

            double w1 = 1.0, w2 = 1.0, w3 = 1.0;
            if (mode == 3) {
                cout << "Enter weights for (distance time cost): ";
                cin >> w1 >> w2 >> w3;
            }

            tuple<double, double, vector<PathNode>> r;
            r = dijkstra(graph, src, dst, mode, avoidT, avoidN, w1, w2, w3);

            double total = get<0>(r);
            double ms    = get<1>(r);
            vector<PathNode> path = get<2>(r);

            if (total < 0) {
                cout << "\nNo path found between " << src << " and " << dst << "!\n";
            } else {
                printPath(path, mode);
                cout << "Execution time: " << fixed << setprecision(4) << ms << " ms\n";
            }

        } else if (choice == 2) {

            graph.printGraph();

        } else if (choice == 3) {

            string src, dst;
            cout << "Source: ";
            cin >> src;
            cout << "Destination: ";
            cin >> dst;

            string modeNames[3] = {"Distance", "Time", "Cost"};
            string unitNames[3] = {"km", "min", "taka"};

            cout << "\n--- Performance Comparison ---\n";
            cout << setw(12) << "Mode"
                 << setw(12) << "Result"
                 << setw(14) << "Time(ms)\n";
            cout << string(40, '-') << "\n";

            for (int m = 0; m < 3; m++) {
                string noAvoid = "";
                tuple<double, double, vector<PathNode>> r2;
                r2 = dijkstra(graph, src, dst, m, noAvoid, noAvoid, 1.0, 1.0, 1.0);

                double total2 = get<0>(r2);
                double ms2    = get<1>(r2);

                cout << setw(12) << modeNames[m]
                     << setw(8)  << fixed << setprecision(1) << total2
                     << " " << unitNames[m]
                     << setw(10) << setprecision(4) << ms2 << " ms\n";
            }
        }

    } while (choice != 0);

    return 0;
}