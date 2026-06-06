#ifndef ORDER_H
#define ORDER_H

#include <string>

class Order {
private:
    std::string orderID;
    std::string customerID;
    std::string restaurantID;
    double priorityScore;
    int preparationTime;    // in minutes
    int deliveryDeadline;   // in minutes from arrival
    std::string status;     // Placed, Accepted, Queued, Prepared, Assigned, Picked, Delivered, Delayed, Cancelled, Rerouted
    int arrivalTime;        // simulation time in minutes

public:
    Order();
    Order(const std::string& orderId, const std::string& customerId, const std::string& restaurantId,
          int prepTime, int deadline, int arrival, const std::string& orderStatus = "Placed");

    std::string getOrderID() const;
    std::string getCustomerID() const;
    std::string getRestaurantID() const;
    double getPriorityScore() const;
    int getPreparationTime() const;
    int getDeliveryDeadline() const;
    std::string getStatus() const;
    int getArrivalTime() const;

    void setPriorityScore(double score);
    void setStatus(const std::string& newStatus);
    void setDeliveryDeadline(int deadline);
    void setPreparationTime(int prepTime);

    // Calculate priority based on VIP status, deadline proximity, and prep time
    void calculatePriority(bool isCustomerVIP, int currentSimTime);
};

#endif // ORDER_H
