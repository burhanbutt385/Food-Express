#ifndef MAP_GRAPH_H
#define MAP_GRAPH_H

#include <string>

struct Edge {
    std::string destinationNodeID;
    double distance;          // In kilometers
    double trafficMultiplier; // 1.0 = clear, 1.5 = moderate, 2.0+ = heavy traffic
    bool isBlocked;          // Road blocked flag
    Edge* next;

    Edge(const std::string& dest, double dist, double traffic, bool blocked = false);
};

struct Vertex {
    std::string locationNodeID;
    std::string name;
    Edge* edgeHead;

    Vertex();
    ~Vertex();
};

class MapGraph {
private:
    Vertex* vertices;
    int capacity;
    int vertexCount;

    void resize();

public:
    MapGraph(int initialCapacity = 10);
    ~MapGraph();

    // Adds a location node
    void addLocation(const std::string& locationId, const std::string& name);

    // Adds an undirected road connection
    void addRoad(const std::string& sourceId, const std::string& destinationId, 
                 double distance, double trafficMultiplier = 1.0, bool isBlocked = false);

    // Blocks a specific road
    bool blockRoad(const std::string& sourceId, const std::string& destinationId);

    // Unblocks a specific road
    bool unblockRoad(const std::string& sourceId, const std::string& destinationId);

    // Updates traffic conditions for a road
    bool setTrafficFactor(const std::string& sourceId, const std::string& destinationId, double multiplier);

    // Helper functions
    int getVertexCount() const;
    int findVertexIndex(const std::string& locationId) const;
    std::string getVertexIdAt(int index) const;
    std::string getVertexNameAt(int index) const;
    Edge* getEdgesFrom(const std::string& locationId) const;
    
    // Clear all vertices and edges
    void clear();

    // Check if location exists
    bool locationExists(const std::string& locationId) const;
};

#endif // MAP_GRAPH_H
