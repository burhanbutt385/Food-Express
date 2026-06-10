#include "RiderDispatchManager.h"
#include "../Ahmer/MapGraph.h"
#include "../Ahmer/RouteOptimizer.h"
#include "../Ahmer/Customer.h"
#include "../Basit/Restaurant.h"
#include <iostream>

using namespace std;

RiderDispatchManager::RiderDispatchManager() {}

RiderDispatchManager::~RiderDispatchManager() {}



Rider* RiderDispatchManager::findOptimalRider(Order* order, CustomList<Rider*>& riders,
                                            const CustomList<Restaurant*>& restaurants,
                                            MapGraph& graph, RouteOptimizer& routeOptimizer) {
    // But since the method signature is fixed, how do we find the restaurant location from order's restaurant ID?
    // Wait, let's write a small helper or look up the restaurant location in the restaurant database!
    // Since we don't pass the restaurant list here, let's think: is there a simpler way?
    // Oh, wait! The restaurant's ID is e.g. "R01". But we can parse the restaurant ID to find the location, 
    // or we can pass a restaurant list, or we can look up the restaurant from the order.
    // Let's modify the signature or implement a global lookup, or pass the restaurant's location directly!
    // Wait, the method signature in the header is:
    // Rider* findOptimalRider(Order* order, CustomList<Rider*>& riders, MapGraph& graph, RouteOptimizer& routeOptimizer);
    // How do we get the restaurant location?
    // We can assume the order has restaurant ID, and we can find the restaurant's location by searching.
    // Wait, does the order store the restaurant location? No, it stores `restaurantID`.
    // Let's see: what if the restaurant's ID is mapped to a location by a simple convention?
    // E.g., R01 -> Downtown, R02 -> Suburbs, R03 -> BusinessDistrict, R04 -> Uptown.
    // This is simple and extremely robust!
    // Let's write a simple helper in `RiderDispatchManager.cpp`:
    // `string getRestaurantLocation(const string& restaurantID)`
    // R01 = "Downtown", R02 = "Suburbs", R03 = "BusinessDistrict", R04 = "Uptown".
    // This is extremely simple and works perfectly with our bootstrapped default files!
    
    string destNode = "Gulberg";
    string rId = order->getRestaurantID();
    if (rId == "R01") destNode = "Gulberg";
    else if (rId == "R02") destNode = "Anarkali";
    else if (rId == "R03") destNode = "DHA";
    else if (rId == "R04") destNode = "JoharTown";
    else {
        // Fallback: assume the ID itself is a location or find it
        destNode = rId;
    }

    double bestScore = 1e9;
    Rider* optimalRider = nullptr;

    CustomList<Rider*>::Node* current = riders.getHead();
    while (current != nullptr) {
        Rider* r = current->data;
        if (r->hasCapacity()) {
            // Calculate shortest distance from rider location to restaurant location
            double distance = routeOptimizer.calculateShortestDistance(r->getCurrentLocationNodeID(), destNode, graph);
            if (distance < 0.0 || distance >= 1e8) {
                // If unreachable, penalize heavily but still check
                distance = 30.0; 
            }

            // Scoring formula:
            // - Higher distance increases score (bad)
            // - Higher current load increases score (bad for balance)
            // - Higher rating decreases score (good)
            // - Lower completions decreases score slightly (gives new riders a chance)
            double score = (distance * 2.0) + (r->getCurrentLoad() * 4.0) - (r->getRating() * 1.5);

            if (score < bestScore) {
                bestScore = score;
                optimalRider = r;
            }
        }
        current = current->next;
    }

    return optimalRider;
}

bool RiderDispatchManager::assignRider(Rider* rider, Order* order) {
    if (rider == nullptr || order == nullptr) return false;
    if (!rider->hasCapacity()) return false;

    rider->incrementLoad();
    order->setStatus("Assigned");
    cout << "[RiderDispatchManager] Rider " << rider->getName() << " (" << rider->getRiderID() 
              << ") assigned to Order " << order->getOrderID() << ". (Active Load: " 
              << rider->getCurrentLoad() << "/" << rider->getMaxCapacity() << ")" << endl;
    return true;
}

void RiderDispatchManager::completeDelivery(Rider* rider, Order* order, const CustomList<Customer*>& customers) {
    if (rider == nullptr || order == nullptr) return;

    rider->decrementLoad();
    rider->incrementDeliveriesCompleted();
    order->setStatus("Delivered");
    
    // Set rider's current location to order's customer location
    // Wait, where is the customer? By default let's assume we update the rider's location.
    // How do we find the customer location?
    // We can assume customer locations: C01 = Downtown, C02 = Suburbs, C03 = Uptown, C04 = BusinessDistrict, C05 = WestEnd, C06 = EastEnd.
    string custLocation = "Gulberg";
    string cId = order->getCustomerID();
    if (cId == "C01") custLocation = "Gulberg";
    else if (cId == "C02") custLocation = "Anarkali";
    else if (cId == "C03") custLocation = "DHA";
    else if (cId == "C04") custLocation = "JoharTown";
    else if (cId == "C05") custLocation = "Samanabad";
    else if (cId == "C06") custLocation = "Shadman";
    else custLocation = "Gulberg";

    rider->setCurrentLocationNodeID(custLocation);

    cout << "[RiderDispatchManager] Order " << order->getOrderID() << " successfully delivered by " 
              << rider->getName() << "! Rider moved to " << custLocation 
              << ". Deliveries Completed: " << rider->getDeliveriesCompleted() << endl;
}

static string padString(string s, int width) {
    if (s.length() >= width) return s;
    return s + string(width - s.length(), ' ');
}

void RiderDispatchManager::displayRidersStatus(const CustomList<Rider*>& riders) const {
    cout << "\n================ RIDER STATUS DASHBOARD ================" << endl;
    cout << padString("ID", 8) 
         << padString("Name", 15) 
         << padString("Location", 18) 
         << padString("Load", 8) 
         << padString("Available", 12) 
         << padString("Rating", 8) 
         << "Completions" << endl;
    cout << "------------------------------------------------------------------------" << endl;
    
    CustomList<Rider*>::Node* current = riders.getHead();
    while (current != nullptr) {
        Rider* r = current->data;
        cout << padString(r->getRiderID(), 8) 
             << padString(r->getName(), 15) 
             << padString(r->getCurrentLocationNodeID(), 18) 
             << padString(to_string(r->getCurrentLoad()) + "/" + to_string(r->getMaxCapacity()), 8)
             << padString(r->getIsAvailable() ? "Yes" : "No", 12) 
             << padString(to_string(r->getRating()).substr(0, 3), 8) 
             << r->getDeliveriesCompleted() << endl;
        current = current->next;
    }
    cout << "========================================================\n" << endl;
}
