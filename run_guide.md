Aapne bilkul sahi sawaal pucha hai! Visual GUI (Raylib) run hone par screen par **kab, kya aur kaisa dikhega**, iska poora stepwise visual workflow niche **System Execution Guide** ke roop me hai.

Isko aap apne project me `RUN_GUIDE.md` ya documentation ke roop me shamil kar sakte hain.

---

Here is the complete walkthrough formatted as a clean, ready-to-use **`EXECUTION_GUIDE.md`** file that you can directly save and include in your repository.

```markdown
# 🚀 Smart Logistics Engine: Step-by-Step Interactive Execution Guide

This guide provides a complete, line-by-line test script to demonstrate every functionality of the **Smart Logistics & Route Optimization Engine**. It covers backend operations (Data Structures & Algorithms), state persistence, benchmarking, and the **100% Real-Time 2D Graphical Visualizer (Raylib)**.

---

## 🏃 Launching the Application

Run the compiled executable in your terminal:

```bash
./LogisticsEngine.exe

```

---

## 📋 Interactive Step-by-Step Test Sequence

### 🔹 Step 1: Insert Orders into Priority Dispatch Pipeline (`Option 1`)

* **Data Structure:** Priority Queue (`Max-Heap`)
* **Operation:** Add two orders with different priority levels to test dynamic prioritization.

1. Select **`1`** and press **Enter**:
* **Order ID:** `101`
* **Priority Level (1-10):** `2` *(Standard Delivery)*
* **SKU Name:** `PHONE`
* **Weight (kg):** `10`
* **Value ($):** `500`


2. Select **`1`** again and press **Enter**:
* **Order ID:** `102`
* **Priority Level (1-10):** `9` *(Emergency / Medical Urgent)*
* **SKU Name:** `MEDS_OXYGEN`
* **Weight (kg):** `5`
* **Value ($):** `1200`



---

### 🔹 Step 2: Dispatch Highest-Priority Order (`Option 2`)

* **Data Structure:** Priority Queue
* **Operation:** Process the next package in line.

1. Select **`2`** and press **Enter**.
2. **Terminal Output:**
```text
--> DISPATCHED Order ID: #102 | Priority: 9 | SKU: MEDS_OXYGEN

```


*Notice how Order `#102` is dispatched first despite being inserted second, because its priority (`9`) is higher than `#101` (`2`).*

---

### 🔹 Step 3: Insert Product SKUs into Inventory (`Option 3`)

* **Data Structure:** Trie (Prefix Tree)
* **Operation:** Add structured product categories to the inventory tree.

1. Select **`3`** and press **Enter**:
* **SKU Name:** `ELECTRONICS_LAPTOP`
* **Item ID:** `501`


2. Select **`3`** again and press **Enter**:
* **SKU Name:** `ELECTRONICS_MOUSE`
* **Item ID:** `502`



---

### 🔹 Step 4: Search Inventory by Prefix (`Option 4`)

* **Data Structure:** Trie + DFS Recursion
* **Operation:** Perform an auto-complete search using a prefix string (`ELEC`).

1. Select **`4`** and press **Enter**.
2. **Enter search prefix:** `ELEC`
3. **Terminal Output:**
```text
Matching SKUs found:
  - ELECTRONICS_LAPTOP
  - ELECTRONICS_MOUSE

```



---

### 🔹 Step 5: Add a New Road Link to the Network (`Option 5`)

* **Data Structure:** Weighted Undirected Graph (Adjacency List)
* **Operation:** Dynamically extend the city graph network by creating a connection to a new node (`Hub 5`).

1. Select **`5`** and press **Enter**:
* **Source Hub ID:** `0`
* **Destination Hub ID:** `5`
* **Distance (km):** `15.0`


2. **Terminal Output:**
```text
--> Road added between Hub 0 and Hub 5 (Added to live Graph!)

```



---

### 🔹 Step 6: Compute Route & Test LRU Cache (`Option 6`)

* **Data Structure:** Graph + Dijkstra's Min-Heap + Doubly-Linked List LRU Cache
* **Operation:** Calculate the shortest route to `Hub 5` and cache the computed path.

1. Select **`6`** and press **Enter**:
* **Start Hub ID:** `0`
* **Destination Hub ID:** `5`


2. **Terminal Output:**
```text
[GRAPH COMPUTATION] Calculating path via Dijkstra...
Optimal Path: Hub 0 -> Hub 5

```


*This route calculation is now stored in the $O(1)$ LRU Cache and saved for real-time GUI highlighting.*

---

### 🔹 Step 7: Dynamic Truck Cargo Optimization (`Option 7`)

* **Data Structure:** Dynamic Programming (0/1 Knapsack)
* **Operation:** Optimize item selection for a 30kg payload limit to maximize revenue.

1. Select **`7`** and press **Enter**:
* **Truck Maximum Capacity (kg):** `30`
* **Number of items to evaluate:** `2`


2. **Item 1:** Name: `LAPTOP_BATCH` | Weight: `15` | Value: `800`
3. **Item 2:** Name: `DESK_BATCH` | Weight: `15` | Value: `500`
4. **Terminal Output:**
```text
Optimized Load:
  - LAPTOP_BATCH | Weight: 15kg | Value: $800
  - DESK_BATCH | Weight: 15kg | Value: $500
Total Value: $1300 | Total Weight: 30 / 30kg

```



---

### 🔹 Step 8: Test Operational History Stack & Undo (`Option 8`)

* **Data Structure:** Stack (`std::stack` - LIFO)
* **Operation:** Log an operational event and roll it back.

1. Select **`8`** -> Choose **`1`** (Record Action) -> Enter: `Truck_1_Dispatched`
2. Select **`8`** again -> Choose **`2`** (Undo Last Action)
3. **Terminal Output:**
```text
[STACK UNDO] Reverting operation: 'Truck_1_Dispatched'

```



---

### 🔹 Step 9: Algorithmic Performance Benchmarking (`Option 9`)

* **Operation:** Run automated microsecond timing benchmarks comparing Naive Recursion $O(2^N)$ vs Dynamic Programming $O(N \cdot W)$ on 22 heavy cargo items.

1. Select **`9`** and press **Enter**.
2. **Terminal Output:**
```text
[1] Brute-Force Recursion O(2^N): Time Taken: 45210 microseconds
[2] Dynamic Programming O(N*W):   Time Taken: 14 microseconds
--> DP Optimization achieved a 3229.28x speedup over Naive Recursion!

```



---

### 🔹 Step 10: Save System State to Disk (`Option 10`)

* **Data Structure:** File I/O (`std::fstream`)
* **Operation:** Write current Graph nodes and Trie inventory state to disk.

1. Select **`10`** and press **Enter**.
2. **Terminal Output:**
```text
--> System data saved to 'logistics_data.txt' successfully!

```



---

### 🔹 Step 11: Real-Time 2D Graphical Visualizer (`Option 11`) 🔥

* **Library:** Raylib 2D Graphics Pipeline
* **Operation:** Render the live data updated across Steps 1 to 10 in a 60 FPS interactive desktop window.

1. Select **`11`** and press **Enter**.
2. **What appears on screen:**
* **Upper Half (Graph Map):** Your dynamically added **`Hub 5`** node (from Step 5) renders automatically as a circular hub, and the calculated **`Hub 0 -> Hub 5`** path (from Step 6) glows in **Bright Gold**.
* **Lower Half (Truck Container):** The 30kg truck container (from Step 7) displays `LAPTOP_BATCH` and `DESK_BATCH` packed inside as color-coded, proportional 2D blocks.


3. **To Exit Visualizer:** Press **`ESC`** on your keyboard to close the window and safely return to the CLI menu.

---

### 🔹 Step 0: Exit System (`Option 0`)

1. Select **`0`** and press **Enter**.
2. The application saves all active states and exits cleanly.

```

```
---

Is guide ki madad se aap poore system ki working step-by-step test kar sakte hain aur apne project presentation/demo me ise seamlessly dikha sakte hain!


command to run: g++ -std=c++17 main.cpp -Iinclude -lraylib -lopengl32 -lgdi32 -lwinmm -o LogisticsEngine