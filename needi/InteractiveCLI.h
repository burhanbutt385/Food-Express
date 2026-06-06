#ifndef INTERACTIVE_CLI_H
#define INTERACTIVE_CLI_H

#include "../Burhan/OrderSchedulingEngine.h"
#include "../Burhan/FilePersistence.h"
#include "../Burhan/CustomQueue.h"
#include "../Person2/KitchenLoadBalancer.h"
#include "../Person2/RiderDispatchManager.h"
#include "../Person3/MapGraph.h"
#include "../Person3/RouteOptimizer.h"
#include "../Person3/SearchEngine.h"
#include "OrderStatusHistory.h"
#include "PerformanceAnalyzer.h"

#include <string>

class InteractiveCLI {
private:
    int currentSimulationTime; // Simulation clock in minutes
    
    // Core Engine components
    OrderSchedulingEngine schedulingEngine;
    KitchenLoadBalancer loadBalancer;
    RiderDispatchManager dispatchManager;
    MapGraph mapGraph;
    RouteOptimizer routeOptimizer;
    SearchEngine searchEngine;
    FilePersistence filePersistence;

    // Master lists
    CustomList<Customer*> customers;
    CustomList<Restaurant*> restaurants;
    CustomList<Rider*> riders;
    CustomList<OrderStatusHistory*> orderHistories;
    CustomQueue<std::string> notifications;

    // Helper functions for menu options
    void runOrderSchedulingMenu();
    void runKitchenLoadMenu();
    void runRiderDispatchMenu();
    void runRouteOptimizationMenu();
    void runSearchEngineMenu();
    void runOrderHistoryMenu();
    void runPerformanceAnalysisMenu();
    void runScalabilitySimulationMenu();
    void runEntityManagementMenu();
    void printSystemReports();

    // Internal helper to find OrderStatusHistory for an order
    OrderStatusHistory* getOrCreateHistory(const std::string& orderId);
    void addNotification(const std::string& msg);

    // Initializer helpers
    void initializeIndexes();
    void saveAllToDisk();

public:
    InteractiveCLI();
    ~InteractiveCLI();

    // Starts the main menu loop
    void start();
};

#endif // INTERACTIVE_CLI_H
