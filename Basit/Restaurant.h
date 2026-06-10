#ifndef RESTAURANT_H
#define RESTAURANT_H

#include <string>
using namespace std;

class Restaurant {
private:
    string restaurantID;
    string name;
    string locationNodeID;
    int maxCapacity;
    int currentLoad;

public:
    Restaurant();
    Restaurant(const string& id, const string& restaurantName, 
               const string& locationId, int capacity, int load = 0);

    string getRestaurantID() const;
    string getName() const;
    string getLocationNodeID() const;
    int getMaxCapacity() const;
    int getCurrentLoad() const;

    void setCurrentLoad(int load);
    void incrementLoad();
    void decrementLoad();
    
    // Checks if the kitchen is overloaded (load >= capacity)
    bool isOverloaded() const;

    // Workload ratio (0.0 to 1.0+)
    double getWorkloadRatio() const;
};

#endif // RESTAURANT_H
