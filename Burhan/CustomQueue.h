#ifndef CUSTOM_QUEUE_H
#define CUSTOM_QUEUE_H

template <typename T>
class CustomQueue {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& val) : data(val), next(nullptr) {}
    };

    Node* frontNode;
    Node* rearNode;
    int queueSize;

public:
    CustomQueue();
    ~CustomQueue();

    void enqueue(const T& val);
    void dequeue();
    T getFront() const;
    bool isEmpty() const;
    int size() const;
    void clear();
};

// Definitions moved to CustomQueue.cpp – included below
#include "CustomQueue.cpp"

#endif // CUSTOM_QUEUE_H
