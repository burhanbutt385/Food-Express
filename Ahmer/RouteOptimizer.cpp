#include "RouteOptimizer.h"
#include <iostream>

RouteOptimizer::RouteOptimizer() {}

RouteOptimizer::~RouteOptimizer() {}

// Core Dijkstra implementation
static void runDijkstra(const std::string& startId, const MapGraph& graph, bool optimizeForTime, 
                        double* dist, int* prev) {
    int V = graph.getVertexCount();
    int startIndex = graph.findVertexIndex(startId);

    bool* visited = new bool[V];
    for (int i = 0; i < V; i++) {
        dist[i] = 1e9;
        prev[i] = -1;
        visited[i] = false;
    }

    if (startIndex != -1) {
        dist[startIndex] = 0.0;
    }

    for (int count = 0; count < V; count++) {
        // Find the vertex with the minimum distance value from the set of vertices not yet processed
        int u = -1;
        double minDist = 1e9;
        for (int i = 0; i < V; i++) {
            if (!visited[i] && dist[i] < minDist) {
                minDist = dist[i];
                u = i;
            }
        }

        if (u == -1 || dist[u] >= 1e9) {
            break;
        }

        visited[u] = true;

        // Update dist value of the adjacent vertices of the picked vertex.
        std::string uId = graph.getVertexIdAt(u);
        Edge* edge = graph.getEdgesFrom(uId);
        while (edge != nullptr) {
            // REROUTING: If the road is blocked, skip it
            if (edge->isBlocked) {
                edge = edge->next;
                continue;
            }

            int v = graph.findVertexIndex(edge->destinationNodeID);
            if (v != -1 && !visited[v]) {
                // If optimizing for time, edge weight = distance * trafficMultiplier
                // If optimizing for distance, edge weight = distance
                double weight = optimizeForTime ? (edge->distance * edge->trafficMultiplier) : edge->distance;
                
                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    prev[v] = u;
                }
            }
            edge = edge->next;
        }
    }

    delete[] visited;
}

double RouteOptimizer::calculateShortestDistance(const std::string& startId, const std::string& endId, 
                                                 const MapGraph& graph, bool optimizeForTime) const {
    if (startId == endId) return 0.0;

    int V = graph.getVertexCount();
    double* dist = new double[V];
    int* prev = new int[V];

    runDijkstra(startId, graph, optimizeForTime, dist, prev);

    int endIndex = graph.findVertexIndex(endId);
    double result = 1e9;
    if (endIndex != -1) {
        result = dist[endIndex];
    }

    delete[] dist;
    delete[] prev;
    return result;
}

CustomList<std::string> RouteOptimizer::findShortestPath(const std::string& startId, const std::string& endId, 
                                                        const MapGraph& graph, bool optimizeForTime) const {
    CustomList<std::string> path;
    if (!graph.locationExists(startId) || !graph.locationExists(endId)) return path;

    int V = graph.getVertexCount();
    double* dist = new double[V];
    int* prev = new int[V];

    runDijkstra(startId, graph, optimizeForTime, dist, prev);

    int endIndex = graph.findVertexIndex(endId);
    if (endIndex != -1 && dist[endIndex] < 1e9) {
        // Reconstruct path backward
        int current = endIndex;
        while (current != -1) {
            path.pushFront(graph.getVertexIdAt(current));
            current = prev[current];
        }
    }

    delete[] dist;
    delete[] prev;
    return path;
}

double RouteOptimizer::calculateTravelTime(const std::string& startId, const std::string& endId, const MapGraph& graph) const {
    // Travel time optimization is standard. 
    // Distance * traffic multiplier gives effective distance.
    // Assuming 50 km/h average speed: travel time in minutes = effective distance * 1.2
    double effectiveDistance = calculateShortestDistance(startId, endId, graph, true);
    if (effectiveDistance >= 1e9) return 1e9; // unreachable
    
    // travel time = effectiveDistance * 1.2 minutes
    return effectiveDistance * 1.2;
}

double RouteOptimizer::estimateDeliveryFee(double distance, double averageTrafficMultiplier) const {
    // Base delivery fee = $2.50
    // Distance charge = $0.75 per km
    // Traffic surcharge = averageTrafficMultiplier * 1.20
    if (distance >= 1e9) return 0.0;
    return 2.50 + (distance * 0.75) * averageTrafficMultiplier;
}

void RouteOptimizer::printRouteDetails(const std::string& startId, const std::string& endId, const MapGraph& graph) const {
    std::cout << "Calculating route from [" << startId << "] to [" << endId << "]..." << std::endl;
    
    // Compare time vs distance optimization
    double rawDistance = calculateShortestDistance(startId, endId, graph, false); // distance optimized
    double timeOptimizedDistance = calculateShortestDistance(startId, endId, graph, true); // time optimized
    double travelTime = calculateTravelTime(startId, endId, graph);

    if (rawDistance >= 1e9) {
        std::cout << "[RouteOptimizer] Error: No route exists between " << startId << " and " << endId 
                  << " (roads might be blocked)." << std::endl;
        return;
    }

    CustomList<std::string> path = findShortestPath(startId, endId, graph, true);
    
    std::cout << "\n--- Route Optimization Summary ---" << std::endl;
    std::cout << "Optimized Path : ";
    CustomList<std::string>::Node* node = path.getHead();
    while (node != nullptr) {
        std::cout << node->data;
        if (node->next != nullptr) std::cout << " -> ";
        node = node->next;
    }
    std::cout << std::endl;

    std::cout << "Shortest Distance    : " << rawDistance << " km" << std::endl;
    std::cout << "Actual Route Distance: " << timeOptimizedDistance << " km (Traffic Adjusted)" << std::endl;
    std::cout << "Estimated Travel Time: " << travelTime << " mins" << std::endl;
    std::cout << "Estimated Delivery Fee: $" << estimateDeliveryFee(timeOptimizedDistance, timeOptimizedDistance / (rawDistance + 0.01)) << std::endl;
    std::cout << "----------------------------------\n" << std::endl;
}
