#ifndef MAP_GRAPH_H
#define MAP_GRAPH_H

#include <string>
using namespace std;

struct Edge {
    string destinationNodeID;
    double distance;          // In kilometers
    double trafficMultiplier; // 1.0 = clear, 1.5 = moderate, 2.0+ = heavy traffic
    bool isBlocked;          // Road blocked flag
    Edge* next;

    Edge(const string& dest, double dist, double traffic, bool blocked = false);
};

struct Vertex {
    string locationNodeID;
    string name;
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
    void addLocation(const string& locationId, const string& name);

    // Adds an undirected road connection
    void addRoad(const string& sourceId, const string& destinationId, 
                 double distance, double trafficMultiplier = 1.0, bool isBlocked = false);

    // Blocks a specific road
    bool blockRoad(const string& sourceId, const string& destinationId);

    // Unblocks a specific road
    bool unblockRoad(const string& sourceId, const string& destinationId);

    // Updates traffic conditions for a road
    bool setTrafficFactor(const string& sourceId, const string& destinationId, double multiplier);

    // Helper functions
    int getVertexCount() const;
    int findVertexIndex(const string& locationId) const;
    string getVertexIdAt(int index) const;
    string getVertexNameAt(int index) const;
    Edge* getEdgesFrom(const string& locationId) const;
    
    // Clear all vertices and edges
    void clear();

    // Check if location exists
    bool locationExists(const string& locationId) const;
};

#endif // MAP_GRAPH_H
