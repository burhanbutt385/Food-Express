#include "KitchenLoadBalancer.h"
#include <iostream>

KitchenLoadBalancer::KitchenLoadBalancer() {}

KitchenLoadBalancer::~KitchenLoadBalancer() {}

Restaurant* KitchenLoadBalancer::assignOrderToKitchen(Order* order, CustomList<Restaurant*>& restaurants, bool autoRebalance) {
    std::string targetRestId = order->getRestaurantID();
    Restaurant* originalRestaurant = nullptr;

    // Find original restaurant
    CustomList<Restaurant*>::Node* current = restaurants.getHead();
    while (current != nullptr) {
        if (current->data->getRestaurantID() == targetRestId) {
            originalRestaurant = current->data;
            break;
        }
        current = current->next;
    }

    if (originalRestaurant == nullptr) {
        std::cout << "[KitchenLoadBalancer] Error: Restaurant " << targetRestId << " not found!" << std::endl;
        return nullptr;
    }

    // Check if the original restaurant is overloaded
    if (originalRestaurant->isOverloaded()) {
        std::cout << "[KitchenLoadBalancer] WARNING: Kitchen " << originalRestaurant->getName() 
                  << " (" << originalRestaurant->getRestaurantID() << ") is OVERLOADED! (Load: " 
                  << originalRestaurant->getCurrentLoad() << "/" << originalRestaurant->getMaxCapacity() << ")" << std::endl;

        if (autoRebalance) {
            // Find a sister restaurant with the lowest workload ratio that has spare capacity
            Restaurant* alternativeRestaurant = nullptr;
            double lowestRatio = 1.0; // Must be under 1.0 workload ratio (not overloaded)

            current = restaurants.getHead();
            while (current != nullptr) {
                Restaurant* r = current->data;
                if (r->getRestaurantID() != targetRestId && !r->isOverloaded()) {
                    double ratio = r->getWorkloadRatio();
                    if (ratio < lowestRatio) {
                        lowestRatio = ratio;
                        alternativeRestaurant = r;
                    }
                }
                current = current->next;
            }

            if (alternativeRestaurant != nullptr) {
                std::cout << "[KitchenLoadBalancer] ACTION: Rebalanced Order " << order->getOrderID() 
                          << " from " << originalRestaurant->getName() << " to " 
                          << alternativeRestaurant->getName() << " (" << alternativeRestaurant->getRestaurantID() 
                          << ") to ease workload." << std::endl;

                // Reassign order
                order->setDeliveryDeadline(order->getDeliveryDeadline() + 5); // Add a small delay due to rerouting
                order->setStatus("Rerouted");
                alternativeRestaurant->incrementLoad();
                return alternativeRestaurant;
            } else {
                std::cout << "[KitchenLoadBalancer] NOTICE: No alternative kitchen has spare capacity. Order remains at " 
                          << originalRestaurant->getName() << "." << std::endl;
            }
        }
    }

    // If we didn't rebalance, allocate to original
    originalRestaurant->incrementLoad();
    return originalRestaurant;
}

int KitchenLoadBalancer::estimateWaitingTime(const std::string& restaurantId, Order* order, const CustomList<Restaurant*>& restaurants) const {
    CustomList<Restaurant*>::Node* current = restaurants.getHead();
    while (current != nullptr) {
        if (current->data->getRestaurantID() == restaurantId) {
            Restaurant* r = current->data;
            // Estimated wait time: base prep time + 5 minutes for each active order currently queued/preparing
            return order->getPreparationTime() + (r->getCurrentLoad() * 4);
        }
        current = current->next;
    }
    return order->getPreparationTime();
}

CustomList<Restaurant*> KitchenLoadBalancer::getOverloadedKitchens(const CustomList<Restaurant*>& restaurants) const {
    CustomList<Restaurant*> overloadedList;
    CustomList<Restaurant*>::Node* current = restaurants.getHead();
    while (current != nullptr) {
        if (current->data->isOverloaded()) {
            overloadedList.pushBack(current->data);
        }
        current = current->next;
    }
    return overloadedList;
}

void KitchenLoadBalancer::balanceWorkloads(CustomList<Restaurant*>& restaurants) {
    // Check if any kitchen is highly overloaded and others are empty, print report suggestions
    std::cout << "\n================ KITCHEN WORKLOAD REPORT ================" << std::endl;
    CustomList<Restaurant*>::Node* current = restaurants.getHead();
    while (current != nullptr) {
        Restaurant* r = current->data;
        double ratio = r->getWorkloadRatio();
        std::cout << "- " << r->getName() << " (" << r->getRestaurantID() << "): " 
                  << r->getCurrentLoad() << "/" << r->getMaxCapacity() << " Active Orders (";
        if (ratio >= 1.0) {
            std::cout << "OVERLOADED - CRITICAL";
        } else if (ratio >= 0.7) {
            std::cout << "HIGH LOAD";
        } else if (ratio >= 0.4) {
            std::cout << "MODERATE LOAD";
        } else {
            std::cout << "NORMAL LOAD";
        }
        std::cout << ")" << std::endl;
        current = current->next;
    }
    std::cout << "=========================================================\n" << std::endl;
}
