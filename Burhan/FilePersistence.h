#ifndef FILE_PERSISTENCE_H
#define FILE_PERSISTENCE_H

#include "CustomList.h"
#include <string>

// Forward declarations to avoid circular dependencies
class Customer;
class Restaurant;
class Rider;
class OrderSchedulingEngine;
class MapGraph;

class FilePersistence {
private:
    std::string customersFile;
    std::string restaurantsFile;
    std::string ridersFile;
    std::string ordersFile;
    std::string mapFile;

    void generateDefaultFiles();

public:
    FilePersistence(
        const std::string& customersPath = "customers.txt",
        const std::string& restaurantsPath = "restaurants.txt",
        const std::string& ridersPath = "riders.txt",
        const std::string& ordersPath = "orders.txt",
        const std::string& mapPath = "map.txt"
    );

    // Creates default data if files do not exist
    void checkAndBootstrap();

    // Load functions
    bool loadCustomers(CustomList<Customer*>& customers);
    bool loadRestaurants(CustomList<Restaurant*>& restaurants);
    bool loadRiders(CustomList<Rider*>& riders);
    bool loadOrders(OrderSchedulingEngine& schedulingEngine, int currentSimTime);
    bool loadMap(MapGraph& graph);

    // Save functions
    bool saveCustomers(const CustomList<Customer*>& customers);
    bool saveRestaurants(const CustomList<Restaurant*>& restaurants);
    bool saveRiders(const CustomList<Rider*>& riders);
    bool saveOrders(OrderSchedulingEngine& schedulingEngine);
    bool saveMap(const MapGraph& graph);
};

#endif // FILE_PERSISTENCE_H
