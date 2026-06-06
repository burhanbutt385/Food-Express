#include "MapGraph.h"
#include <iostream>

Edge::Edge(const std::string& dest, double dist, double traffic, bool blocked)
    : destinationNodeID(dest), distance(dist), trafficMultiplier(traffic), isBlocked(blocked), next(nullptr) {}

Vertex::Vertex() : locationNodeID(""), name(""), edgeHead(nullptr) {}

Vertex::~Vertex() {
    Edge* current = edgeHead;
    while (current != nullptr) {
        Edge* temp = current;
        current = current->next;
        delete temp;
    }
}

MapGraph::MapGraph(int initialCapacity)
    : capacity(initialCapacity), vertexCount(0) {
    vertices = new Vertex[capacity];
}

MapGraph::~MapGraph() {
    delete[] vertices;
}

void MapGraph::resize() {
    capacity *= 2;
    Vertex* newVertices = new Vertex[capacity];
    for (int i = 0; i < vertexCount; i++) {
        newVertices[i].locationNodeID = vertices[i].locationNodeID;
        newVertices[i].name = vertices[i].name;
        // Transfer head pointer ownership to new array, clearing from old array
        newVertices[i].edgeHead = vertices[i].edgeHead;
        vertices[i].edgeHead = nullptr; // prevent double deletion in old array destructor
    }
    delete[] vertices;
    vertices = newVertices;
}

void MapGraph::addLocation(const std::string& locationId, const std::string& name) {
    if (locationExists(locationId)) return;
    
    if (vertexCount == capacity) {
        resize();
    }
    vertices[vertexCount].locationNodeID = locationId;
    vertices[vertexCount].name = name;
    vertices[vertexCount].edgeHead = nullptr;
    vertexCount++;
}

void MapGraph::addRoad(const std::string& sourceId, const std::string& destinationId, 
                       double distance, double trafficMultiplier, bool isBlocked) {
    addLocation(sourceId, sourceId); // ensure locations exist
    addLocation(destinationId, destinationId);

    int u = findVertexIndex(sourceId);
    int v = findVertexIndex(destinationId);

    if (u == -1 || v == -1) return;

    // Check if edge already exists source -> dest
    Edge* curr = vertices[u].edgeHead;
    while (curr != nullptr) {
        if (curr->destinationNodeID == destinationId) {
            // Edge already exists, just update it
            curr->distance = distance;
            curr->trafficMultiplier = trafficMultiplier;
            curr->isBlocked = isBlocked;
            break;
        }
        curr = curr->next;
    }
    if (curr == nullptr) {
        // Add new edge source -> dest
        Edge* newEdge = new Edge(destinationId, distance, trafficMultiplier, isBlocked);
        newEdge->next = vertices[u].edgeHead;
        vertices[u].edgeHead = newEdge;
    }

    // Since roads are bidirectional, add edge dest -> source
    curr = vertices[v].edgeHead;
    while (curr != nullptr) {
        if (curr->destinationNodeID == sourceId) {
            curr->distance = distance;
            curr->trafficMultiplier = trafficMultiplier;
            curr->isBlocked = isBlocked;
            break;
        }
        curr = curr->next;
    }
    if (curr == nullptr) {
        Edge* newEdge = new Edge(sourceId, distance, trafficMultiplier, isBlocked);
        newEdge->next = vertices[v].edgeHead;
        vertices[v].edgeHead = newEdge;
    }
}

bool MapGraph::blockRoad(const std::string& sourceId, const std::string& destinationId) {
    int u = findVertexIndex(sourceId);
    int v = findVertexIndex(destinationId);

    if (u == -1 || v == -1) return false;

    bool found = false;
    Edge* curr = vertices[u].edgeHead;
    while (curr != nullptr) {
        if (curr->destinationNodeID == destinationId) {
            curr->isBlocked = true;
            found = true;
            break;
        }
        curr = curr->next;
    }

    curr = vertices[v].edgeHead;
    while (curr != nullptr) {
        if (curr->destinationNodeID == sourceId) {
            curr->isBlocked = true;
            found = true;
            break;
        }
        curr = curr->next;
    }

    return found;
}

bool MapGraph::unblockRoad(const std::string& sourceId, const std::string& destinationId) {
    int u = findVertexIndex(sourceId);
    int v = findVertexIndex(destinationId);

    if (u == -1 || v == -1) return false;

    bool found = false;
    Edge* curr = vertices[u].edgeHead;
    while (curr != nullptr) {
        if (curr->destinationNodeID == destinationId) {
            curr->isBlocked = false;
            found = true;
            break;
        }
        curr = curr->next;
    }

    curr = vertices[v].edgeHead;
    while (curr != nullptr) {
        if (curr->destinationNodeID == sourceId) {
            curr->isBlocked = false;
            found = true;
            break;
        }
        curr = curr->next;
    }

    return found;
}

bool MapGraph::setTrafficFactor(const std::string& sourceId, const std::string& destinationId, double multiplier) {
    int u = findVertexIndex(sourceId);
    int v = findVertexIndex(destinationId);

    if (u == -1 || v == -1) return false;

    bool found = false;
    Edge* curr = vertices[u].edgeHead;
    while (curr != nullptr) {
        if (curr->destinationNodeID == destinationId) {
            curr->trafficMultiplier = multiplier;
            found = true;
            break;
        }
        curr = curr->next;
    }

    curr = vertices[v].edgeHead;
    while (curr != nullptr) {
        if (curr->destinationNodeID == sourceId) {
            curr->trafficMultiplier = multiplier;
            found = true;
            break;
        }
        curr = curr->next;
    }

    return found;
}

int MapGraph::getVertexCount() const {
    return vertexCount;
}

int MapGraph::findVertexIndex(const std::string& locationId) const {
    for (int i = 0; i < vertexCount; i++) {
        if (vertices[i].locationNodeID == locationId) {
            return i;
        }
    }
    return -1;
}

std::string MapGraph::getVertexIdAt(int index) const {
    if (index < 0 || index >= vertexCount) return "";
    return vertices[index].locationNodeID;
}

std::string MapGraph::getVertexNameAt(int index) const {
    if (index < 0 || index >= vertexCount) return "";
    return vertices[index].name;
}

Edge* MapGraph::getEdgesFrom(const std::string& locationId) const {
    int u = findVertexIndex(locationId);
    if (u == -1) return nullptr;
    return vertices[u].edgeHead;
}

void MapGraph::clear() {
    delete[] vertices;
    capacity = 10;
    vertexCount = 0;
    vertices = new Vertex[capacity];
}

bool MapGraph::locationExists(const std::string& locationId) const {
    return findVertexIndex(locationId) != -1;
}
