#ifndef KITCHEN_LOAD_BALANCER_H
#define KITCHEN_LOAD_BALANCER_H

#include "Restaurant.h"
#include "../Burhan/Order.h"
#include "../Burhan/CustomList.h"
#include <string>
using namespace std;

class KitchenLoadBalancer {
public:
    KitchenLoadBalancer();
    ~KitchenLoadBalancer();

    // Assigns an order to a restaurant and checks for overloading.
    // If overloaded, automatically attempts to find a sister restaurant with spare capacity.
    // Returns the final assigned Restaurant.
    Restaurant* assignOrderToKitchen(Order* order, CustomList<Restaurant*>& restaurants, bool autoRebalance = true);

    // Estimates waiting time for an order at a specific restaurant
    int estimateWaitingTime(const string& restaurantId, Order* order, const CustomList<Restaurant*>& restaurants) const;

    // Checks and returns a list of overloaded restaurants
    CustomList<Restaurant*> getOverloadedKitchens(const CustomList<Restaurant*>& restaurants) const;

    // Performs explicit dynamic load balancing across all restaurants
    void balanceWorkloads(CustomList<Restaurant*>& restaurants);
};

#endif // KITCHEN_LOAD_BALANCER_H
