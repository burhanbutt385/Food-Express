#ifndef RESTAURANT_H
#define RESTAURANT_H

#include <string>

class Restaurant {
private:
    std::string restaurantID;
    std::string name;
    std::string locationNodeID;
    int maxCapacity;
    int currentLoad;

public:
    Restaurant();
    Restaurant(const std::string& id, const std::string& restaurantName, 
               const std::string& locationId, int capacity, int load = 0);

    std::string getRestaurantID() const;
    std::string getName() const;
    std::string getLocationNodeID() const;
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
