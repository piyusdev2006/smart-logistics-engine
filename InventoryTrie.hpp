// InventoryTrie.hpp (Trees + Recursion)

#ifndef INVENTORY_TRIE_HPP
#define INVENTORY_TRIE_HPP

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <fstream>


// SKU :-  stock keeping unit
struct TrieNode {
    std::unordered_map<char, std::shared_ptr<TrieNode>> children;
    bool isEndOfSKU = false;
    int itemID = -1;
};

class InventoryTrie {
private:
    std::shared_ptr<TrieNode> root;

    // Recursive helper function for prefix traversal
    void collectSKUsRecursive(std::shared_ptr<TrieNode> curr, std::string currentPrefix, std::vector<std::string>& results) {
        if (curr->isEndOfSKU) {
            results.push_back(currentPrefix);
        }
        for (auto& pair : curr->children) {
            collectSKUsRecursive(pair.second, currentPrefix + pair.first, results);
        }
    }

public:
    InventoryTrie() : root(std::make_shared<TrieNode>()) {}

    void insertSKU(const std::string& sku, int itemID) {
        auto curr = root;
        for (char ch : sku) {
            if (curr->children.find(ch) == curr->children.end()) {
                curr->children[ch] = std::make_shared<TrieNode>();
            }
            curr = curr->children[ch];
        }
        curr->isEndOfSKU = true;
        curr->itemID = itemID;
    }

    std::vector<std::string> searchByPrefix(const std::string& prefix) {
        auto curr = root;
        std::vector<std::string> results;
        for (char ch : prefix) {
            if (curr->children.find(ch) == curr->children.end()) {
                return results; // Empty list
            }
            curr = curr->children[ch];
        }
        collectSKUsRecursive(curr, prefix, results);
        return results;
    }

    void saveToFile(std::ofstream& outFile) {
        std::vector<std::string> allSKUs = searchByPrefix("");
        outFile << "[INVENTORY]\n" << allSKUs.size() << "\n";
        for (const auto& sku : allSKUs) {
            outFile << sku << "\n";
        }
    }

    void loadFromFile(std::ifstream& inFile) {
        size_t count;
        if (inFile >> count) {
            for (size_t i = 0; i < count; ++i) {
                std::string sku;
                inFile >> sku;
                insertSKU(sku, i + 1);
            }
        }
    }
};

#endif