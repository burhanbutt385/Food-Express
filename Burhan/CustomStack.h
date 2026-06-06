#ifndef CUSTOM_STACK_H
#define CUSTOM_STACK_H

template <typename T>
class CustomStack {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& val) : data(val), next(nullptr) {}
    };

    Node* topNode;
    int stackSize;

public:
    CustomStack();
    ~CustomStack();

    void push(const T& val);
    void pop();
    T top() const;
    bool isEmpty() const;
    int size() const;
    void clear();
};

#include <stdexcept>

template <typename T>
CustomStack<T>::CustomStack() : topNode(nullptr), stackSize(0) {}

template <typename T>
CustomStack<T>::~CustomStack() {
    clear();
}

template <typename T>
void CustomStack<T>::push(const T& val) {
    Node* newNode = new Node(val);
    newNode->next = topNode;
    topNode = newNode;
    stackSize++;
}

template <typename T>
void CustomStack<T>::pop() {
    if (isEmpty()) return;
    Node* temp = topNode;
    topNode = topNode->next;
    delete temp;
    stackSize--;
}

template <typename T>
T CustomStack<T>::top() const {
    if (isEmpty()) {
        throw std::runtime_error("Stack is empty");
    }
    return topNode->data;
}

template <typename T>
bool CustomStack<T>::isEmpty() const {
    return topNode == nullptr;
}

template <typename T>
int CustomStack<T>::size() const {
    return stackSize;
}

template <typename T>
void CustomStack<T>::clear() {
    while (!isEmpty()) {
        pop();
    }
}

#endif // CUSTOM_STACK_H
