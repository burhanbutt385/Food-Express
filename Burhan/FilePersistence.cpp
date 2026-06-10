#include "FilePersistence.h"
#include "../Burhan/Order.h"
#include "../Burhan/OrderSchedulingEngine.h"
#include "../Basit/Restaurant.h"
#include "../Basit/Rider.h"
#include "../Ahmer/Customer.h"
#include "../Ahmer/MapGraph.h"

#include <fstream>

#include <iostream>
using namespace std;

static void splitString(const string& str, char delim, CustomList<string>& tokens) {
    size_t start = 0;
    size_t end = str.find(delim);
    while (end != string::npos) {
        tokens.pushBack(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delim, start);
    }
    tokens.pushBack(str.substr(start));
}

FilePersistence::FilePersistence(
    const string& customersPath,
    const string& restaurantsPath,
    const string& ridersPath,
    const string& ordersPath,
    const string& mapPath
) : customersFile(customersPath), restaurantsFile(restaurantsPath), 
    ridersFile(ridersPath), ordersFile(ordersPath), mapFile(mapPath) {}

void FilePersistence::checkAndBootstrap() {
    // Check if files exist. If not, generate them.
    ifstream fMap(mapFile);
    ifstream fCust(customersFile);
    ifstream fRest(restaurantsFile);
    ifstream fRide(ridersFile);
    ifstream fOrd(ordersFile);

    bool needGenerate = !fMap.good() || !fCust.good() || !fRest.good() || !fRide.good() || !fOrd.good();

    if (fMap.good()) fMap.close();
    if (fCust.good()) fCust.close();
    if (fRest.good()) fRest.close();
    if (fRide.good()) fRide.close();
    if (fOrd.good()) fOrd.close();

    if (needGenerate) {
        cout << "[FilePersistence] Generating default simulation data files..." << endl;
        generateDefaultFiles();
    }
}

void FilePersistence::generateDefaultFiles() {
    // 1. Map graph
    ofstream fMap(mapFile);
    if (fMap.is_open()) {
        fMap << "LOCATIONS\n";
        fMap << "Gulberg,Gulberg Commercial Zone\n";
        fMap << "DHA,DHA Residential Area\n";
        fMap << "ModelTown,Model Town Green Park\n";
        fMap << "JoharTown,Johar Town Food Street\n";
        fMap << "Samanabad,Samanabad Old City\n";
        fMap << "Anarkali,Anarkali Historical Bazaar\n";
        fMap << "Shadman,Shadman Market Square\n";
        fMap << "BahriaTown,Bahria Town Suburb\n";
        fMap << "ROADS\n";
        fMap << "Gulberg,DHA,8.5,1.5,0\n";
        fMap << "Gulberg,ModelTown,5.0,1.2,0\n";
        fMap << "Gulberg,JoharTown,10.0,1.4,0\n";
        fMap << "ModelTown,JoharTown,6.0,1.1,0\n";
        fMap << "Samanabad,Anarkali,4.0,1.8,0\n";
        fMap << "JoharTown,Samanabad,9.0,1.3,0\n";
        fMap << "Anarkali,Shadman,3.5,1.6,0\n";
        fMap << "Shadman,Gulberg,4.5,1.4,0\n";
        fMap << "DHA,BahriaTown,25.0,1.1,0\n";
        fMap << "JoharTown,BahriaTown,15.0,1.2,0\n";
        fMap.close();
    }

    // 2. Customers
    ofstream fCust(customersFile);
    if (fCust.is_open()) {
        fCust << "C01,Fatima,Gulberg,1\n";
        fCust << "C02,Muhammad,Anarkali,0\n";
        fCust << "C03,Zainab,DHA,1\n";
        fCust << "C04,Ayesha,JoharTown,0\n";
        fCust << "C05,Hamza,Samanabad,0\n";
        fCust << "C06,Mustafa,Shadman,1\n";
        fCust.close();
    }

    // 3. Restaurants
    ofstream fRest(restaurantsFile);
    if (fRest.is_open()) {
        fRest << "R01,ButtKarahi,Gulberg,5,0\n";
        fRest << "R02,SavourFoods,Anarkali,4,0\n";
        fRest << "R03,SaltAnPepper,DHA,3,0\n";
        fRest << "R04,BunduKhan,JoharTown,4,0\n";
        fRest.close();
    }

    // 4. Riders
    ofstream fRide(ridersFile);
    if (fRide.is_open()) {
        fRide << "RD01,AliKhan,Gulberg,3,0,1,4.8,12\n";
        fRide << "RD02,AhmedShah,Anarkali,3,0,1,4.5,8\n";
        fRide << "RD03,BilalRaza,DHA,3,0,1,4.9,15\n";
        fRide << "RD04,UsmanSiddique,JoharTown,3,0,1,4.2,5\n";
        fRide.close();
    }

    // 5. Orders (seed with a couple of past orders)
    ofstream fOrd(ordersFile);
    if (fOrd.is_open()) {
        fOrd << "O01,C02,R01,15.0,15,35,Delivered,0\n";
        fOrd << "O02,C01,R02,40.0,20,40,Queued,0\n";
        fOrd << "O03,C03,R03,25.0,12,30,Placed,0\n";
        fOrd.close();
    }
}

bool FilePersistence::loadCustomers(CustomList<Customer*>& customers) {
    ifstream file(customersFile);
    if (!file.is_open()) return false;

    // Clear existing
    while (!customers.isEmpty()) {
        delete customers.getHead()->data;
        customers.popFront();
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        CustomList<string> tokens;
        splitString(line, ',', tokens);
        if (tokens.size() >= 4) {
            string id = tokens.getHead()->data;
            string name = tokens.getHead()->next->data;
            string loc = tokens.getHead()->next->next->data;
            bool vip = (tokens.getHead()->next->next->next->data == "1");
            customers.pushBack(new Customer(id, name, loc, vip));
        }
    }
    file.close();
    return true;
}

bool FilePersistence::loadRestaurants(CustomList<Restaurant*>& restaurants) {
    ifstream file(restaurantsFile);
    if (!file.is_open()) return false;

    // Clear existing
    while (!restaurants.isEmpty()) {
        delete restaurants.getHead()->data;
        restaurants.popFront();
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        CustomList<string> tokens;
        splitString(line, ',', tokens);
        if (tokens.size() >= 5) {
            string id = tokens.getHead()->data;
            string name = tokens.getHead()->next->data;
            string loc = tokens.getHead()->next->next->data;
            int cap = stoi(tokens.getHead()->next->next->next->data);
            int load = stoi(tokens.getHead()->next->next->next->next->data);
            restaurants.pushBack(new Restaurant(id, name, loc, cap, load));
        }
    }
    file.close();
    return true;
}

bool FilePersistence::loadRiders(CustomList<Rider*>& riders) {
    ifstream file(ridersFile);
    if (!file.is_open()) return false;

    // Clear existing
    while (!riders.isEmpty()) {
        delete riders.getHead()->data;
        riders.popFront();
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        CustomList<string> tokens;
        splitString(line, ',', tokens);
        if (tokens.size() >= 8) {
            string id = tokens.getHead()->data;
            string name = tokens.getHead()->next->data;
            string loc = tokens.getHead()->next->next->data;
            int cap = stoi(tokens.getHead()->next->next->next->data);
            int load = stoi(tokens.getHead()->next->next->next->next->data);
            bool avail = (tokens.getHead()->next->next->next->next->next->data == "1");
            double rat = stod(tokens.getHead()->next->next->next->next->next->next->data);
            int completed = stoi(tokens.getHead()->next->next->next->next->next->next->next->data);
            riders.pushBack(new Rider(id, name, loc, cap, load, avail, rat, completed));
        }
    }
    file.close();
    return true;
}

bool FilePersistence::loadOrders(OrderSchedulingEngine& schedulingEngine, int currentSimTime) {
    ifstream file(ordersFile);
    if (!file.is_open()) return false;

    // Clear active queues (the destructor of engine clears allOrders ownership)
    schedulingEngine.getActiveOrdersQueue().clear();
    
    // Note: To reload allOrdersList, we need to manually clear it. But let's keep it clean
    // by freeing the old list.
    CustomList<Order*>& allOrders = schedulingEngine.getAllOrders();
    while (!allOrders.isEmpty()) {
        delete allOrders.getHead()->data;
        allOrders.popFront();
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        CustomList<string> tokens;
        splitString(line, ',', tokens);
        if (tokens.size() >= 8) {
            string id = tokens.getHead()->data;
            string custId = tokens.getHead()->next->data;
            string restId = tokens.getHead()->next->next->data;
            double prio = stod(tokens.getHead()->next->next->next->data);
            int prep = stoi(tokens.getHead()->next->next->next->next->data);
            int dead = stoi(tokens.getHead()->next->next->next->next->next->data);
            string stat = tokens.getHead()->next->next->next->next->next->next->data;
            int arr = stoi(tokens.getHead()->next->next->next->next->next->next->next->data);

            Order* order = new Order(id, custId, restId, prep, dead, arr, stat);
            order->setPriorityScore(prio);

            // Re-insert into scheduling engine:
            // If active status (Placed, Queued, Delayed, Accepted), put back in queue.
            // All orders go to allOrdersList.
            allOrders.pushBack(order);
            if (stat == "Placed" || stat == "Queued" || stat == "Delayed" || stat == "Accepted") {
                schedulingEngine.getActiveOrdersQueue().insert(order);
            }
        }
    }
    file.close();
    return true;
}

bool FilePersistence::loadMap(MapGraph& graph) {
    ifstream file(mapFile);
    if (!file.is_open()) return false;

    graph.clear();

    string line;
    bool inLocations = false;
    bool inRoads = false;

    while (getline(file, line)) {
        if (line.empty()) continue;
        if (line == "LOCATIONS") {
            inLocations = true;
            inRoads = false;
            continue;
        } else if (line == "ROADS") {
            inLocations = false;
            inRoads = true;
            continue;
        }

        CustomList<string> tokens;
        splitString(line, ',', tokens);

        if (inLocations && tokens.size() >= 2) {
            string id = tokens.getHead()->data;
            string name = tokens.getHead()->next->data;
            graph.addLocation(id, name);
        } else if (inRoads && tokens.size() >= 5) {
            string src = tokens.getHead()->data;
            string dest = tokens.getHead()->next->data;
            double dist = stod(tokens.getHead()->next->next->data);
            double traffic = stod(tokens.getHead()->next->next->next->data);
            bool blocked = (tokens.getHead()->next->next->next->next->data == "1");
            graph.addRoad(src, dest, dist, traffic, blocked);
        }
    }
    file.close();
    return true;
}

bool FilePersistence::saveCustomers(const CustomList<Customer*>& customers) {
    ofstream file(customersFile);
    if (!file.is_open()) return false;

    CustomList<Customer*>::Node* current = customers.getHead();
    while (current != nullptr) {
        Customer* c = current->data;
        file << c->getCustomerID() << "," << c->getName() << "," << c->getLocationNodeID() << ","
             << (c->getIsVIP() ? "1" : "0") << "\n";
        current = current->next;
    }
    file.close();
    return true;
}

bool FilePersistence::saveRestaurants(const CustomList<Restaurant*>& restaurants) {
    ofstream file(restaurantsFile);
    if (!file.is_open()) return false;

    CustomList<Restaurant*>::Node* current = restaurants.getHead();
    while (current != nullptr) {
        Restaurant* r = current->data;
        file << r->getRestaurantID() << "," << r->getName() << "," << r->getLocationNodeID() << ","
             << r->getMaxCapacity() << "," << r->getCurrentLoad() << "\n";
        current = current->next;
    }
    file.close();
    return true;
}

bool FilePersistence::saveRiders(const CustomList<Rider*>& riders) {
    ofstream file(ridersFile);
    if (!file.is_open()) return false;

    CustomList<Rider*>::Node* current = riders.getHead();
    while (current != nullptr) {
        Rider* r = current->data;
        file << r->getRiderID() << "," << r->getName() << "," << r->getCurrentLocationNodeID() << ","
             << r->getMaxCapacity() << "," << r->getCurrentLoad() << ","
             << (r->getIsAvailable() ? "1" : "0") << "," << r->getRating() << ","
             << r->getDeliveriesCompleted() << "\n";
        current = current->next;
    }
    file.close();
    return true;
}

bool FilePersistence::saveOrders(OrderSchedulingEngine& schedulingEngine) {
    ofstream file(ordersFile);
    if (!file.is_open()) return false;

    CustomList<Order*>& allOrders = schedulingEngine.getAllOrders();
    CustomList<Order*>::Node* current = allOrders.getHead();
    while (current != nullptr) {
        Order* o = current->data;
        file << o->getOrderID() << "," << o->getCustomerID() << "," << o->getRestaurantID() << ","
             << o->getPriorityScore() << "," << o->getPreparationTime() << ","
             << o->getDeliveryDeadline() << "," << o->getStatus() << ","
             << o->getArrivalTime() << "\n";
        current = current->next;
    }
    file.close();
    return true;
}

bool FilePersistence::saveMap(const MapGraph& graph) {
    ofstream file(mapFile);
    if (!file.is_open()) return false;

    file << "LOCATIONS\n";
    for (int i = 0; i < graph.getVertexCount(); i++) {
        file << graph.getVertexIdAt(i) << "," << graph.getVertexNameAt(i) << "\n";
    }

    file << "ROADS\n";
    // To prevent saving duplicate undirected edges, we can do a simple check or save all, 
    // and loadMap updates existing edges anyway. Saving all is safe since loadRoad updates!
    for (int i = 0; i < graph.getVertexCount(); i++) {
        string srcId = graph.getVertexIdAt(i);
        Edge* edge = graph.getEdgesFrom(srcId);
        while (edge != nullptr) {
            // Write undirected edge: to write once, save only if srcId < destinationNodeID
            if (srcId < edge->destinationNodeID) {
                file << srcId << "," << edge->destinationNodeID << "," << edge->distance << ","
                     << edge->trafficMultiplier << "," << (edge->isBlocked ? "1" : "0") << "\n";
            }
            edge = edge->next;
        }
    }
    file.close();
    return true;
}
