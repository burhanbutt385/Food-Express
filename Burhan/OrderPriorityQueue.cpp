#include "OrderPriorityQueue.h"
using namespace std;

OrderPriorityQueue::OrderPriorityQueue(int initialCapacity)
    : capacity(initialCapacity), heapSize(0) {
    heapArray = new Order*[capacity];
}

OrderPriorityQueue::~OrderPriorityQueue() {
    delete[] heapArray;
}

void OrderPriorityQueue::resize() {
    capacity *= 2;
    Order** newArray = new Order*[capacity];
    for (int i = 0; i < heapSize; i++) {
        newArray[i] = heapArray[i];
    }
    delete[] heapArray;
    heapArray = newArray;
}

void OrderPriorityQueue::heapifyUp(int index) {
    int parentIndex = (index - 1) / 2;
    while (index > 0 && heapArray[index]->getPriorityScore() > heapArray[parentIndex]->getPriorityScore()) {
        // Swap
        Order* temp = heapArray[index];
        heapArray[index] = heapArray[parentIndex];
        heapArray[parentIndex] = temp;

        index = parentIndex;
        parentIndex = (index - 1) / 2;
    }
}

void OrderPriorityQueue::heapifyDown(int index) {
    int largestIndex = index;
    int leftChild = 2 * index + 1;
    int rightChild = 2 * index + 2;

    if (leftChild < heapSize && heapArray[leftChild]->getPriorityScore() > heapArray[largestIndex]->getPriorityScore()) {
        largestIndex = leftChild;
    }

    if (rightChild < heapSize && heapArray[rightChild]->getPriorityScore() > heapArray[largestIndex]->getPriorityScore()) {
        largestIndex = rightChild;
    }

    if (largestIndex != index) {
        Order* temp = heapArray[index];
        heapArray[index] = heapArray[largestIndex];
        heapArray[largestIndex] = temp;

        heapifyDown(largestIndex);
    }
}

void OrderPriorityQueue::insert(Order* order) {
    if (heapSize == capacity) {
        resize();
    }
    heapArray[heapSize] = order;
    heapifyUp(heapSize);
    heapSize++;
}

Order* OrderPriorityQueue::popMax() {
    if (isEmpty()) return nullptr;
    Order* maxOrder = heapArray[0];
    heapArray[0] = heapArray[heapSize - 1];
    heapSize--;
    if (heapSize > 0) {
        heapifyDown(0);
    }
    return maxOrder;
}

Order* OrderPriorityQueue::getMax() const {
    if (isEmpty()) return nullptr;
    return heapArray[0];
}

bool OrderPriorityQueue::isEmpty() const {
    return heapSize == 0;
}

int OrderPriorityQueue::size() const {
    return heapSize;
}

void OrderPriorityQueue::clear() {
    heapSize = 0;
}

bool OrderPriorityQueue::updatePriority(const string& orderId, double newScore) {
    // Find index of order
    for (int i = 0; i < heapSize; i++) {
        if (heapArray[i]->getOrderID() == orderId) {
            double oldScore = heapArray[i]->getPriorityScore();
            heapArray[i]->setPriorityScore(newScore);

            if (newScore > oldScore) {
                heapifyUp(i);
            } else if (newScore < oldScore) {
                heapifyDown(i);
            }
            return true;
        }
    }
    return false;
}

bool OrderPriorityQueue::remove(const string& orderId) {
    // Find index of order
    for (int i = 0; i < heapSize; i++) {
        if (heapArray[i]->getOrderID() == orderId) {
            // Replace with last element and heapify
            Order* target = heapArray[i];
            heapArray[i] = heapArray[heapSize - 1];
            heapSize--;

            if (i < heapSize) {
                double score = heapArray[i]->getPriorityScore();
                double targetScore = target->getPriorityScore();
                if (score > targetScore) {
                    heapifyUp(i);
                } else {
                    heapifyDown(i);
                }
            }
            return true;
        }
    }
    return false;
}
