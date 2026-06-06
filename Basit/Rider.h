#ifndef RIDER_H
#define RIDER_H

#include <string>

class Rider {
private:
    std::string riderID;
    std::string name;
    std::string currentLocationNodeID;
    int maxCapacity;
    int currentLoad;
    bool isAvailable;
    double rating;
    int deliveriesCompleted;

public:
    Rider();
    Rider(const std::string& id, const std::string& riderName, const std::string& locationId,
          int capacity, int load = 0, bool available = true, double riderRating = 5.0, int completed = 0);

    std::string getRiderID() const;
    std::string getName() const;
    std::string getCurrentLocationNodeID() const;
    int getMaxCapacity() const;
    int getCurrentLoad() const;
    bool getIsAvailable() const;
    double getRating() const;
    int getDeliveriesCompleted() const;

    void setCurrentLocationNodeID(const std::string& locationId);
    void setCurrentLoad(int load);
    void incrementLoad();
    void decrementLoad();
    void setIsAvailable(bool available);
    void setRating(double riderRating);
    void setDeliveriesCompleted(int completed);
    void incrementDeliveriesCompleted();

    // Check if the rider has space for more orders
    bool hasCapacity() const;
};

#endif // RIDER_H
