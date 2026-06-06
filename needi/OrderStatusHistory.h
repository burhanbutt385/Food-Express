#ifndef ORDER_STATUS_HISTORY_H
#define ORDER_STATUS_HISTORY_H

#include "../Burhan/Order.h"
#include "../Burhan/CustomStack.h"
#include "../Burhan/CustomList.h"
#include <string>

struct StatusTransition {
    std::string status;
    int timestamp; // Simulated time in minutes

    StatusTransition() : status(""), timestamp(0) {}
    StatusTransition(const std::string& stat, int time) : status(stat), timestamp(time) {}
};

class OrderStatusHistory {
private:
    CustomStack<StatusTransition> undoStack;
    CustomList<StatusTransition> timeline;
    std::string orderID;

public:
    OrderStatusHistory();
    OrderStatusHistory(const std::string& orderId, const std::string& initialStatus, int initialTime);
    ~OrderStatusHistory();

    // Records a new status transition
    void recordTransition(Order* order, const std::string& newStatus, int currentSimTime);

    // Reverts the order to the previous status using the stack
    bool undoLastTransition(Order* order, int currentSimTime);

    // Replays the timeline of status transitions
    void replayTimeline() const;

    std::string getOrderID() const;
    const CustomList<StatusTransition>& getTimeline() const;
};

#endif // ORDER_STATUS_HISTORY_H
