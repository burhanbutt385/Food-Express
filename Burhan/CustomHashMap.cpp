// Implementation file for CustomHashMap template – include guard removed
#include <string>
#include "CustomHashMap.h"

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
int CustomHashMap<V>::computeHash(const std::string& key) const {
    unsigned int hashVal = 0;
    for (char c : key) {
        hashVal = hashVal * 31 + c;
    }
    return hashVal % tableSize;
}

template <typename V>
void CustomHashMap<V>::put(const std::string& key, const V& value) {
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
bool CustomHashMap<V>::get(const std::string& key, V& outValue) const {
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
V* CustomHashMap<V>::getPointer(const std::string& key) const {
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
bool CustomHashMap<V>::remove(const std::string& key) {
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
bool CustomHashMap<V>::contains(const std::string& key) const {
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


