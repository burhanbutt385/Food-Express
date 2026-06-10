#include "SearchEngine.h"
using namespace std;

SearchEngine::SearchEngine() {}

SearchEngine::~SearchEngine() {}

void SearchEngine::indexOrder(Order* order) {
    if (order != nullptr) {
        orderIndex.put(order->getOrderID(), order);
    }
}

void SearchEngine::indexRider(Rider* rider) {
    if (rider != nullptr) {
        riderIndex.put(rider->getRiderID(), rider);
    }
}

void SearchEngine::indexRestaurant(Restaurant* restaurant) {
    if (restaurant != nullptr) {
        restaurantIndex.put(restaurant->getRestaurantID(), restaurant);
    }
}

void SearchEngine::indexCustomer(Customer* customer) {
    if (customer != nullptr) {
        customerIndex.put(customer->getCustomerID(), customer);
    }
}

void SearchEngine::removeOrderIndex(const string& orderId) {
    orderIndex.remove(orderId);
}

Order* SearchEngine::findOrder(const string& orderId) const {
    Order* val = nullptr;
    orderIndex.get(orderId, val);
    return val;
}

Rider* SearchEngine::findRider(const string& riderId) const {
    Rider* val = nullptr;
    riderIndex.get(riderId, val);
    return val;
}

Restaurant* SearchEngine::findRestaurant(const string& restaurantId) const {
    Restaurant* val = nullptr;
    restaurantIndex.get(restaurantId, val);
    return val;
}

Customer* SearchEngine::findCustomer(const string& customerId) const {
    Customer* val = nullptr;
    customerIndex.get(customerId, val);
    return val;
}

CustomList<Order*> SearchEngine::getOrdersByCustomer(const string& customerId) const {
    CustomList<Order*> results;
    int size = orderIndex.getTableSize();
    CustomHashMap<Order*>::HashNode** table = orderIndex.getTable();
    
    for (int i = 0; i < size; i++) {
        CustomHashMap<Order*>::HashNode* curr = table[i];
        while (curr != nullptr) {
            if (curr->value->getCustomerID() == customerId) {
                results.pushBack(curr->value);
            }
            curr = curr->next;
        }
    }
    return results;
}

CustomList<Rider*> SearchEngine::getAvailableRiders() const {
    CustomList<Rider*> results;
    int size = riderIndex.getTableSize();
    CustomHashMap<Rider*>::HashNode** table = riderIndex.getTable();
    
    for (int i = 0; i < size; i++) {
        CustomHashMap<Rider*>::HashNode* curr = table[i];
        while (curr != nullptr) {
            if (curr->value->hasCapacity()) {
                results.pushBack(curr->value);
            }
            curr = curr->next;
        }
    }
    return results;
}

CustomList<Restaurant*> SearchEngine::getOverloadedRestaurants(double workloadThreshold) const {
    CustomList<Restaurant*> results;
    int size = restaurantIndex.getTableSize();
    CustomHashMap<Restaurant*>::HashNode** table = restaurantIndex.getTable();
    
    for (int i = 0; i < size; i++) {
        CustomHashMap<Restaurant*>::HashNode* curr = table[i];
        while (curr != nullptr) {
            if (curr->value->getWorkloadRatio() >= workloadThreshold) {
                results.pushBack(curr->value);
            }
            curr = curr->next;
        }
    }
    return results;
}

CustomList<Order*> SearchEngine::getOrdersByStatus(const string& status) const {
    CustomList<Order*> results;
    int size = orderIndex.getTableSize();
    CustomHashMap<Order*>::HashNode** table = orderIndex.getTable();
    
    for (int i = 0; i < size; i++) {
        CustomHashMap<Order*>::HashNode* curr = table[i];
        while (curr != nullptr) {
            if (curr->value->getStatus() == status) {
                results.pushBack(curr->value);
            }
            curr = curr->next;
        }
    }
    return results;
}

void SearchEngine::rebuildOrderIndex(const CustomList<Order*>& allOrders) {
    orderIndex.clear();
    CustomList<Order*>::Node* curr = allOrders.getHead();
    while (curr != nullptr) {
        indexOrder(curr->data);
        curr = curr->next;
    }
}

void SearchEngine::rebuildRiderIndex(const CustomList<Rider*>& allRiders) {
    riderIndex.clear();
    CustomList<Rider*>::Node* curr = allRiders.getHead();
    while (curr != nullptr) {
        indexRider(curr->data);
        curr = curr->next;
    }
}

void SearchEngine::rebuildRestaurantIndex(const CustomList<Restaurant*>& allRestaurants) {
    restaurantIndex.clear();
    CustomList<Restaurant*>::Node* curr = allRestaurants.getHead();
    while (curr != nullptr) {
        indexRestaurant(curr->data);
        curr = curr->next;
    }
}

void SearchEngine::rebuildCustomerIndex(const CustomList<Customer*>& allCustomers) {
    customerIndex.clear();
    CustomList<Customer*>::Node* curr = allCustomers.getHead();
    while (curr != nullptr) {
        indexCustomer(curr->data);
        curr = curr->next;
    }
}
