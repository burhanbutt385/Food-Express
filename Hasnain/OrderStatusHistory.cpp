#include "OrderStatusHistory.h"
#include <iostream>
using namespace std;

OrderStatusHistory::OrderStatusHistory() : orderID("") {}

OrderStatusHistory::OrderStatusHistory(const string& orderId, const string& initialStatus, int initialTime)
    : orderID(orderId) {
    StatusTransition transition(initialStatus, initialTime);
    undoStack.push(transition);
    timeline.pushBack(transition);
}

OrderStatusHistory::~OrderStatusHistory() {}

void OrderStatusHistory::recordTransition(Order* order, const string& newStatus, int currentSimTime) {
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
        cout << "[OrderStatusHistory] Cannot undo: Order " << orderID << " is at its initial state." << endl;
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

    cout << "[OrderStatusHistory] UNDO: Reverted Order " << orderID 
              << " to status: '" << prevState.status << "'" << endl;
    return true;
}

void OrderStatusHistory::replayTimeline() const {
    cout << "\nTimeline Replay for Order: " << orderID << endl;
    cout << "---------------------------------------" << endl;
    
    CustomList<StatusTransition>::Node* current = timeline.getHead();
    int step = 1;
    while (current != nullptr) {
        cout << step++ << ". [Sim Time: " << current->data.timestamp << " min] status: " 
                  << current->data.status << endl;
        current = current->next;
    }
    cout << "---------------------------------------\n" << endl;
}

string OrderStatusHistory::getOrderID() const { return orderID; }

const CustomList<StatusTransition>& OrderStatusHistory::getTimeline() const { return timeline; }
