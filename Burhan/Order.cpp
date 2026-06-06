#include "Order.h"

Order::Order()
    : orderID(""), customerID(""), restaurantID(""), priorityScore(0.0),
      preparationTime(0), deliveryDeadline(0), status("Placed"), arrivalTime(0) {}

Order::Order(const std::string& orderId, const std::string& customerId, const std::string& restaurantId,
             int prepTime, int deadline, int arrival, const std::string& orderStatus)
    : orderID(orderId), customerID(customerId), restaurantID(restaurantId), priorityScore(0.0),
      preparationTime(prepTime), deliveryDeadline(deadline), status(orderStatus), arrivalTime(arrival) {
    // Basic priority calculation on creation
    calculatePriority(false, arrival);
}

std::string Order::getOrderID() const { return orderID; }
std::string Order::getCustomerID() const { return customerID; }
std::string Order::getRestaurantID() const { return restaurantID; }
double Order::getPriorityScore() const { return priorityScore; }
int Order::getPreparationTime() const { return preparationTime; }
int Order::getDeliveryDeadline() const { return deliveryDeadline; }
std::string Order::getStatus() const { return status; }
int Order::getArrivalTime() const { return arrivalTime; }

void Order::setPriorityScore(double score) { priorityScore = score; }
void Order::setStatus(const std::string& newStatus) { status = newStatus; }
void Order::setDeliveryDeadline(int deadline) { deliveryDeadline = deadline; }
void Order::setPreparationTime(int prepTime) { preparationTime = prepTime; }

void Order::calculatePriority(bool isCustomerVIP, int currentSimTime) {
    // Dynamic Priority formula:
    // - VIP customer: +150 points
    // - Time remaining before deadline: 120 / (deliveryDeadline - (currentSimTime - arrivalTime))
    // - Longer preparation time (means we need to start it sooner): + (prepTime * 0.5)
    
    double vipWeight = isCustomerVIP ? 150.0 : 0.0;
    
    int elapsedSimTime = currentSimTime - arrivalTime;
    int timeRemaining = deliveryDeadline - elapsedSimTime;
    
    double urgencyWeight = 0.0;
    if (timeRemaining > 0) {
        urgencyWeight = 300.0 / timeRemaining; // grows as timeRemaining shrinks
    } else {
        // Already passed the deadline, extremely high priority to finish late orders
        urgencyWeight = 500.0 + (double)(-timeRemaining * 10);
    }
    
    double prepWeight = (double)preparationTime * 1.5;
    
    priorityScore = vipWeight + urgencyWeight + prepWeight;
}
