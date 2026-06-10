#ifndef ORDER_PRIORITY_QUEUE_H
#define ORDER_PRIORITY_QUEUE_H

#include "Order.h"
#include <string>
using namespace std;

class OrderPriorityQueue {
private:
    Order** heapArray;
    int capacity;
    int heapSize;

    void heapifyUp(int index);
    void heapifyDown(int index);
    void resize();

public:
    OrderPriorityQueue(int initialCapacity = 10);
    ~OrderPriorityQueue();

    void insert(Order* order);
    Order* popMax();
    Order* getMax() const;
    bool isEmpty() const;
    int size() const;
    void clear();

    // Updates priority score of an order by ID and restores heap property
    bool updatePriority(const string& orderId, double newScore);
    
    // Cancels/removes an order from the queue by ID
    bool remove(const string& orderId);

    // Expose raw access for iteration/searching
    Order** getHeapArray() const { return heapArray; }
};

#endif // ORDER_PRIORITY_QUEUE_H
