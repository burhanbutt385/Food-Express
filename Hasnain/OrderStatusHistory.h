#ifndef ORDER_STATUS_HISTORY_H
#define ORDER_STATUS_HISTORY_H

#include "../Burhan/Order.h"
#include "../Burhan/CustomStack.h"
#include "../Burhan/CustomList.h"
#include <string>
using namespace std;

struct StatusTransition {
    string status;
    int timestamp; // Simulated time in minutes

    StatusTransition() : status(""), timestamp(0) {}
    StatusTransition(const string& stat, int time) : status(stat), timestamp(time) {}
};

class OrderStatusHistory {
private:
    CustomStack<StatusTransition> undoStack;
    CustomList<StatusTransition> timeline;
    string orderID;

public:
    OrderStatusHistory();
    OrderStatusHistory(const string& orderId, const string& initialStatus, int initialTime);
    ~OrderStatusHistory();

    // Records a new status transition
    void recordTransition(Order* order, const string& newStatus, int currentSimTime);

    // Reverts the order to the previous status using the stack
    bool undoLastTransition(Order* order, int currentSimTime);

    // Replays the timeline of status transitions
    void replayTimeline() const;

    string getOrderID() const;
    const CustomList<StatusTransition>& getTimeline() const;
};

#endif // ORDER_STATUS_HISTORY_H
