#include "Restaurant.h"

Restaurant::Restaurant() 
    : restaurantID(""), name(""), locationNodeID(""), maxCapacity(5), currentLoad(0) {}

Restaurant::Restaurant(const std::string& id, const std::string& restaurantName, 
                       const std::string& locationId, int capacity, int load)
    : restaurantID(id), name(restaurantName), locationNodeID(locationId), 
      maxCapacity(capacity), currentLoad(load) {}

std::string Restaurant::getRestaurantID() const { return restaurantID; }
std::string Restaurant::getName() const { return name; }
std::string Restaurant::getLocationNodeID() const { return locationNodeID; }
int Restaurant::getMaxCapacity() const { return maxCapacity; }
int Restaurant::getCurrentLoad() const { return currentLoad; }

void Restaurant::setCurrentLoad(int load) { currentLoad = load; }

void Restaurant::incrementLoad() {
    currentLoad++;
}

void Restaurant::decrementLoad() {
    if (currentLoad > 0) {
        currentLoad--;
    }
}

bool Restaurant::isOverloaded() const {
    return currentLoad >= maxCapacity;
}

double Restaurant::getWorkloadRatio() const {
    if (maxCapacity == 0) return 1.0;
    return (double)currentLoad / maxCapacity;
}
