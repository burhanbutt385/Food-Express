#ifndef ROUTE_OPTIMIZER_H
#define ROUTE_OPTIMIZER_H

#include "MapGraph.h"
#include "../Burhan/CustomList.h"
#include <string>

class RouteOptimizer {
public:
    RouteOptimizer();
    ~RouteOptimizer();

    // Runs Dijkstra to find the shortest distance (either by raw distance or traffic-adjusted travel time)
    double calculateShortestDistance(const std::string& startId, const std::string& endId, 
                                     const MapGraph& graph, bool optimizeForTime = true) const;

    // Runs Dijkstra and returns the path sequence of location IDs
    CustomList<std::string> findShortestPath(const std::string& startId, const std::string& endId, 
                                            const MapGraph& graph, bool optimizeForTime = true) const;

    // Estimates travel time in minutes (travel time = distance * trafficMultiplier at speed 50 km/h or 1 km/min)
    double calculateTravelTime(const std::string& startId, const std::string& endId, const MapGraph& graph) const;

    // Estimates delivery fee based on distance and traffic factor
    double estimateDeliveryFee(double distance, double averageTrafficMultiplier) const;

    // Prints path details to the screen
    void printRouteDetails(const std::string& startId, const std::string& endId, const MapGraph& graph) const;
};

#endif // ROUTE_OPTIMIZER_H
