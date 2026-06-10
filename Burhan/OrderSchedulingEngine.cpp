#include "OrderSchedulingEngine.h"
using namespace std;

OrderSchedulingEngine::OrderSchedulingEngine() {}

OrderSchedulingEngine::~OrderSchedulingEngine() {
    // Free all dynamically allocated orders
    CustomList<Order*>::Node* current = allOrdersList.getHead();
    while (current != nullptr) {
        delete current->data;
        current = current->next;
    }
    allOrdersList.clear();
}

void OrderSchedulingEngine::insertOrder(Order* order, bool isVIP, int currentSimTime) {
    // Cache VIP status
    setCustomerVIPStatus(order->getCustomerID(), isVIP);
    
    // Calculate initial priority score
    order->calculatePriority(isVIP, currentSimTime);
    order->setStatus("Queued");
    
    // Add to ownership list and priority queue
    allOrdersList.pushBack(order);
    activeOrdersQueue.insert(order);
}

bool OrderSchedulingEngine::cancelOrder(const string& orderId) {
    // Remove from active queue
    bool removed = activeOrdersQueue.remove(orderId);
    if (removed) {
        // Update status in the main orders list
        CustomList<Order*>::Node* current = allOrdersList.getHead();
        while (current != nullptr) {
            if (current->data->getOrderID() == orderId) {
                current->data->setStatus("Cancelled");
                break;
            }
            current = current->next;
        }
    }
    return removed;
}

bool OrderSchedulingEngine::updateOrderPriority(const string& orderId, double newScore) {
    return activeOrdersQueue.updatePriority(orderId, newScore);
}

bool OrderSchedulingEngine::delayOrder(const string& orderId, int delayMinutes, int currentSimTime) {
    Order** heapArray = activeOrdersQueue.getHeapArray();
    int size = activeOrdersQueue.size();
    
    for (int i = 0; i < size; i++) {
        if (heapArray[i]->getOrderID() == orderId) {
            Order* order = heapArray[i];
            
            // Push the deadline back
            order->setDeliveryDeadline(order->getDeliveryDeadline() + delayMinutes);
            order->setStatus("Delayed");
            
            // Recalculate priority
            bool isVIP = false;
            vipCustomerMap.get(order->getCustomerID(), isVIP);
            order->calculatePriority(isVIP, currentSimTime);
            
            // Re-heapify queue by running update
            activeOrdersQueue.updatePriority(orderId, order->getPriorityScore());
            return true;
        }
    }
    return false;
}

Order* OrderSchedulingEngine::getNextProcessableOrder() {
    Order* nextOrder = activeOrdersQueue.popMax();
    if (nextOrder != nullptr) {
        nextOrder->setStatus("Accepted");
    }
    return nextOrder;
}

void OrderSchedulingEngine::rescheduleActiveOrders(int currentSimTime) {
    if (activeOrdersQueue.isEmpty()) return;

    // Temporary storage for active orders
    int size = activeOrdersQueue.size();
    Order** activeList = new Order*[size];
    
    // Pop everyone out of the queue
    for (int i = 0; i < size; i++) {
        activeList[i] = activeOrdersQueue.popMax();
    }
    
    // Clear queue completely just in case
    activeOrdersQueue.clear();
    
    // Recalculate priorities and re-insert
    for (int i = 0; i < size; i++) {
        bool isVIP = false;
        vipCustomerMap.get(activeList[i]->getCustomerID(), isVIP);
        activeList[i]->calculatePriority(isVIP, currentSimTime);
        activeOrdersQueue.insert(activeList[i]);
    }
    
    delete[] activeList;
}

void OrderSchedulingEngine::setCustomerVIPStatus(const string& customerId, bool isVIP) {
    vipCustomerMap.put(customerId, isVIP);
}

bool OrderSchedulingEngine::isCustomerVIP(const string& customerId) const {
    bool isVIP = false;
    vipCustomerMap.get(customerId, isVIP);
    return isVIP;
}

CustomList<Order*>& OrderSchedulingEngine::getAllOrders() {
    return allOrdersList;
}

OrderPriorityQueue& OrderSchedulingEngine::getActiveOrdersQueue() {
    return activeOrdersQueue;
}
