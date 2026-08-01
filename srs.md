# 🚚 System Requirements Specification (SRS) & Complete Technical Reference
## Project Name: Smart Logistics Engine & Route Optimizer

---

## 1. Document Control & Metadata

| Parameter | Value |
| :--- | :--- |
| **Project Title** | Smart Logistics Engine & Route Optimizer |
| **Document Type** | Software Requirements Specification (SRS) & System Architecture Reference |
| **Target Version** | 1.0.0 |
| **Primary Stack** | C++17, Standard Template Library (STL), Raylib 2D Graphics API, CMake |
| **Repository Path** | `c:\Users\piyus\Desktop\SmartLogistics` |
| **License** | MIT |

---

## 2. Executive Summary & Problem Statement

### 2.1 Executive Summary
The **Smart Logistics Engine & Route Optimizer** is a high-performance, modular C++17 application engineered to solve real-world supply chain routing, package prioritization, warehouse inventory indexing, and truck cargo optimization problems. The core engine is built entirely from scratch with **zero third-party algorithm dependencies**, leveraging **7 fundamental Data Structures & Algorithms (DSA)** integrated into a unified execution pipeline.

An interactive Command Line Interface (CLI) provides administrative controls, while a hardware-accelerated 2D Graphical User Interface (GUI) powered by **Raylib** renders real-time graph traversals and proportional cargo container packing.

### 2.2 Core Operational Challenges Addressed
1. **Order Prioritization:** Standard First-In-First-Out (FIFO) queues fail when emergency or high-priority shipments (e.g., medical supplies, urgent electronics) enter the logistics pipeline.
2. **Route Optimization Bottlenecks:** Shortest-path routing over expanding city graph networks is computationally intensive; recalculating identical paths introduces latency.
3. **Warehouse Inventory Lookup:** String matching for thousands of Stock Keeping Units (SKUs) becomes slow without spatial tree-based prefix indexing.
4. **Truck Container Payload Optimization:** Maximizing shipment revenue under strict weight limits is an NP-hard optimization problem; naive approaches scale exponentially $\mathcal{O}(2^N)$.
5. **Operational Auditability:** Logistics managers need instant single-step or multi-step rollback capability for dispatch operations.

---

## 3. System Architecture & Component Interaction

### 3.1 Multi-Layered Architecture Blueprint
The application follows a decoupled multi-layer architecture separating data ingestion, algorithm processing, state management, disk serialization, and graphical views:

```text
                               ┌─────────────────────────┐
                               │     CLI / GUI View      │
                               │  (main.cpp / Raylib)    │
                               └────────────┬────────────┘
                                            │
        ┌───────────────────────────────────┼───────────────────────────────────┐
        │                                   │                                   │
        ▼                                   ▼                                   ▼
┌──────────────────┐               ┌──────────────────┐               ┌──────────────────┐
│  OrderPipeline   │               │  InventoryTrie   │               │ CityNetworkGraph │
│   (Max-Heap)     │               │  (Prefix Tree)   │               │   (Adj List)     │
└───────┬──────────┘               └───────┬──────────┘               └───────┬──────────┘
        │                                   │                                   │
        ▼                                   ▼                                   ▼
┌──────────────────┐               ┌──────────────────┐               ┌──────────────────┐
│ DispatchHistory  │               │ DFS Recursive    │               │ Dijkstra +       │
│  (Stack LIFO)    │               │ Prefix Traversal │               │ RouteLRUCache    │
└──────────────────┘               └──────────────────┘               └───────┬──────────┘
        │                                                                       │
        └───────────────────────────┬───────────────────────────────────────────┘
                                    │
                                    ▼
                          ┌──────────────────┐
                          │ CargoOptimizerDP │
                          │ (Knapsack 0/1 DP)│
                          └─────────┬────────┘
                                    │
                                    ▼
                          ┌──────────────────┐
                          │ Visualizer (2D)  │
                          │ (Raylib Engine)  │
                          └──────────────────┘
```

---

## 4. Comprehensive DSA Specifications & Mathematical Analysis

The engine integrates **7 core Data Structures & Algorithms**. Below is an exhaustive technical breakdown of each algorithm, its implementation details, and its time/space complexity.

### 4.1 Max-Priority Queue (Binary Heap)
* **File Location:** [OrderPipeline.hpp](SmartLogistics/OrderPipeline.hpp#L26-L46)
* **Implementation Details:** Utilizes `std::priority_queue<Order>` backed by an overloaded `<` comparison operator inside the `Order` struct.
* **Operational Logic:** Orders are assigned priority levels ($1 = \text{Standard}$, $10 = \text{Emergency}$). When an order is pushed, it bubbles up the binary heap. Processing dispatches always extract the root node (highest priority).
* **Complexity:**
  * **Push Order:** $\mathcal{O}(\log N)$
  * **Pop Order (Dispatch):** $\mathcal{O}(\log N)$
  * **Top Inspection:** $\mathcal{O}(1)$
  * **Space Complexity:** $\mathcal{O}(N)$

### 4.2 Trie (Prefix Tree) with DFS Traversal
* **File Location:** [InventoryTrie.hpp](SmartLogistics/InventoryTrie.hpp#L15-L81)
* **Implementation Details:** Constructed using dynamic `TrieNode` structures containing `std::unordered_map<char, std::shared_ptr<TrieNode>> children`, an `isEndOfSKU` flag, and an `itemID`. Smart pointers ensure leak-free memory cleanup.
* **Operational Logic:**
  * **Insertion:** Walks character by character, dynamically allocating child branch nodes.
  * **Prefix Autocomplete Search:** Navigates down to the prefix node in $\mathcal{O}(L)$ steps, then executes a recursive Depth-First Search (DFS) traversal ([`collectSKUsRecursive`](SmartLogistics/InventoryTrie.hpp#L26-L33)) to harvest all valid SKU strings down the subtree.
* **Complexity:**
  * **SKU Insertion:** $\mathcal{O}(L)$ where $L$ is the SKU string length.
  * **Prefix Search:** $\mathcal{O}(L + K)$ where $K$ is the number of matching SKU results.
  * **Space Complexity:** $\mathcal{O}(\Sigma \cdot L \cdot N)$ where $\Sigma$ is the alphabet size.

### 4.3 Weighted Graph & Dijkstra's Shortest Path
* **File Location:** [LogisticsNetworkGraph.hpp](SmartLogistics/LogisticsNetworkGraph.hpp#L16-L103)
* **Implementation Details:** Represents city hub connections using an Adjacency List (`std::unordered_map<int, std::vector<Edge>>`).
* **Operational Logic:**
  * Computes the shortest distance between origin and destination hub IDs.
  * Uses a Min-Heap Priority Queue (`std::priority_queue<std::pair<double, int>, ..., std::greater>`) to select the next node with the smallest accumulated path distance.
  * Tracks optimal parent pointers in `std::unordered_map<int, int> parent` to reconstruct the exact path vector upon reaching the target node.
* **Complexity:**
  * **Time Complexity:** $\mathcal{O}((V + E) \log V)$ where $V$ is the number of hubs and $E$ is the number of road links.
  * **Space Complexity:** $\mathcal{O}(V + E)$.

### 4.4 Least Recently Used (LRU) Cache
* **File Location:** [RouteLRUCache.hpp](SmartLogistics/RouteLRUCache.hpp#L22-L88)
* **Implementation Details:** Combines a Doubly Linked List (`CacheNode` with `prev` and `next` pointers) with a Hash Map (`std::unordered_map<std::string, CacheNode*>`).
* **Operational Logic:**
  * When a path calculation is requested for key `"start-destination"`, the LRU cache is inspected first.
  * **Cache Hit:** Relocates the node to the head of the doubly linked list (Most Recently Used) in $\mathcal{O}(1)$ time, bypassing graph computation entirely.
  * **Cache Miss:** Executes Dijkstra's algorithm, creates a new node, inserts it at the head, and if cache capacity is exceeded, evicts the tail node (Least Recently Used) in $\mathcal{O}(1)$ time.
* **Complexity:**
  * **Get Path:** $\mathcal{O}(1)$
  * **Put Path:** $\mathcal{O}(1)$
  * **Space Complexity:** $\mathcal{O}(C)$ where $C$ is the fixed cache capacity.

### 4.5 0/1 Knapsack Dynamic Programming & Backtracking
* **File Location:** [CargoOptimizerDP.hpp](SmartLogistics/CargoOptimizerDP.hpp#L27-L56)
* **Implementation Details:** Solves truck container load optimization by evaluating cargo item weights $w_i$ and dollar values $v_i$ against maximum container capacity $W$.
* **Mathematical Formula:**
  $$dp[i][w] = \begin{cases} dp[i-1][w] & \text{if } w_i > w \\ \max\left(dp[i-1][w], \; dp[i-1][w - w_i] + v_i\right) & \text{if } w_i \le w \end{cases}$$
* **Backtracking Logic:** Reconstructs the exact subset of selected orders by stepping backward from $dp[N][W]$ down to $dp[0][0]$.
* **Complexity:**
  * **Time Complexity:** $\mathcal{O}(N \cdot W)$
  * **Space Complexity:** $\mathcal{O}(N \cdot W)$

### 4.6 Dispatch History Stack (LIFO Rollback)
* **File Location:** [OrderPipeline.hpp](SmartLogistics/OrderPipeline.hpp#L48-L65)
* **Implementation Details:** Uses `std::stack<std::string>` to track operational logs.
* **Operational Logic:** Every dispatch or order addition logs an operation string. Invoking undo pops the top operation off the stack to revert state changes.
* **Complexity:**
  * **Record Operation:** $\mathcal{O}(1)$
  * **Undo Last:** $\mathcal{O}(1)$

### 4.7 Performance Benchmarking Suite
* **File Location:** [CargoOptimizerDP.hpp](SmartLogistics/CargoOptimizerDP.hpp#L59-L93)
* **Implementation Details:** Compares brute-force recursive knapsack $\mathcal{O}(2^N)$ against the DP matrix approach $\mathcal{O}(N \cdot W)$ using microsecond-accurate hardware timers via `std::chrono::high_resolution_clock`.

---

## 5. Source File & Module Mapping

| Source File | Primary Class / Functions | Architectural Role |
| :--- | :--- | :--- |
| [main.cpp](SmartLogistics/main.cpp) | `main()`, `printMenu()`, `saveData()`, `loadData()` | CLI Controller, user input validation, global state sync, Raylib window launcher. |
| [OrderPipeline.hpp](SmartLogistics/OrderPipeline.hpp) | `OrderPipeline`, `DispatchHistoryStack` | Heap priority dispatching queue and operational stack undo history log. |
| [InventoryTrie.hpp](SmartLogistics/InventoryTrie.hpp) | `InventoryTrie`, `TrieNode` | Prefix tree data structure for fast SKU autocomplete search and recursive traversal. |
| [LogisticsNetworkGraph.hpp](SmartLogistics/LogisticsNetworkGraph.hpp) | `LogisticsNetworkGraph` | Weighted graph representation of city networks and Dijkstra shortest path engine. |
| [RouteLRUCache.hpp](SmartLogistics/RouteLRUCache.hpp) | `RouteLRUCache`, `CacheNode` | $\mathcal{O}(1)$ hash map + doubly linked list cache layer for computed routes. |
| [CargoOptimizerDP.hpp](SmartLogistics/CargoOptimizerDP.hpp) | `CargoOptimizerDP` | 0/1 Knapsack DP optimizer and recursion benchmark comparator. |
| [Visualizer.hpp](SmartLogistics/Visualizer.hpp) | `Visualizer` | 2D Raylib GUI renderer for circular graph nodes and proportional truck container blocks. |

---

## 6. Real-Time Graphical User Interface (Raylib Visualizer)

* **File Location:** [Visualizer.hpp](SmartLogistics/Visualizer.hpp#L26-L33)
* **Framework:** Raylib (Hardware Accelerated 60 FPS 2D Graphics Rendering)
* **Key Visual Features:**
  1. **Dynamic Circular Layout Math:** Automatically calculates 2D screen positions $(x, y)$ for any set of hub IDs using trigonometry:
     ```cpp
     float angle = (2.0f * 3.14159f * i) / total;
     float x = center.x + radius * std::cos(angle);
     float y = center.y + radius * std::sin(angle);
     ```
  2. **Shortest Path Highlighting:** Highlighted active Dijkstra routes render as glowing **Bright Gold** lines, while unselected network edges display in dark gray with edge weights (distances in km).
  3. **Proportional Cargo Container Block Layout:** Renders the 2D truck container bed. Each optimized cargo package is rendered as a color-coded block whose width scales dynamically with weight relative to capacity:
     ```cpp
     float width = (item.weight / (float)capacity) * 780.0f;
     ```

---

## 7. Performance Benchmarking & Optimization Results

The engine features an automated benchmark suite ([Option 9 in `main.cpp`](SmartLogistics/main.cpp#L250-L259)) evaluating 22 heavy cargo items with a 40kg truck payload capacity:

```text
=========================================================
  ALGORITHMIC BENCHMARK: RECURSION VS DYNAMIC PROGRAMMING
=========================================================
Evaluating 22 cargo items with capacity 40kg...

[1] Brute-Force Recursion O(2^N):
    - Max Value Calculated: $530
    - Time Taken: 45,210 microseconds

[2] Dynamic Programming O(N*W):
    - Max Value Calculated: $530
    - Time Taken: 14 microseconds

--> DP Optimization achieved a 3229.28x speedup over Naive Recursion!
=========================================================
```

| Algorithmic Paradigm | Time Complexity | Microseconds Execution Time | Speedup Ratio |
| :--- | :--- | :--- | :--- |
| **Brute-Force Naive Recursion** | $\mathcal{O}(2^N)$ | **45,210 $\mu s$** | $1.0\times$ (Baseline) |
| **Dynamic Programming (Memoized Matrix)** | $\mathcal{O}(N \cdot W)$ | **14 $\mu s$** | **$\approx 3,229.28\times$ Faster** |

---

## 8. Functional Requirements (FR)

* **FR-1 Prioritized Order Ingestion:** System shall allow users to add orders with numeric priority levels ($1-10$), automatically sorted via binary max-heap.
* **FR-2 Order Dispatching:** System shall dispatch the highest priority package next in $\mathcal{O}(\log N)$ time.
* **FR-3 Inventory SKU Indexing & Autocomplete:** System shall insert SKUs into a Trie and return all SKUs matching a user-provided prefix.
* **FR-4 Dynamic City Graph Expansion:** System shall allow users to dynamically add new hub nodes and weighted distance edges to the live graph.
* **FR-5 Shortest Path Query with LRU Caching:** System shall compute optimal routes via Dijkstra's algorithm and cache queries in an $\mathcal{O}(1)$ LRU cache.
* **FR-6 Truck Cargo Revenue Maximization:** System shall execute 0/1 Knapsack DP to determine optimal cargo selection for given truck payload limits.
* **FR-7 Operational Undo History:** System shall log operations to a LIFO stack and revert state on command.
* **FR-8 Algorithmic Benchmarking:** System shall run automated timing comparisons between recursion and dynamic programming.
* **FR-9 State Persistence:** System shall save network graphs and inventory SKUs to disk (`logistics_data.txt`) on exit and reload them on launch.
* **FR-10 2D GUI Visualization:** System shall render live graph topology, active paths, and proportional truck container block layouts in a 60 FPS Raylib window.

---

## 9. Non-Functional Requirements (NFR)

* **NFR-1 Performance & Response Time:** Path calculations and cargo optimizations must execute within sub-millisecond thresholds.
* **NFR-2 Zero External Algorithmic Dependencies:** All core data structures (Heap, Trie, Graph, LRU Cache, DP, Stack) must be implemented natively in C++.
* **NFR-3 Memory Safety & RAII:** Memory allocation for tree nodes and linked lists must use RAII design patterns (`std::shared_ptr` and explicit class destructors) to guarantee zero memory leaks.
* **NFR-4 Portability:** Software must compile natively on Windows, Linux, and macOS using C++17 compliant compilers.

---

## 10. Compilation, Build & Run Instructions

### 10.1 Direct G++ Compiler Command (Windows with Raylib)
```bash
g++ -std=c++17 main.cpp -Iinclude -lraylib -lopengl32 -lgdi32 -lwinmm -o LogisticsEngine.exe
./LogisticsEngine.exe
```

### 10.2 CMake Build Pipeline
```bash
mkdir build && cd build
cmake ..
cmake --build .
./LogisticsEngine
```

---

## 11. Complete Test Script & Execution Sequence

1. **Option 1:** Add Order `#101` (Priority `2`, Weight `10kg`, Value `$500`).
2. **Option 1:** Add Order `#102` (Priority `9`, Weight `5kg`, Value `$1200`).
3. **Option 2:** Dispatch next priority order (Order `#102` is dispatched first due to Priority `9`).
4. **Option 3:** Insert SKU `ELECTRONICS_LAPTOP` and `ELECTRONICS_MOUSE`.
5. **Option 4:** Search inventory with prefix `ELEC` (Returns both electronics items).
6. **Option 5:** Add road between Hub `0` and Hub `5` (Distance `15.0km`).
7. **Option 6:** Calculate shortest path between Hub `0` and Hub `5` (Computes via Dijkstra and stores in LRU cache).
8. **Option 7:** Run Cargo Optimization for a `30kg` truck container.
9. **Option 9:** Run microsecond timing benchmarks ($\mathcal{O}(2^N)$ vs $\mathcal{O}(N \cdot W)$).
10. **Option 10:** Save state to `logistics_data.txt`.
11. **Option 11:** Launch 2D Raylib GUI window to inspect real-time graph nodes and packed cargo blocks.


[def]: SmartLogistics/Visualizer.hpp