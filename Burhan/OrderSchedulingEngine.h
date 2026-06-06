#ifndef ORDER_SCHEDULING_ENGINE_H
#define ORDER_SCHEDULING_ENGINE_H

#include "Order.h"
#include "OrderPriorityQueue.h"
#include "CustomList.h"
#include "CustomHashMap.h"
#include <string>

class OrderSchedulingEngine {
private:
    OrderPriorityQueue activeOrdersQueue;
    CustomList<Order*> allOrdersList; // stores ownership of all orders created in simulation
    CustomHashMap<bool> vipCustomerMap; // cache for VIP statuses

public:
    OrderSchedulingEngine();
    ~OrderSchedulingEngine();

    // Inserts a new order into the scheduler
    void insertOrder(Order* order, bool isVIP, int currentSimTime);

    // Cancels an order by ID
    bool cancelOrder(const std::string& orderId);

    // Updates priority score of an order manually
    bool updateOrderPriority(const std::string& orderId, double newScore);

    // Handles a delay: extends deadline and recalculates priority
    bool delayOrder(const std::string& orderId, int delayMinutes, int currentSimTime);

    // Retrieves and removes the next order to process
    Order* getNextProcessableOrder();

    // Dynamic Rescheduling: Recalculates priorities for all active orders based on current simulated time
    void rescheduleActiveOrders(int currentSimTime);

    // Adds a customer VIP status to cache
    void setCustomerVIPStatus(const std::string& customerId, bool isVIP);
    bool isCustomerVIP(const std::string& customerId) const;

    CustomList<Order*>& getAllOrders();
    OrderPriorityQueue& getActiveOrdersQueue();
};

#endif // ORDER_SCHEDULING_ENGINE_H
