#ifndef CUSTOM_LIST_H
#define CUSTOM_LIST_H

template <typename T>
class CustomList {
public:
    struct Node {
        T data;
        Node* prev;
        Node* next;
        Node(const T& val) : data(val), prev(nullptr), next(nullptr) {}
    };

private:
    Node* head;
    Node* tail;
    int listSize;

public:
    CustomList();
    CustomList(const CustomList& other);
    CustomList& operator=(const CustomList& other);
    ~CustomList();

    void pushBack(const T& val);
    void pushFront(const T& val);
    void popBack();
    void popFront();
    bool remove(const T& val);
    void clear();

    bool isEmpty() const;
    int size() const;

    Node* getHead() const;
    Node* getTail() const;
};

template <typename T>
CustomList<T>::CustomList() : head(nullptr), tail(nullptr), listSize(0) {}

template <typename T>
CustomList<T>::CustomList(const CustomList& other) : head(nullptr), tail(nullptr), listSize(0) {
    Node* current = other.head;
    while (current != nullptr) {
        pushBack(current->data);
        current = current->next;
    }
}

template <typename T>
CustomList<T>& CustomList<T>::operator=(const CustomList& other) {
    if (this != &other) {
        clear();
        Node* current = other.head;
        while (current != nullptr) {
            pushBack(current->data);
            current = current->next;
        }
    }
    return *this;
}

template <typename T>
CustomList<T>::~CustomList() {
    clear();
}

template <typename T>
void CustomList<T>::pushBack(const T& val) {
    Node* newNode = new Node(val);
    if (isEmpty()) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    listSize++;
}

template <typename T>
void CustomList<T>::pushFront(const T& val) {
    Node* newNode = new Node(val);
    if (isEmpty()) {
        head = tail = newNode;
    } else {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }
    listSize++;
}

template <typename T>
void CustomList<T>::popBack() {
    if (isEmpty()) return;
    Node* temp = tail;
    if (head == tail) {
        head = tail = nullptr;
    } else {
        tail = tail->prev;
        tail->next = nullptr;
    }
    delete temp;
    listSize--;
}

template <typename T>
void CustomList<T>::popFront() {
    if (isEmpty()) return;
    Node* temp = head;
    if (head == tail) {
        head = tail = nullptr;
    } else {
        head = head->next;
        head->prev = nullptr;
    }
    delete temp;
    listSize--;
}

template <typename T>
bool CustomList<T>::remove(const T& val) {
    Node* current = head;
    while (current != nullptr) {
        if (current->data == val) {
            if (current == head) {
                popFront();
            } else if (current == tail) {
                popBack();
            } else {
                current->prev->next = current->next;
                current->next->prev = current->prev;
                delete current;
                listSize--;
            }
            return true;
        }
        current = current->next;
    }
    return false;
}

template <typename T>
void CustomList<T>::clear() {
    while (!isEmpty()) {
        popFront();
    }
}

template <typename T>
bool CustomList<T>::isEmpty() const {
    return head == nullptr;
}

template <typename T>
int CustomList<T>::size() const {
    return listSize;
}

template <typename T>
typename CustomList<T>::Node* CustomList<T>::getHead() const {
    return head;
}

template <typename T>
typename CustomList<T>::Node* CustomList<T>::getTail() const {
    return tail;
}

#endif // CUSTOM_LIST_H
