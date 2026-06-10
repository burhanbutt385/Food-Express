#ifndef RIDER_H
#define RIDER_H

#include <string>
using namespace std;

class Rider {
private:
    string riderID;
    string name;
    string currentLocationNodeID;
    int maxCapacity;
    int currentLoad;
    bool isAvailable;
    double rating;
    int deliveriesCompleted;

public:
    Rider();
    Rider(const string& id, const string& riderName, const string& locationId,
          int capacity, int load = 0, bool available = true, double riderRating = 5.0, int completed = 0);

    string getRiderID() const;
    string getName() const;
    string getCurrentLocationNodeID() const;
    int getMaxCapacity() const;
    int getCurrentLoad() const;
    bool getIsAvailable() const;
    double getRating() const;
    int getDeliveriesCompleted() const;

    void setCurrentLocationNodeID(const string& locationId);
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
