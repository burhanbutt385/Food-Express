#ifndef CUSTOM_QUEUE_H
#define CUSTOM_QUEUE_H
using namespace std;

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
    CustomQueue() : frontNode(nullptr), rearNode(nullptr), queueSize(0) {}
    ~CustomQueue() { clear(); }

    void enqueue(const T& val) {
        Node* newNode = new Node(val);
        if (isEmpty()) {
            frontNode = rearNode = newNode;
        } else {
            rearNode->next = newNode;
            rearNode = newNode;
        }
        queueSize++;
    }

    void dequeue() {
        if (isEmpty()) return;
        Node* temp = frontNode;
        frontNode = frontNode->next;
        if (frontNode == nullptr) rearNode = nullptr;
        delete temp;
        queueSize--;
    }

    T getFront() const {
        if (!isEmpty()) return frontNode->data;
        return T(); 
    }

    bool isEmpty() const { return queueSize == 0; }
    int size() const { return queueSize; }

    void clear() {
        while (!isEmpty()) {
            dequeue();
        }
    }
};

#endif // CUSTOM_QUEUE_H
