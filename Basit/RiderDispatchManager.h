#ifndef RIDER_DISPATCH_MANAGER_H
#define RIDER_DISPATCH_MANAGER_H

#include "Rider.h"
#include "Restaurant.h"
#include "../Burhan/Order.h"
#include "../Burhan/CustomList.h"
#include "../Ahmer/Customer.h"

// Forward declarations
class MapGraph;
class RouteOptimizer;

class RiderDispatchManager {
public:
    RiderDispatchManager();
    ~RiderDispatchManager();

    // Finds the optimal rider based on proximity, current load, and rating.
    // Looks up the restaurant's actual map node from the restaurants list.
    Rider* findOptimalRider(Order* order, CustomList<Rider*>& riders,
                            const CustomList<Restaurant*>& restaurants,
                            MapGraph& graph, RouteOptimizer& routeOptimizer);

    // Assigns an order to a rider, updating their load and the order status
    bool assignRider(Rider* rider, Order* order);

    // Releases a rider when they complete an order.
    // Updates rider location to the customer's actual map node.
    void completeDelivery(Rider* rider, Order* order, const CustomList<Customer*>& customers);

    // Prints a dispatch dashboard
    void displayRidersStatus(const CustomList<Rider*>& riders) const;
};

#endif // RIDER_DISPATCH_MANAGER_H
