#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <fstream>
#include <tuple>

// Custom DSA Modules
#include "OrderPipeline.hpp"
#include "RouteLRUCache.hpp"
#include "InventoryTrie.hpp"
#include "LogisticsNetworkGraph.hpp"
#include "CargoOptimizerDP.hpp"

// Raylib Visualizer Module
#include "Visualizer.hpp"

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void saveData(LogisticsNetworkGraph& graph, InventoryTrie& trie) {
    std::ofstream outFile("logistics_data.txt");
    if (!outFile) {
        std::cout << "[ERROR] Unable to open file for writing.\n";
        return;
    }
    graph.saveToFile(outFile);
    trie.saveToFile(outFile);
    outFile.close();
    std::cout << "--> System data saved to 'logistics_data.txt' successfully!\n";
}

void loadData(LogisticsNetworkGraph& graph, InventoryTrie& trie) {
    std::ifstream inFile("logistics_data.txt");
    if (!inFile) {
        std::cout << "[INFO] No previous save data found. Using default initialization.\n";
        graph.addRoad(0, 1, 10.5);
        graph.addRoad(1, 2, 5.2);
        graph.addRoad(0, 2, 20.0);
        return;
    }
    std::string tag;
    while (inFile >> tag) {
        if (tag == "[GRAPH]") graph.loadFromFile(inFile);
        else if (tag == "[INVENTORY]") trie.loadFromFile(inFile);
    }
    inFile.close();
    std::cout << "--> System state successfully loaded from 'logistics_data.txt'!\n";
}

void printMenu() {
    std::cout << "\n=========================================================\n";
    std::cout << "        SMART LOGISTICS MANAGEMENT SYSTEM (CLI)          \n";
    std::cout << "=========================================================\n";
    std::cout << " 1. Add Order to Priority Dispatch Pipeline\n";
    std::cout << " 2. Process / Dispatch Next Priority Order\n";
    std::cout << " 3. Add SKU to Inventory (Trie)\n";
    std::cout << " 4. Search Inventory by Prefix (Trie + Recursion)\n";
    std::cout << " 5. Add Road Link to Logistics Network (Graph)\n";
    std::cout << " 6. Find Shortest Route (Dijkstra + LRU Cache)\n";
    std::cout << " 7. Optimize Truck Cargo Load (0/1 Knapsack DP)\n";
    std::cout << " 8. Record Operation / Undo Operation (Stack History)\n";
    std::cout << " 9. Run Benchmark: Brute-Force Recursion vs DP\n";
    std::cout << " 10. Save System State to Disk\n";
    std::cout << " 11. Launch 2D Graphical GUI (Raylib Window)\n";
    std::cout << " 0. Exit System\n";
    std::cout << "=========================================================\n";
    std::cout << "Select an option [0-11]: ";
}

int main() {
    OrderPipeline pipeline;
    InventoryTrie inventory;
    LogisticsNetworkGraph cityNetwork;
    RouteLRUCache routeCache(5);
    DispatchHistoryStack history;

    // Global state variables for GUI sync
    std::vector<int> lastDijkstraPath;
    int lastTruckCapacity = 30;
    std::vector<Order> lastLoadedCargo;

    // Load existing state
    loadData(cityNetwork, inventory);

    int choice = -1;

    while (true) {
        printMenu();
        if (!(std::cin >> choice)) {
            std::cout << "\n[ERROR] Invalid input. Please enter a number.\n";
            clearInput();
            continue;
        }

        if (choice == 0) {
            saveData(cityNetwork, inventory);
            std::cout << "\nExiting Logistics System. Goodbye!\n";
            break;
        }

        switch (choice) {
            case 1: {
                Order newOrder;
                std::cout << "\n--- Add New Order ---\n";
                std::cout << "Enter Order ID: "; std::cin >> newOrder.id;
                std::cout << "Enter Priority (1-10): "; std::cin >> newOrder.priority;
                std::cout << "Enter SKU Name: "; std::cin >> newOrder.sku;
                std::cout << "Enter Weight (kg): "; std::cin >> newOrder.weight;
                std::cout << "Enter Value ($): "; std::cin >> newOrder.value;

                pipeline.pushOrder(newOrder);
                history.recordOperation("Added Order #" + std::to_string(newOrder.id));
                std::cout << "--> Order #" << newOrder.id << " added!\n";
                break;
            }

            case 2: {
                std::cout << "\n--- Dispatch Priority Order ---\n";
                if (pipeline.empty()) {
                    std::cout << "[INFO] Queue empty.\n";
                } else {
                    Order dispatched = pipeline.popNextOrder();
                    std::cout << "--> DISPATCHED Order ID: #" << dispatched.id 
                              << " | Priority: " << dispatched.priority 
                              << " | SKU: " << dispatched.sku << "\n";
                    history.recordOperation("Dispatched Order #" + std::to_string(dispatched.id));
                }
                break;
            }

            case 3: {
                std::string sku; int id;
                std::cout << "\n--- Add SKU to Inventory ---\n";
                std::cout << "Enter SKU Name: "; std::cin >> sku;
                std::cout << "Enter Item ID: "; std::cin >> id;

                inventory.insertSKU(sku, id);
                std::cout << "--> SKU '" << sku << "' inserted into Trie.\n";
                break;
            }

            case 4: {
                std::string prefix;
                std::cout << "\n--- Search Inventory (Prefix) ---\n";
                std::cout << "Enter search prefix: "; std::cin >> prefix;

                auto results = inventory.searchByPrefix(prefix);
                if (results.empty()) std::cout << "[INFO] No SKUs found.\n";
                else {
                    std::cout << "Matching SKUs found:\n";
                    for (const auto& sku : results) std::cout << "  - " << sku << "\n";
                }
                break;
            }

            case 5: {
                int u, v; double dist;
                std::cout << "\n--- Add Road to Graph Network ---\n";
                std::cout << "Source Hub ID: "; std::cin >> u;
                std::cout << "Destination Hub ID: "; std::cin >> v;
                std::cout << "Distance: "; std::cin >> dist;

                cityNetwork.addRoad(u, v, dist);
                std::cout << "--> Road added between Hub " << u << " and Hub " << v << " (Added to live Graph!)\n";
                break;
            }

            case 6: {
                int start, dest;
                std::cout << "\n--- Find Shortest Route ---\n";
                std::cout << "Start Hub ID: "; std::cin >> start;
                std::cout << "Destination Hub ID: "; std::cin >> dest;

                std::string cacheKey = std::to_string(start) + "-" + std::to_string(dest);
                std::vector<int> path;

                if (routeCache.get(cacheKey, path)) {
                    std::cout << "[LRU CACHE HIT] Retrieved path from Cache!\n";
                } else {
                    std::cout << "[GRAPH COMPUTATION] Calculating path via Dijkstra...\n";
                    path = cityNetwork.findShortestPath(start, dest);
                    if (!path.empty()) routeCache.put(cacheKey, path);
                }

                if (path.empty()) std::cout << "[ERROR] Path not found.\n";
                else {
                    lastDijkstraPath = path; // Save for Live GUI!
                    std::cout << "Optimal Path: ";
                    for (size_t i = 0; i < path.size(); ++i) {
                        std::cout << "Hub " << path[i] << (i + 1 < path.size() ? " -> " : "");
                    }
                    std::cout << "\n";
                }
                break;
            }

            case 7: {
                int capacity, count;
                std::cout << "\n--- Truck Cargo Optimization (Knapsack DP) ---\n";
                std::cout << "Truck Maximum Capacity (kg): "; std::cin >> capacity;
                std::cout << "Number of items to evaluate: "; std::cin >> count;

                std::vector<Order> items;
                for (int i = 0; i < count; ++i) {
                    Order item;
                    item.id = i + 1;
                    std::cout << "  Item #" << (i + 1) << " Name: "; std::cin >> item.sku;
                    std::cout << "  Item #" << (i + 1) << " Weight (kg): "; std::cin >> item.weight;
                    std::cout << "  Item #" << (i + 1) << " Value ($): "; std::cin >> item.value;
                    items.push_back(item);
                }

                auto selected = CargoOptimizerDP::optimizeTruckCargo(capacity, items);
                lastTruckCapacity = capacity;
                lastLoadedCargo = selected; // Save for Live GUI!

                std::cout << "\nOptimized Load:\n";
                int totalVal = 0, totalWt = 0;
                for (const auto& item : selected) {
                    std::cout << "  - " << item.sku << " | Weight: " << item.weight << "kg | Value: $" << item.value << "\n";
                    totalVal += item.value;
                    totalWt += item.weight;
                }
                std::cout << "Total Value: $" << totalVal << " | Total Weight: " << totalWt << " / " << capacity << "kg\n";
                break;
            }

            case 8: {
                int subChoice;
                std::cout << "\n--- Operational History Stack ---\n";
                std::cout << "1. Record Custom Action\n2. Undo Last Action\nSelect [1 or 2]: ";
                std::cin >> subChoice;

                if (subChoice == 1) {
                    std::string action;
                    std::cout << "Enter action: ";
                    clearInput();
                    std::getline(std::cin, action);
                    history.recordOperation(action);
                    std::cout << "--> Action recorded.\n";
                } else if (subChoice == 2) {
                    history.undoLast();
                }
                break;
            }

            case 9: {
                int capacity = 40;
                std::cout << "\nRunning dynamic benchmarking with 22 test orders...\n";
                std::vector<Order> testOrders;
                for (int i = 1; i <= 22; ++i) {
                    testOrders.push_back({i, 1, "ITEM_" + std::to_string(i), (i % 7) + 2, (i * 15) % 100 + 10});
                }
                CargoOptimizerDP::runAlgorithmBenchmark(capacity, testOrders);
                break;
            }

            case 10: {
                saveData(cityNetwork, inventory);
                break;
            }

            // =========================================================================
            // OPTION 11: 100% REAL-TIME DYNAMIC RAYLIB GUI
            // =========================================================================
            case 11: {
                std::cout << "\n[INFO] Launching 100% Real-Time 2D Raylib Graphical Window...\n";

                const int screenWidth = 900;
                const int screenHeight = 650;
                InitWindow(screenWidth, screenHeight, "Smart Logistics Engine - Real-Time Visualizer");
                SetTargetFPS(60);

                // Fetch LIVE Adjacency List from the actual Graph Object
                const auto& liveAdjList = cityNetwork.getAdjacencyList();

                // Render Loop
                while (!WindowShouldClose()) {
                    BeginDrawing();
                    ClearBackground(GetColor(0x181818FF)); // Dark Theme

                    DrawText("LIVE LOGISTICS GRAPH & CARGO VISUALIZER", 20, 20, 18, RAYWHITE);

                    // 1. Draw Real-Time Graph (Renders ANY hubs/roads added in Option 5)
                    Visualizer::drawDynamicGraph(liveAdjList, lastDijkstraPath);

                    // 2. Draw Real-Time Cargo (Renders exact truck capacity & items from Option 7)
                    Visualizer::drawTruckCargo(lastTruckCapacity, lastLoadedCargo);

                    DrawText("Press ESC or Close Window to return to CLI Menu", 20, 610, 14, GRAY);
                    EndDrawing();
                }

                CloseWindow();
                std::cout << "[INFO] Returned to CLI Menu.\n";
                break;
            }

            default:
                std::cout << "\n[ERROR] Option invalid. Choose between 0 and 11.\n";
                break;
        }
    }

    return 0;
}