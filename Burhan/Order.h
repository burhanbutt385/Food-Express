#ifndef ORDER_H
#define ORDER_H

#include <string>
using namespace std;

class Order {
private:
    string orderID;
    string customerID;
    string restaurantID;
    double priorityScore;
    int preparationTime;    // in minutes
    int deliveryDeadline;   // in minutes from arrival
    string status;     // Placed, Accepted, Queued, Prepared, Assigned, Picked, Delivered, Delayed, Cancelled, Rerouted
    int arrivalTime;        // simulation time in minutes

public:
    Order();
    Order(const string& orderId, const string& customerId, const string& restaurantId,
          int prepTime, int deadline, int arrival, const string& orderStatus = "Placed");

    string getOrderID() const;
    string getCustomerID() const;
    string getRestaurantID() const;
    double getPriorityScore() const;
    int getPreparationTime() const;
    int getDeliveryDeadline() const;
    string getStatus() const;
    int getArrivalTime() const;

    void setPriorityScore(double score);
    void setStatus(const string& newStatus);
    void setDeliveryDeadline(int deadline);
    void setPreparationTime(int prepTime);

    // Calculate priority based on VIP status, deadline proximity, and prep time
    void calculatePriority(bool isCustomerVIP, int currentSimTime);
};

#endif // ORDER_H
