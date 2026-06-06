#include "Rider.h"

Rider::Rider()
    : riderID(""), name(""), currentLocationNodeID(""), maxCapacity(3), 
      currentLoad(0), isAvailable(true), rating(5.0), deliveriesCompleted(0) {}

Rider::Rider(const std::string& id, const std::string& riderName, const std::string& locationId,
             int capacity, int load, bool available, double riderRating, int completed)
    : riderID(id), name(riderName), currentLocationNodeID(locationId), maxCapacity(capacity),
      currentLoad(load), isAvailable(available), rating(riderRating), deliveriesCompleted(completed) {}

std::string Rider::getRiderID() const { return riderID; }
std::string Rider::getName() const { return name; }
std::string Rider::getCurrentLocationNodeID() const { return currentLocationNodeID; }
int Rider::getMaxCapacity() const { return maxCapacity; }
int Rider::getCurrentLoad() const { return currentLoad; }
bool Rider::getIsAvailable() const { return isAvailable; }
double Rider::getRating() const { return rating; }
int Rider::getDeliveriesCompleted() const { return deliveriesCompleted; }

void Rider::setCurrentLocationNodeID(const std::string& locationId) { currentLocationNodeID = locationId; }
void Rider::setCurrentLoad(int load) { currentLoad = load; }

void Rider::incrementLoad() {
    currentLoad++;
}

void Rider::decrementLoad() {
    if (currentLoad > 0) {
        currentLoad--;
    }
}

void Rider::setIsAvailable(bool available) { isAvailable = available; }
void Rider::setRating(double riderRating) { rating = riderRating; }
void Rider::setDeliveriesCompleted(int completed) { deliveriesCompleted = completed; }

void Rider::incrementDeliveriesCompleted() {
    deliveriesCompleted++;
}

bool Rider::hasCapacity() const {
    return isAvailable && (currentLoad < maxCapacity);
}
