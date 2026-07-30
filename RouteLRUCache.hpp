// RouteLRUCache.hpp (Doubly-Linked List + Hash Map)

#ifndef ROUTE_LRU_CACHE_HPP
#define ROUTE_LRU_CACHE_HPP

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <utility>

struct CacheNode {
    std::string key;
    std::vector<int> path;
    CacheNode* prev;
    CacheNode* next;

    CacheNode(std::string k, std::vector<int> p)
        : key(std::move(k)), path(std::move(p)), prev(nullptr), next(nullptr) {}
};

class RouteLRUCache {
private:
    size_t capacity;
    std::unordered_map<std::string, CacheNode*> map;
    CacheNode* head; // Most Recently Used
    CacheNode* tail; // Least Recently Used

    void removeNode(CacheNode* node) {
        if (node->prev) node->prev->next = node->next;
        else head = node->next;

        if (node->next) node->next->prev = node->prev;
        else tail = node->prev;
    }

    void moveToHead(CacheNode* node) {
        removeNode(node);
        node->next = head;
        node->prev = nullptr;
        if (head) head->prev = node;
        head = node;
        if (!tail) tail = head;
    }

public:
    explicit RouteLRUCache(size_t cap) : capacity(cap), head(nullptr), tail(nullptr) {}

    ~RouteLRUCache() {
        CacheNode* curr = head;
        while (curr) {
            CacheNode* nextNode = curr->next;
            delete curr;
            curr = nextNode;
        }
    }

    bool get(const std::string& key, std::vector<int>& resultPath) {
        if (map.find(key) == map.end()) return false;
        CacheNode* node = map[key];
        moveToHead(node);
        resultPath = node->path;
        return true;
    }

    void put(const std::string& key, const std::vector<int>& path) {
        if (map.find(key) != map.end()) {
            CacheNode* node = map[key];
            node->path = path;
            moveToHead(node);
            return;
        }

        if (map.size() >= capacity) {
            map.erase(tail->key);
            CacheNode* oldTail = tail;
            removeNode(tail);
            delete oldTail;
        }

        CacheNode* newNode = new CacheNode(key, path);
        newNode->next = head;
        if (head) head->prev = newNode;
        head = newNode;
        if (!tail) tail = head;
        map[key] = newNode;
    }
};

#endif