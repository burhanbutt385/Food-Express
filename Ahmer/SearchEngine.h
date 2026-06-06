#ifndef SEARCH_ENGINE_H
#define SEARCH_ENGINE_H

#include "../Burhan/Order.h"
#include "../Person2/Rider.h"
#include "../Person2/Restaurant.h"
#include "Customer.h"
#include "../Burhan/CustomList.h"
#include "../Burhan/CustomHashMap.h"
#include <string>

class SearchEngine {
private:
    CustomHashMap<Order*> orderIndex;
    CustomHashMap<Rider*> riderIndex;
    CustomHashMap<Restaurant*> restaurantIndex;
    CustomHashMap<Customer*> customerIndex;

public:
    SearchEngine();
    ~SearchEngine();

    // Index insertions
    void indexOrder(Order* order);
    void indexRider(Rider* rider);
    void indexRestaurant(Restaurant* restaurant);
    void indexCustomer(Customer* customer);

    // Index removals (e.g. on deletion/cancellation)
    void removeOrderIndex(const std::string& orderId);

    // O(1) Lookups
    Order* findOrder(const std::string& orderId) const;
    Rider* findRider(const std::string& riderId) const;
    Restaurant* findRestaurant(const std::string& restaurantId) const;
    Customer* findCustomer(const std::string& customerId) const;

    // Filtered searches
    CustomList<Order*> getOrdersByCustomer(const std::string& customerId) const;
    CustomList<Rider*> getAvailableRiders() const;
    CustomList<Restaurant*> getOverloadedRestaurants(double workloadThreshold = 1.0) const;
    CustomList<Order*> getOrdersByStatus(const std::string& status) const;

    // Rebuild indexes from master lists
    void rebuildOrderIndex(const CustomList<Order*>& allOrders);
    void rebuildRiderIndex(const CustomList<Rider*>& allRiders);
    void rebuildRestaurantIndex(const CustomList<Restaurant*>& allRestaurants);
    void rebuildCustomerIndex(const CustomList<Customer*>& allCustomers);
};

#endif // SEARCH_ENGINE_H
