#ifndef CUSTOM_HASH_MAP_H
#define CUSTOM_HASH_MAP_H
#include <string>
using namespace std;

template <typename V>
class CustomHashMap {
public:
    struct HashNode {
        string key;
        V value;
        HashNode* next;
        HashNode(const string& k, const V& val) : key(k), value(val), next(nullptr) {}
    };

private:
    HashNode** table;
    int tableSize;
    int mapSize;

    int computeHash(const string& key) const;

public:
    CustomHashMap(int size = 101);
    ~CustomHashMap();

    void put(const string& key, const V& value);
    bool get(const string& key, V& outValue) const;
    V* getPointer(const string& key) const;
    bool remove(const string& key);
    bool contains(const string& key) const;
    void clear();

    int size() const;
    bool isEmpty() const;

    // Helper to get all nodes/keys for iteration
    HashNode** getTable() const { return table; }
    int getTableSize() const { return tableSize; }
};

// Template implementations must be visible at compile time for all instantiations.
// Implementation moved inline into the header to avoid duplicate definitions.

template <typename V>
CustomHashMap<V>::CustomHashMap(int size) : tableSize(size), mapSize(0) {
    table = new HashNode*[tableSize];
    for (int i = 0; i < tableSize; i++) {
        table[i] = nullptr;
    }
}

template <typename V>
CustomHashMap<V>::~CustomHashMap() {
    clear();
    delete[] table;
}

template <typename V>
int CustomHashMap<V>::computeHash(const string& key) const {
    unsigned int hashVal = 0;
    for (char c : key) {
        hashVal = hashVal * 31 + c;
    }
    return hashVal % tableSize;
}

template <typename V>
void CustomHashMap<V>::put(const string& key, const V& value) {
    int index = computeHash(key);
    HashNode* current = table[index];
    while (current != nullptr) {
        if (current->key == key) {
            current->value = value;
            return;
        }
        current = current->next;
    }

    HashNode* newNode = new HashNode(key, value);
    newNode->next = table[index];
    table[index] = newNode;
    mapSize++;
}

template <typename V>
bool CustomHashMap<V>::get(const string& key, V& outValue) const {
    int index = computeHash(key);
    HashNode* current = table[index];
    while (current != nullptr) {
        if (current->key == key) {
            outValue = current->value;
            return true;
        }
        current = current->next;
    }
    return false;
}

template <typename V>
V* CustomHashMap<V>::getPointer(const string& key) const {
    int index = computeHash(key);
    HashNode* current = table[index];
    while (current != nullptr) {
        if (current->key == key) {
            return &(current->value);
        }
        current = current->next;
    }
    return nullptr;
}

template <typename V>
bool CustomHashMap<V>::remove(const string& key) {
    int index = computeHash(key);
    HashNode* current = table[index];
    HashNode* prev = nullptr;
    while (current != nullptr) {
        if (current->key == key) {
            if (prev == nullptr) {
                table[index] = current->next;
            } else {
                prev->next = current->next;
            }
            delete current;
            mapSize--;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}

template <typename V>
bool CustomHashMap<V>::contains(const string& key) const {
    int index = computeHash(key);
    HashNode* current = table[index];
    while (current != nullptr) {
        if (current->key == key) {
            return true;
        }
        current = current->next;
    }
    return false;
}

template <typename V>
void CustomHashMap<V>::clear() {
    for (int i = 0; i < tableSize; i++) {
        HashNode* current = table[i];
        while (current != nullptr) {
            HashNode* temp = current;
            current = current->next;
            delete temp;
        }
        table[i] = nullptr;
    }
    mapSize = 0;
}

template <typename V>
int CustomHashMap<V>::size() const {
    return mapSize;
}

template <typename V>
bool CustomHashMap<V>::isEmpty() const {
    return mapSize == 0;
}

#endif // CUSTOM_HASH_MAP_H
