#ifndef CUSTOM_HASH_MAP_H
#define CUSTOM_HASH_MAP_H
#include <string>

template <typename V>
class CustomHashMap {
public:
    struct HashNode {
        std::string key;
        V value;
        HashNode* next;
        HashNode(const std::string& k, const V& val) : key(k), value(val), next(nullptr) {}
    };

private:
    HashNode** table;
    int tableSize;
    int mapSize;

    int computeHash(const std::string& key) const;

public:
    CustomHashMap(int size = 101);
    ~CustomHashMap();

    void put(const std::string& key, const V& value);
    bool get(const std::string& key, V& outValue) const;
    V* getPointer(const std::string& key) const;
    bool remove(const std::string& key);
    bool contains(const std::string& key) const;
    void clear();

    int size() const;
    bool isEmpty() const;

    // Helper to get all nodes/keys for iteration
    HashNode** getTable() const { return table; }
    int getTableSize() const { return tableSize; }
};

// Template implementations must be visible at compile time for all instantiations.
// This mirrors the pattern used in CustomList.h and CustomStack.h.
#include "CustomHashMap.cpp"

#endif // CUSTOM_HASH_MAP_H
