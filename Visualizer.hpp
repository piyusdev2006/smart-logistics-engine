#ifndef VISUALIZER_HPP
#define VISUALIZER_HPP

#include "raylib.h"
#include "LogisticsNetworkGraph.hpp"
#include "CargoOptimizerDP.hpp"
#include "OrderPipeline.hpp"

#include <unordered_map>
#include <vector>
#include <tuple>
#include <string>
#include <cmath>

struct ScreenNode {
    int id;
    Vector2 position;
};

class Visualizer {
public:
    // Helper: Automatically calculates 2D positions for any Hub IDs in a circle/grid layout
    static std::unordered_map<int, Vector2> generateNodePositions(const std::vector<int>& allHubIDs) {
        std::unordered_map<int, Vector2> posMap;
        size_t total = allHubIDs.size();
        if (total == 0) return posMap;

        Vector2 center = { 450.0f, 200.0f };
        float radius = 130.0f;

        for (size_t i = 0; i < total; ++i) {
            float angle = (2.0f * 3.14159f * i) / total;
            float x = center.x + radius * std::cos(angle);
            float y = center.y + radius * std::sin(angle);
            posMap[allHubIDs[i]] = { x, y };
        }
        return posMap;
    }

    // 1. DYNAMIC GRAPH RENDERER
    static void drawDynamicGraph(
        const std::unordered_map<int, std::vector<Edge>>& adjList,
        const std::vector<int>& shortestPath
    ) {
        // Collect all unique Hub IDs from the actual Graph
        std::vector<int> hubIDs;
        for (const auto& pair : adjList) {
            hubIDs.push_back(pair.first);
        }

        // Generate positions dynamically
        auto posMap = generateNodePositions(hubIDs);

        // A. Draw All Edges (Roads added by user via Option 5)
        for (const auto& pair : adjList) {
            int u = pair.first;
            if (posMap.find(u) == posMap.end()) continue;

            for (const auto& edge : pair.second) {
                int v = edge.target;
                if (posMap.find(v) == posMap.end()) continue;

                // To prevent double drawing in undirected graph
                if (u < v) {
                    Vector2 start = posMap[u];
                    Vector2 end = posMap[v];

                    DrawLineEx(start, end, 3.0f, DARKGRAY);

                    // Draw weight
                    Vector2 mid = { (start.x + end.x) / 2.0f, (start.y + end.y) / 2.0f };
                    DrawText(TextFormat("%.1f km", edge.weight), mid.x - 15, mid.y - 10, 13, RAYWHITE);
                }
            }
        }

        // B. Highlight Live Dijkstra Path (Golden Line)
        if (shortestPath.size() >= 2) {
            for (size_t i = 0; i < shortestPath.size() - 1; ++i) {
                int u = shortestPath[i];
                int v = shortestPath[i + 1];
                if (posMap.count(u) && posMap.count(v)) {
                    DrawLineEx(posMap[u], posMap[v], 6.0f, GOLD);
                }
            }
        }

        // C. Draw All Hub Nodes
        for (int id : hubIDs) {
            Vector2 pos = posMap[id];
            bool isHighlighted = false;
            for (int p : shortestPath) {
                if (p == id) { isHighlighted = true; break; }
            }

            Color nodeColor = isHighlighted ? GOLD : BLUE;
            DrawCircleV(pos, 22.0f, nodeColor);
            DrawCircleLines(pos.x, pos.y, 22.0f, WHITE);

            DrawText(TextFormat("Hub %d", id), pos.x - 15, pos.y - 6, 14, WHITE);
        }
    }

    // 2. DYNAMIC TRUCK CONTAINER RENDERER
    static void drawTruckCargo(int capacity, const std::vector<Order>& selectedCargo) {
        Rectangle truckBody = { 50, 420, 800, 150 };
        DrawRectangleRec(truckBody, GetColor(0x22223bFF));
        DrawRectangleLinesEx(truckBody, 3.0f, SKYBLUE);
        
        DrawText(TextFormat("REAL-TIME TRUCK CONTAINER (Max Capacity: %d kg)", capacity), 60, 430, 16, RAYWHITE);

        if (selectedCargo.empty()) {
            DrawText("No cargo loaded in truck yet.", 60, 480, 14, GRAY);
            return;
        }

        float currentX = 60.0f;
        float startY = 460.0f;
        
        Color blockColors[] = { RED, GREEN, ORANGE, PURPLE, MAROON, LIME };
        int colorIdx = 0;

        for (const auto& item : selectedCargo) {
            // Scale box size based on actual weight / capacity
            float width = (item.weight / (float)(capacity > 0 ? capacity : 1)) * 780.0f;
            if (width < 60.0f) width = 60.0f; // Minimum visibility width

            Rectangle box = { currentX, startY, width - 5, 90.0f };

            DrawRectangleRec(box, blockColors[colorIdx % 6]);
            DrawRectangleLinesEx(box, 2.0f, WHITE);
            
            DrawText(item.sku.c_str(), box.x + 5, box.y + 15, 12, WHITE);
            DrawText(TextFormat("%dkg | $%d", item.weight, item.value), box.x + 5, box.y + 45, 12, YELLOW);

            currentX += width;
            colorIdx++;
        }
    }
};

#endif