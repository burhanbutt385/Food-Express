#ifndef FILE_PERSISTENCE_H
#define FILE_PERSISTENCE_H

#include "CustomList.h"
#include <string>
using namespace std;

// Forward declarations to avoid circular dependencies
class Customer;
class Restaurant;
class Rider;
class OrderSchedulingEngine;
class MapGraph;

class FilePersistence {
private:
    string customersFile;
    string restaurantsFile;
    string ridersFile;
    string ordersFile;
    string mapFile;

    void generateDefaultFiles();

public:
    FilePersistence(
        const string& customersPath = "customers.txt",
        const string& restaurantsPath = "restaurants.txt",
        const string& ridersPath = "riders.txt",
        const string& ordersPath = "orders.txt",
        const string& mapPath = "map.txt"
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
