# 🚚 Smart Logistics Engine & Route Optimizer

[![C++17](https://img.shields.io/badge/Language-C%2B%2B17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![Build-Status](https://img.shields.io/badge/Build-Passing-brightgreen.svg)]()
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)]()

A high-performance, modular C++17 logistics engine that solves multi-objective optimization problems in supply chain routing, order scheduling, cargo loading, and spatial inventory searches. 

Built completely from scratch with **zero external library overhead**, this project showcases clean design patterns and practical applications of **7 fundamental Data Structures & Algorithms**.

---

## 📐 System Architecture

The core engine decouples data ingestion, spatial indexing, shortest-path calculation, caching, and state management into specialized algorithmic modules:

```text
                                [ CLIENT CLI MENU ]
                                         │
        ┌────────────────────────────────┼────────────────────────────────┐
        │                                │                                │
        ▼                                ▼                                ▼
[ OrderPipeline ]               [ InventoryTrie ]              [ LogisticsNetworkGraph ]
  Priority Queue                  Trie (Tree)                   Weighted Directed Graph
  Order Scheduling               Prefix Ingestion                      │
        │                                │                             │
        ▼                                ▼                             ▼
[ DispatchHistory ]            [ Recursive Traversals ]      [ Dijkstra + Min-Heap ]
  Stack (LIFO)                   Prefix Matching               Shortest Path Search
  Undo/Redo Log                          │                             │
                                         │                             ▼
                                         │                    [ RouteLRUCache ]
                                         │                     Doubly LL + Hash Map
                                         │                     O(1) Cached Query
                                         │                             │
                                         └──────────────┬──────────────┘
                                                        │
                                                        ▼
                                              [ CargoOptimizerDP ]
                                                0/1 Knapsack DP
                                                Truck Load Optimizer



```
## ⚡ Performance Benchmark (DP vs. Naive Recursion)

  The engine features an integrated microsecond-accurate benchmarking suite comparing pure brute-force recursive knapsack ($O(2^N)$) against the memoized DP approach ($O(N \cdot W)$) on 22 heavy cargo loads:


  ==================================================
  ALGORITHMIC BENCHMARK: RECURSION VS DYNAMIC PROGRAMMING
  ==================================================
  Evaluating 22 cargo items with capacity 40kg...

  [1] Brute-Force Recursion O(2^N):
      - Max Value Calculated: $530
      - Execution Time:       45,210 microseconds

  [2] Dynamic Programming O(N*W):
    - Max Value Calculated: $530
    - Execution Time:       14 microseconds

  --> DP Optimization achieved a 3229.28x speedup over Naive Recursion!
 ===================================================
```

## Repository structure

smart-logistics/
├── include/
│   ├── CargoOptimizerDP.hpp       # 0/1 Knapsack DP + Benchmarking Engine
│   ├── InventoryTrie.hpp          # Trie Data Structure + Recursive Search
│   ├── LogisticsNetworkGraph.hpp  # Weighted Graph + Dijkstra Implementation
│   ├── OrderPipeline.hpp          # Priority Queue (Orders) & Stack (History)
│   └── RouteLRUCache.hpp          # Doubly Linked List + Hash Map LRU Cache
├── data/
│   └── logistics_data.txt         # Serialized system state file
├── src/
│   └── main.cpp                   # Interactive CLI Controller & Storage Manager
├── CMakeLists.txt                 # Modern CMake build instructions
└── README.md