#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <cstdlib>
#include <sstream>
using namespace std;

struct Node {
    int id;
    int weight;
    vector<int> neighbors;
    vector<Edge*> edges;
};

struct Edge {
    int u, v;
    double weight;
    Edge(int u, int v, double weight) : u(u), v(v), weight(weight) {}
};

double edgeWeight(const Node& u, const Node& v) {
    int commonNeighbors = 0;
    unordered_set<int> uNeighbors(u.neighbors.begin(), u.neighbors.end());
    for (int neighbor : v.neighbors) {
        if (uNeighbors.count(neighbor)) {
            ++commonNeighbors;
        }
    }
    return commonNeighbors;
}

void coarsenGraph(vector<Node>& nodes, vector<Edge>& edges, vector<Node>& coarseNodes, vector<Edge>& coarseEdges) {
    unordered_map<int, int> mapping;
    int coarseNodeId = 0;

    // Sort edges by weight in descending order
    sort(edges.begin(), edges.end(), [](const Edge& e1, const Edge& e2) {
        return e1.weight > e2.weight;
    });

    for (const Edge& e : edges) {
        int u = mapping.count(e.u) ? mapping[e.u] : e.u;
        int v = mapping.count(e.v) ? mapping[e.v] : e.v;

        if (u != v) {
            // Contract nodes u and v into a new coarse node
            Node coarseNode;
            coarseNode.id = coarseNodeId++;
            coarseNode.weight = nodes[u].weight + nodes[v].weight;

            // Update neighbor lists and edge lists
            for (int neighbor : nodes[u].neighbors) {
                if (neighbor != v) {
                    coarseNode.neighbors.push_back(mapping.count(neighbor) ? mapping[neighbor] : neighbor);
                    coarseNode.edges.push_back(nullptr); // Placeholder for new edge
                }
            }
            for (int neighbor : nodes[v].neighbors) {
                if (neighbor != u && !count(coarseNode.neighbors.begin(), coarseNode.neighbors.end(), mapping.count(neighbor) ? mapping[neighbor] : neighbor)) {
                    coarseNode.neighbors.push_back(mapping.count(neighbor) ? mapping[neighbor] : neighbor);
                    coarseNode.edges.push_back(nullptr); // Placeholder for new edge
                }
            }

            coarseNodes.push_back(coarseNode);
            mapping[u] = mapping[v] = coarseNode.id;
        }
    }

    // Create new edges for the coarse graph
    for (Node& coarseNode : coarseNodes) {
        for (int i = 0; i < coarseNode.neighbors.size(); ++i) {
            int neighbor = coarseNode.neighbors[i];
            double edgeWt = edgeWeight(coarseNode, coarseNodes[neighbor]);
            coarseEdges.push_back(Edge(coarseNode.id, neighbor, edgeWt));
            coarseNode.edges[i] = &coarseEdges.back();
        }
    }
}

bool isBalanced(const vector<int>& partitions) {
    int n = partitions.size();
    int numNodes0 = 0, numNodes1 = 0;

    for (int partition : partitions) {
        if (partition == 0) {
            ++numNodes0;
        } else {
            ++numNodes1;
        }
    }

    double ratio0 = static_cast<double>(numNodes0) / n;
    double ratio1 = static_cast<double>(numNodes1) / n;

    return (ratio0 >= 0.45 && ratio0 <= 0.55) && (ratio1 >= 0.45 && ratio1 <= 0.55);
}

void initialPartitioning(const vector<Node>& nodes, vector<int>& partitions) {
    int n = nodes.size();
    vector<double> gains(n, 0.0);
    vector<bool> moved(n, false);

    // Initialize partitions randomly
    for (int i = 0; i < n; ++i) {
        partitions[i] = rand() % 2;
    }

    // Compute initial gains
    for (const Edge& e : edges) {
        int u = e.u, v = e.v;
        double weight = e.weight;
        if (partitions[u] != partitions[v]) {
            gains[u] += (partitions[u] == 0) ? weight : -weight;
            gains[v] += (partitions[v] == 1) ? weight : -weight;
        }
    }

    bool improved;
    do {
        improved = false;
        vector<pair<double, int>> sortedGains(gains.begin(), gains.end());
        sort(sortedGains.begin(), sortedGains.end(), greater<pair<double, int>>());

        for (const auto& pair : sortedGains) {
            int node = pair.second;
            double gain = pair.first;
            if (gain != 0 && !moved[node]) {
                int oldPartition = partitions[node];
                partitions[node] = 1 - partitions[node];
                moved[node] = true;
                if (isBalanced(partitions)) {
                    improved = true;
                    for (const Edge& e : edges) {
                        int u = e.u, v = e.v;
                        double weight = e.weight;
                        if (u == node || v == node) {
                            gains[u] += (partitions[u] == 0) ? weight : -weight;
                            gains[v] += (partitions[v] == 1) ? weight : -weight;
                        }
                    }
                } else {
                    partitions[node] = oldPartition;
                }
            }
        }
        fill(moved.begin(), moved.end(), false);
    } while (improved);
}

void refinePartitioning(const vector<Node>& nodes, const vector<Edge>& edges, vector<int>& partitions) {
    int maxPasses = nodes.size();
    bool improved;
    do {
        improved = false;
        for (int pass = 0; pass < maxPasses; ++pass) {
            vector<double> gains(nodes.size(), 0.0);
            for (const Edge& e : edges) {
                int u = e.u, v = e.v;
                if (partitions[u] != partitions[v]) {
                    double gain = e.weight;
                    gains[u] += (partitions[u] == 0) ? gain : -gain;
                    gains[v] += (partitions[v] == 1) ? gain : -gain;
                }
            }

            vector<pair<double, int>> sortedGains(nodes.size());
            for (int i = 0; i < nodes.size(); ++i) {
                sortedGains[i] = make_pair(gains[i], i);
            }
            sort(sortedGains.begin(), sortedGains.end(), greater<pair<double, int>>());

            for (const auto& pair : sortedGains) {
                int node = pair.second;
                double gain = pair.first;
                if (gain != 0) {
                    int oldPartition = partitions[node];
                    partitions[node] = 1 - partitions[node];
                    if (isBalanced(partitions)) {
                        improved = true;
                        break;
                    } else {
                        partitions[node] = oldPartition;
                    }
                }
            }
        }
    } while (improved);
}