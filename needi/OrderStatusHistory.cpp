#include "OrderStatusHistory.h"
#include <iostream>

OrderStatusHistory::OrderStatusHistory() : orderID("") {}

OrderStatusHistory::OrderStatusHistory(const std::string& orderId, const std::string& initialStatus, int initialTime)
    : orderID(orderId) {
    StatusTransition transition(initialStatus, initialTime);
    undoStack.push(transition);
    timeline.pushBack(transition);
}

OrderStatusHistory::~OrderStatusHistory() {}

void OrderStatusHistory::recordTransition(Order* order, const std::string& newStatus, int currentSimTime) {
    if (order == nullptr || order->getOrderID() != orderID) return;

    // Check if newStatus is different from current status to avoid duplicate logs
    if (order->getStatus() == newStatus) return;

    order->setStatus(newStatus);
    StatusTransition transition(newStatus, currentSimTime);
    undoStack.push(transition);
    timeline.pushBack(transition);
}

bool OrderStatusHistory::undoLastTransition(Order* order, int currentSimTime) {
    if (order == nullptr || order->getOrderID() != orderID) return false;
    if (undoStack.size() <= 1) {
        // Can't undo further than the initial transition
        std::cout << "[OrderStatusHistory] Cannot undo: Order " << orderID << " is at its initial state." << std::endl;
        return false;
    }

    // Pop the current state
    undoStack.pop();
    
    // The top of the stack is now the previous state
    StatusTransition prevState = undoStack.top();
    
    // Update the order's status
    order->setStatus(prevState.status);

    // Record the undo action in the timeline
    StatusTransition undoTransition("Undone to " + prevState.status, currentSimTime);
    timeline.pushBack(undoTransition);

    std::cout << "[OrderStatusHistory] UNDO: Reverted Order " << orderID 
              << " to status: '" << prevState.status << "'" << std::endl;
    return true;
}

void OrderStatusHistory::replayTimeline() const {
    std::cout << "\nTimeline Replay for Order: " << orderID << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    
    CustomList<StatusTransition>::Node* current = timeline.getHead();
    int step = 1;
    while (current != nullptr) {
        std::cout << step++ << ". [Sim Time: " << current->data.timestamp << " min] status: " 
                  << current->data.status << std::endl;
        current = current->next;
    }
    std::cout << "---------------------------------------\n" << std::endl;
}

std::string OrderStatusHistory::getOrderID() const { return orderID; }

const CustomList<StatusTransition>& OrderStatusHistory::getTimeline() const { return timeline; }
