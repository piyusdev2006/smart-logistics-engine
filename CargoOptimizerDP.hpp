// CargoOptimizerDP.hpp (Dynamic Programming)

#ifndef CARGO_OPTIMIZER_DP_HPP
#define CARGO_OPTIMIZER_DP_HPP

#include <vector>
#include <algorithm>
#include <chrono>
#include <iostream>
#include "OrderPipeline.hpp"

class CargoOptimizerDP {
    private:
    // Pure Brute-Force Recursive Knapsack O(2^N)
    static int knapsackRecursive(int W, const std::vector<Order>& items, int n) {
        if (n == 0 || W == 0) return 0;
        if (items[n - 1].weight > W)
            return knapsackRecursive(W, items, n - 1);
        else
            return std::max(
                items[n - 1].value + knapsackRecursive(W - items[n - 1].weight, items, n - 1),
                knapsackRecursive(W, items, n - 1)
            );
    }

public:
    static std::vector<Order> optimizeTruckCargo(int maxWeightCapacity, const std::vector<Order>& availableOrders) {
        int n = availableOrders.size();
        std::vector<std::vector<int>> dp(n + 1, std::vector<int>(maxWeightCapacity + 1, 0));

        // Build DP Matrix
        for (int i = 1; i <= n; ++i) {
            for (int w = 0; w <= maxWeightCapacity; ++w) {
                if (availableOrders[i - 1].weight <= w) {
                    dp[i][w] = std::max(
                        dp[i - 1][w],
                        dp[i - 1][w - availableOrders[i - 1].weight] + availableOrders[i - 1].value
                    );
                } else {
                    dp[i][w] = dp[i - 1][w];
                }
            }
        }

        // Backtrack to select items
        std::vector<Order> selectedOrders;
        int w = maxWeightCapacity;
        for (int i = n; i > 0 && w > 0; --i) {
            if (dp[i][w] != dp[i - 1][w]) {
                selectedOrders.push_back(availableOrders[i - 1]);
                w -= availableOrders[i - 1].weight;
            }
        }

        return selectedOrders;
    }

    // Benchmarking comparison: Recursion vs DP
    static void runAlgorithmBenchmark(int capacity, const std::vector<Order>& items) {
        std::cout << "\n=========================================================\n";
        std::cout << "  ALGORITHMIC BENCHMARK: RECURSION VS DYNAMIC PROGRAMMING\n";
        std::cout << "=========================================================\n";
        std::cout << "Evaluating " << items.size() << " cargo items with capacity " << capacity << "kg...\n\n";

        // 1. Measure Pure Recursion O(2^N)
        auto startRec = std::chrono::high_resolution_clock::now();
        int recResult = knapsackRecursive(capacity, items, items.size());
        auto endRec = std::chrono::high_resolution_clock::now();
        auto durationRec = std::chrono::duration_cast<std::chrono::microseconds>(endRec - startRec).count();

        // 2. Measure Dynamic Programming O(N * W)
        auto startDP = std::chrono::high_resolution_clock::now();
        auto dpSelected = optimizeTruckCargo(capacity, items);
        auto endDP = std::chrono::high_resolution_clock::now();
        auto durationDP = std::chrono::duration_cast<std::chrono::microseconds>(endDP - startDP).count();

        int dpResult = 0;
        for (const auto& item : dpSelected) dpResult += item.value;

        std::cout << "[1] Brute-Force Recursion O(2^N):\n";
        std::cout << "    - Max Value Calculated: $" << recResult << "\n";
        std::cout << "    - Time Taken: " << durationRec << " microseconds\n\n";

        std::cout << "[2] Dynamic Programming O(N*W):\n";
        std::cout << "    - Max Value Calculated: $" << dpResult << "\n";
        std::cout << "    - Time Taken: " << durationDP << " microseconds\n\n";

        if (durationDP > 0 && durationRec > 0) {
            double speedup = static_cast<double>(durationRec) / durationDP;
            std::cout << "--> DP Optimization achieved a " << speedup << "x speedup over Naive Recursion!\n";
        }
        std::cout << "=========================================================\n";
    }
};

#endif