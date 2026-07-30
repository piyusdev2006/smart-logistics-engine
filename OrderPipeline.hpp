// OrderPipeline.hpp (Queue & Stack)


#ifndef ORDER_PIPELINE_HPP
#define ORDER_PIPELINE_HPP

#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <stdexcept>

struct Order {
    int id;
    int priority; // Higher value = urgent
    std::string sku;
    int weight;
    int value;

    // Overload comparison operator for Priority Queue (Max-Heap)
    bool operator<(const Order& other) const {
        return this->priority < other.priority;
    }
};

class OrderPipeline {
private:
    std::priority_queue<Order> priorityDispatchQueue;

public:
    void pushOrder(const Order& order) {
        priorityDispatchQueue.push(order);
    }

    Order popNextOrder() {
        if (priorityDispatchQueue.empty()) {
            throw std::runtime_error("Order queue is empty!");
        }
        Order topOrder = priorityDispatchQueue.top();
        priorityDispatchQueue.pop();
        return topOrder;
    }

    bool empty() const { return priorityDispatchQueue.empty(); }
    size_t size() const { return priorityDispatchQueue.size(); }
};

class DispatchHistoryStack {
private:
    std::stack<std::string> history;

public:
    void recordOperation(const std::string& op) {
        history.push(op);
    }

    void undoLast() {
        if (history.empty()) {
            std::cout << "[STACK UNDO] No operations to undo.\n";
            return;
        }
        std::cout << "[STACK UNDO] Reverting operation: '" << history.top() << "'\n";
        history.pop();
    }
};

#endif