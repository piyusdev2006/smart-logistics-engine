#ifndef LOGISTICS_NETWORK_GRAPH_HPP
#define LOGISTICS_NETWORK_GRAPH_HPP

#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <fstream>

struct Edge {
    int target;
    double weight;
};

class LogisticsNetworkGraph {
private:
    std::unordered_map<int, std::vector<Edge>> adjList;

public:
    void addRoad(int u, int v, double distance) {
        adjList[u].push_back({v, distance});
        adjList[v].push_back({u, distance});
    }

    // LogisticsNetworkGraph.hpp ke public section me include karein:
const std::unordered_map<int, std::vector<Edge>>& getAdjacencyList() const {
    return adjList;
}

    std::vector<int> findShortestPath(int start, int destination) {
        std::unordered_map<int, double> dist;
        std::unordered_map<int, int> parent;

        using QueuePair = std::pair<double, int>;
        std::priority_queue<QueuePair, std::vector<QueuePair>, std::greater<QueuePair>> pq;

        for (auto& pair : adjList) {
            dist[pair.first] = 1e9;
        }

        dist[start] = 0.0;
        pq.push({0.0, start});

        while (!pq.empty()) {
            auto [currentDist, u] = pq.top();
            pq.pop();

            if (u == destination) break;
            if (currentDist > dist[u]) continue;

            for (const auto& edge : adjList[u]) {
                if (dist[u] + edge.weight < dist[edge.target]) {
                    dist[edge.target] = dist[u] + edge.weight;
                    parent[edge.target] = u;
                    pq.push({dist[edge.target], edge.target});
                }
            }
        }

        std::vector<int> path;
        if (dist.find(destination) == dist.end() || dist[destination] == 1e9) {
            return path;
        }

        for (int curr = destination; curr != start; curr = parent[curr]) {
            path.push_back(curr);
        }
        path.push_back(start);
        std::reverse(path.begin(), path.end());
        return path;
    }

    void saveToFile(std::ofstream& outFile) {
        outFile << "[GRAPH]\n";
        size_t edgeCount = 0;
        for (auto& pair : adjList) edgeCount += pair.second.size();
        outFile << edgeCount / 2 << "\n"; // Undirected edges
        
        std::vector<std::pair<int, int>> saved;
        for (auto& pair : adjList) {
            int u = pair.first;
            for (auto& edge : pair.second) {
                int v = edge.target;
                if (u < v) {
                    outFile << u << " " << v << " " << edge.weight << "\n";
                }
            }
        }
    }

    void loadFromFile(std::ifstream& inFile) {
        size_t count;
        if (inFile >> count) {
            for (size_t i = 0; i < count; ++i) {
                int u, v;
                double dist;
                inFile >> u >> v >> dist;
                addRoad(u, v, dist);
            }
        }
    }
};

#endif