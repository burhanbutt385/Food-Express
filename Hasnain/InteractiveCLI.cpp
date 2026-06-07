#include "InteractiveCLI.h"
#include "../Ahmer/Customer.h"
#include <iostream>
#include <limits>
#include <cstdlib>

InteractiveCLI::InteractiveCLI() : currentSimulationTime(0) {
    // Check files and create defaults if missing
    filePersistence.checkAndBootstrap();
    
    // Load data
    filePersistence.loadMap(mapGraph);
    filePersistence.loadCustomers(customers);
    filePersistence.loadRestaurants(restaurants);
    filePersistence.loadRiders(riders);
    filePersistence.loadOrders(schedulingEngine, currentSimulationTime);

    // Build search indexes
    initializeIndexes();

    // Populate order history timelines for any loaded orders
    CustomList<Order*>::Node* oNode = schedulingEngine.getAllOrders().getHead();
    while (oNode != nullptr) {
        Order* order = oNode->data;
        OrderStatusHistory* hist = new OrderStatusHistory(order->getOrderID(), order->getStatus(), order->getArrivalTime());
        orderHistories.pushBack(hist);
        oNode = oNode->next;
    }
}

InteractiveCLI::~InteractiveCLI() {
    saveAllToDisk();

    // Clean up allocated objects
    while (!customers.isEmpty()) {
        delete customers.getHead()->data;
        customers.popFront();
    }
    while (!restaurants.isEmpty()) {
        delete restaurants.getHead()->data;
        restaurants.popFront();
    }
    while (!riders.isEmpty()) {
        delete riders.getHead()->data;
        riders.popFront();
    }
    while (!orderHistories.isEmpty()) {
        delete orderHistories.getHead()->data;
        orderHistories.popFront();
    }
}

void InteractiveCLI::initializeIndexes() {
    searchEngine.rebuildCustomerIndex(customers);
    searchEngine.rebuildRestaurantIndex(restaurants);
    searchEngine.rebuildRiderIndex(riders);
    searchEngine.rebuildOrderIndex(schedulingEngine.getAllOrders());

    // Populate VIP cache in scheduler
    CustomList<Customer*>::Node* current = customers.getHead();
    while (current != nullptr) {
        schedulingEngine.setCustomerVIPStatus(current->data->getCustomerID(), current->data->getIsVIP());
        current = current->next;
    }
}

void InteractiveCLI::saveAllToDisk() {
    std::cout << "[InteractiveCLI] Saving simulation state to text files..." << std::endl;
    filePersistence.saveMap(mapGraph);
    filePersistence.saveCustomers(customers);
    filePersistence.saveRestaurants(restaurants);
    filePersistence.saveRiders(riders);
    filePersistence.saveOrders(schedulingEngine);
    std::cout << "[InteractiveCLI] Save completed successfully." << std::endl;
}

OrderStatusHistory* InteractiveCLI::getOrCreateHistory(const std::string& orderId) {
    CustomList<OrderStatusHistory*>::Node* current = orderHistories.getHead();
    while (current != nullptr) {
        if (current->data->getOrderID() == orderId) {
            return current->data;
        }
        current = current->next;
    }
    // Create new if not found
    OrderStatusHistory* newHistory = new OrderStatusHistory(orderId, "Placed", currentSimulationTime);
    orderHistories.pushBack(newHistory);
    return newHistory;
}

void InteractiveCLI::addNotification(const std::string& msg) {
    notifications.enqueue(msg);
    if (notifications.size() > 5) {
        notifications.dequeue();
    }
}

static void clearInput() {
    std::cin.clear();
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
}

void InteractiveCLI::start() {
    int choice = 0;
    while (true) {
        system("cls");
        std::cout << "\n=========================================================" << std::endl;
        std::cout << "        FOODEXPRESS DISPATCH OPTIMIZATION ENGINE         " << std::endl;
        std::cout << "        [Simulated System Time: " << currentSimulationTime << " minutes]" << std::endl;
        std::cout << "=========================================================" << std::endl;
        
        if (!notifications.isEmpty()) {
            std::cout << "--- System Notifications ---" << std::endl;
            CustomQueue<std::string> tempQueue;
            while (!notifications.isEmpty()) {
                std::string msg = notifications.getFront();
                std::cout << " > " << msg << std::endl;
                tempQueue.enqueue(msg);
                notifications.dequeue();
            }
            while (!tempQueue.isEmpty()) {
                notifications.enqueue(tempQueue.getFront());
                tempQueue.dequeue();
            }
            std::cout << "---------------------------------------------------------" << std::endl;
        }

        std::cout << "1. Dynamic Order Scheduling" << std::endl;
        std::cout << "2. Kitchen Load Analysis" << std::endl;
        std::cout << "3. Rider Dispatch Optimization" << std::endl;
        std::cout << "4. Route Optimization" << std::endl;
        std::cout << "5. Search and Retrieval Engine" << std::endl;
        std::cout << "6. Order History Tracking" << std::endl;
        std::cout << "7. Performance Analysis" << std::endl;
        std::cout << "8. Scalability Simulation" << std::endl;
        std::cout << "9. System Reports" << std::endl;
        std::cout << "10. Entity Management (Restaurants & Riders)" << std::endl;
        std::cout << "11. Exit & Save" << std::endl;
        std::cout << "---------------------------------------------------------" << std::endl;
        std::cout << "Enter your choice (1-11): ";
        
        if (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter a number." << std::endl;
            clearInput();
            continue;
        }

        if (choice == 11) {
            break;
        }

        switch (choice) {
            case 1: runOrderSchedulingMenu(); break;
            case 2: runKitchenLoadMenu(); break;
            case 3: runRiderDispatchMenu(); break;
            case 4: runRouteOptimizationMenu(); break;
            case 5: runSearchEngineMenu(); break;
            case 6: runOrderHistoryMenu(); break;
            case 7: runPerformanceAnalysisMenu(); break;
            case 8: runScalabilitySimulationMenu(); break;
            case 9: printSystemReports(); break;
            case 10: runEntityManagementMenu(); break;
            default:
                std::cout << "Invalid choice. Please select from options 1-11." << std::endl;
                break;
        }

        if (choice == 8 || choice == 9 || choice > 11) {
            std::cout << "\nPress Enter to return to main menu...";
            std::cin.get();
        }
        
        // Auto-increment simulation clock by 1 minute on each menu operation
        currentSimulationTime++;
        // OPTIMIZATION: Only reschedule every 5 ticks to avoid O(N log N) overhead
        if (currentSimulationTime % 5 == 0) {
            schedulingEngine.rescheduleActiveOrders(currentSimulationTime);
        }
    }
}

void InteractiveCLI::runOrderSchedulingMenu() {
    int subChoice = 0;
    while (true) {
        system("cls");
        std::cout << "\n--- Dynamic Order Scheduling ---" << std::endl;
        std::cout << "1. Insert New Food Order" << std::endl;
        std::cout << "2. Cancel Active Order" << std::endl;
        std::cout << "3. Update Order Priority Score" << std::endl;
        std::cout << "4. Delay Order (Extend Deadline)" << std::endl;
        std::cout << "5. Retrieve Next Processable Order" << std::endl;
        std::cout << "0. Go Back" << std::endl;
        std::cout << "Choose scheduling action: ";
        
        if (!(std::cin >> subChoice)) {
            clearInput();
            continue;
        }
        clearInput();

        if (subChoice == 0) break;

        if (subChoice == 1) {
        std::string orderId, custId, restId;
        int prepTime, deadline;
        
        std::cout << "Enter Order ID (e.g. O10): ";
        std::cin >> orderId;
        std::cout << "Enter Customer ID (e.g. C01): ";
        std::cin >> custId;
        std::cout << "Enter Restaurant ID (e.g. R01): ";
        std::cin >> restId;
        std::cout << "Enter Preparation Time (mins): ";
        std::cin >> prepTime;
        std::cout << "Enter Delivery Deadline (mins): ";
        std::cin >> deadline;
        clearInput();

        // Duplicate check
        if (searchEngine.findOrder(orderId) != nullptr) {
            std::cout << "[Scheduler] ERROR: Order ID " << orderId << " already exists!" << std::endl;
            return;
        }

        // Check customer validation
        Customer* customer = searchEngine.findCustomer(custId);
        bool isVIP = false;
        if (customer != nullptr) {
            isVIP = customer->getIsVIP();
        } else {
            std::cout << "Customer not found. Creating a generic customer account..." << std::endl;
            customers.pushBack(new Customer(custId, "Customer " + custId, "Downtown", false));
            searchEngine.rebuildCustomerIndex(customers);
        }

        // Check restaurant validation
        Restaurant* rest = searchEngine.findRestaurant(restId);
        if (rest == nullptr) {
            std::cout << "Restaurant not found. Defaulting to R01 (BurgerExpress)." << std::endl;
            restId = "R01";
        }

        Order* order = new Order(orderId, custId, restId, prepTime, deadline, currentSimulationTime);
        
        // 1. Kitchen balancing distributes the order first
        Restaurant* finalRest = loadBalancer.assignOrderToKitchen(order, restaurants, true);
        
        if (finalRest != nullptr) {
            // 2. Schedule order
            schedulingEngine.insertOrder(order, isVIP, currentSimulationTime);
            searchEngine.indexOrder(order);
            
            // Record initial transition in timeline
            OrderStatusHistory* history = getOrCreateHistory(orderId);
            history->recordTransition(order, "Queued", currentSimulationTime);
            
            std::cout << "[Scheduler] Order " << orderId << " placed at " << finalRest->getName() 
                      << " and inserted into priority scheduler. (Priority: " 
                      << order->getPriorityScore() << ")" << std::endl;
            addNotification("Order " + orderId + " queued at " + finalRest->getName());
        } else {
            delete order;
            std::cout << "[Scheduler] Failed to allocate restaurant capacity. Order aborted." << std::endl;
        }

    } else if (subChoice == 2) {
        std::string orderId;
        std::cout << "Enter Order ID to cancel: ";
        std::cin >> orderId;
        clearInput();

        Order* o = searchEngine.findOrder(orderId);
        if (o != nullptr) {
            if (schedulingEngine.cancelOrder(orderId)) {
                OrderStatusHistory* history = getOrCreateHistory(orderId);
                history->recordTransition(o, "Cancelled", currentSimulationTime);
                
                // Reduce load on restaurant
                Restaurant* rest = searchEngine.findRestaurant(o->getRestaurantID());
                if (rest != nullptr) {
                    rest->decrementLoad();
                }
                std::cout << "[Scheduler] Order " << orderId << " has been cancelled successfully." << std::endl;
                addNotification("Order " + orderId + " cancelled");
            } else {
                std::cout << "[Scheduler] Order cannot be cancelled (might already be delivered/cancelled)." << std::endl;
            }
        } else {
            std::cout << "[Scheduler] Order ID not found." << std::endl;
        }

    } else if (subChoice == 3) {
        std::string orderId;
        double score;
        std::cout << "Enter Order ID: ";
        std::cin >> orderId;
        std::cout << "Enter New Priority Score: ";
        std::cin >> score;
        clearInput();

        if (schedulingEngine.updateOrderPriority(orderId, score)) {
            std::cout << "[Scheduler] Priority updated successfully." << std::endl;
        } else {
            std::cout << "[Scheduler] Order not found or not currently active." << std::endl;
        }

    } else if (subChoice == 4) {
        std::string orderId;
        int delay;
        std::cout << "Enter Order ID: ";
        std::cin >> orderId;
        std::cout << "Enter delay minutes to add to deadline: ";
        std::cin >> delay;
        clearInput();

        Order* o = searchEngine.findOrder(orderId);
        if (o != nullptr && schedulingEngine.delayOrder(orderId, delay, currentSimulationTime)) {
            OrderStatusHistory* history = getOrCreateHistory(orderId);
            history->recordTransition(o, "Delayed", currentSimulationTime);
            std::cout << "[Scheduler] Order delay applied. New Priority: " << o->getPriorityScore() << std::endl;
        } else {
            std::cout << "[Scheduler] Order not found or not currently active." << std::endl;
        }

    } else if (subChoice == 5) {
        Order* nextOrder = schedulingEngine.getNextProcessableOrder();
        if (nextOrder != nullptr) {
            OrderStatusHistory* history = getOrCreateHistory(nextOrder->getOrderID());
            history->recordTransition(nextOrder, "Accepted", currentSimulationTime);

            std::cout << "[Scheduler] SUCCESS: Retrieved next processable order: " << std::endl;
            std::cout << "  - Order ID  : " << nextOrder->getOrderID() << std::endl;
            std::cout << "  - Customer  : " << nextOrder->getCustomerID() << std::endl;
            std::cout << "  - Restaurant: " << nextOrder->getRestaurantID() << std::endl;
            std::cout << "  - Priority  : " << nextOrder->getPriorityScore() << std::endl;
            std::cout << "  - Prep Time : " << nextOrder->getPreparationTime() << " mins" << std::endl;
        } else {
            std::cout << "[Scheduler] No active orders in priority queue." << std::endl;
        }
    }
    
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}
}

void InteractiveCLI::runKitchenLoadMenu() {
    int subChoice = 0;
    while (true) {
        system("cls");
        std::cout << "\n--- Kitchen Load Analysis ---" << std::endl;
        std::cout << "1. View Kitchen Workload Report" << std::endl;
        std::cout << "2. View Overloaded Kitchens" << std::endl;
        std::cout << "3. Estimate Waiting Time for an Order" << std::endl;
        std::cout << "0. Go Back" << std::endl;
        std::cout << "Choose option: ";
        
        if (!(std::cin >> subChoice)) {
            clearInput();
            continue;
        }
        clearInput();

        if (subChoice == 0) break;

        if (subChoice == 1) {
        loadBalancer.balanceWorkloads(restaurants);
    } else if (subChoice == 2) {
        CustomList<Restaurant*> overloaded = loadBalancer.getOverloadedKitchens(restaurants);
        if (overloaded.isEmpty()) {
            std::cout << "[KitchenLoadBalancer] Great news: No kitchens are currently overloaded!" << std::endl;
        } else {
            std::cout << "[KitchenLoadBalancer] Critical overload in: " << std::endl;
            CustomList<Restaurant*>::Node* node = overloaded.getHead();
            while (node != nullptr) {
                std::cout << "  - " << node->data->getName() << " (" << node->data->getRestaurantID() 
                          << "): Load = " << node->data->getCurrentLoad() << "/" << node->data->getMaxCapacity() << std::endl;
                node = node->next;
            }
        }
    } else if (subChoice == 3) {
        std::string orderId;
        std::cout << "Enter Order ID: ";
        std::cin >> orderId;
        clearInput();

        Order* order = searchEngine.findOrder(orderId);
        if (order != nullptr) {
            int wait = loadBalancer.estimateWaitingTime(order->getRestaurantID(), order, restaurants);
            std::cout << "[KitchenLoadBalancer] Estimated total waiting time at kitchen for Order " 
                      << orderId << " is " << wait << " minutes." << std::endl;
        } else {
            std::cout << "[KitchenLoadBalancer] Order ID not found." << std::endl;
        }
    }
    
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}
}

void InteractiveCLI::runRiderDispatchMenu() {
    int subChoice = 0;
    while (true) {
        system("cls");
        std::cout << "\n--- Rider Dispatch Optimization ---" << std::endl;
        std::cout << "1. View Rider Status Dashboard" << std::endl;
        std::cout << "2. Auto-Dispatch Optimal Rider for Next Processable Order" << std::endl;
        std::cout << "3. Mark Order Delivery Completion" << std::endl;
        std::cout << "4. Toggle Rider Availability" << std::endl;
        std::cout << "0. Go Back" << std::endl;
        std::cout << "Choose option: ";
        
        if (!(std::cin >> subChoice)) {
            clearInput();
            continue;
        }
        clearInput();

        if (subChoice == 0) break;

        if (subChoice == 1) {
        dispatchManager.displayRidersStatus(riders);
    } else if (subChoice == 2) {
        // Look at the top order in the priority queue
        Order* order = schedulingEngine.getActiveOrdersQueue().getMax();
        if (order == nullptr) {
            std::cout << "[DispatchManager] No active orders available for assignment." << std::endl;
            return;
        }

        if (order->getStatus() != "Queued" && order->getStatus() != "Delayed" && order->getStatus() != "Accepted") {
            std::cout << "[DispatchManager] The highest priority order " << order->getOrderID() 
                      << " is in state '" << order->getStatus() << "' and cannot be dispatched." << std::endl;
            return;
        }

        std::cout << "[DispatchManager] Finding optimal rider for Order " << order->getOrderID() 
                  << " (Priority Score: " << order->getPriorityScore() << ")..." << std::endl;
        
        Rider* optimal = dispatchManager.findOptimalRider(order, riders, restaurants, mapGraph, routeOptimizer);
        if (optimal != nullptr) {
            // Assign order
            if (dispatchManager.assignRider(optimal, order)) {
                // Dequeue order from active scheduling heap since it is now assigned to a rider!
                schedulingEngine.getActiveOrdersQueue().remove(order->getOrderID());
                
                OrderStatusHistory* history = getOrCreateHistory(order->getOrderID());
                history->recordTransition(order, "Assigned", currentSimulationTime);
            }
        } else {
            std::cout << "[DispatchManager] WARNING: No available riders have delivery capacity right now!" << std::endl;
        }

    } else if (subChoice == 3) {
        std::string orderId, riderId;
        std::cout << "Enter Order ID that was delivered: ";
        std::cin >> orderId;
        std::cout << "Enter Rider ID who delivered it: ";
        std::cin >> riderId;
        clearInput();

        Order* o = searchEngine.findOrder(orderId);
        Rider* r = searchEngine.findRider(riderId);

        if (o != nullptr && r != nullptr) {
            if (o->getStatus() == "Assigned" || o->getStatus() == "Picked") {
                dispatchManager.completeDelivery(r, o, customers);
                
                // Decrement restaurant load
                Restaurant* rest = searchEngine.findRestaurant(o->getRestaurantID());
                if (rest != nullptr) {
                    rest->decrementLoad();
                }

                OrderStatusHistory* history = getOrCreateHistory(orderId);
                history->recordTransition(o, "Delivered", currentSimulationTime);
                addNotification("Order " + orderId + " delivered by " + r->getName());
            } else {
                std::cout << "[DispatchManager] Order status is '" << o->getStatus() 
                          << "' but must be 'Assigned' or 'Picked' to complete delivery." << std::endl;
            }
        } else {
            std::cout << "[DispatchManager] Order or Rider ID not found." << std::endl;
        }
    } else if (subChoice == 4) {
        std::string riderId;
        std::cout << "Enter Rider ID to toggle availability: ";
        std::cin >> riderId;
        clearInput();

        Rider* r = searchEngine.findRider(riderId);
        if (r != nullptr) {
            r->setIsAvailable(!r->getIsAvailable());
            std::cout << "[DispatchManager] Rider " << r->getName() << " availability set to: " 
                      << (r->getIsAvailable() ? "AVAILABLE" : "UNAVAILABLE") << std::endl;
            addNotification("Rider " + r->getName() + " is now " + (r->getIsAvailable() ? "AVAILABLE" : "UNAVAILABLE"));
        } else {
            std::cout << "[DispatchManager] Rider ID not found." << std::endl;
        }
    }
    
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}
}

void InteractiveCLI::runRouteOptimizationMenu() {
    int subChoice = 0;
    while (true) {
        system("cls");
        std::cout << "\n--- Route Optimization ---" << std::endl;
        std::cout << "1. Calculate Shortest Path between locations" << std::endl;
        std::cout << "2. Block a Road (Dynamic Event)" << std::endl;
        std::cout << "3. Unblock a Road" << std::endl;
        std::cout << "4. Update Traffic Delay Multiplier" << std::endl;
        std::cout << "0. Go Back" << std::endl;
        std::cout << "Choose option: ";
        
        if (!(std::cin >> subChoice)) {
            clearInput();
            continue;
        }
        clearInput();

        if (subChoice == 0) break;

        if (subChoice == 1) {
        std::string start, end;
        std::cout << "Enter Start Node ID (e.g. Downtown): ";
        std::cin >> start;
        std::cout << "Enter End Node ID (e.g. Airport): ";
        std::cin >> end;
        clearInput();

        if (!mapGraph.locationExists(start) || !mapGraph.locationExists(end)) {
            std::cout << "[RouteOptimizer] Invalid Start or End Node ID." << std::endl;
        } else {
            routeOptimizer.printRouteDetails(start, end, mapGraph);
        }

    } else if (subChoice == 2) {
        std::string src, dest;
        std::cout << "Enter Start Node: ";
        std::cin >> src;
        std::cout << "Enter End Node: ";
        std::cin >> dest;
        clearInput();

        if (mapGraph.blockRoad(src, dest)) {
            std::cout << "[RouteOptimizer] Road block applied between " << src << " and " << dest << "!" << std::endl;
        } else {
            std::cout << "[RouteOptimizer] Road connection not found." << std::endl;
        }

    } else if (subChoice == 3) {
        std::string src, dest;
        std::cout << "Enter Start Node: ";
        std::cin >> src;
        std::cout << "Enter End Node: ";
        std::cin >> dest;
        clearInput();

        if (mapGraph.unblockRoad(src, dest)) {
            std::cout << "[RouteOptimizer] Road block cleared between " << src << " and " << dest << "." << std::endl;
        } else {
            std::cout << "[RouteOptimizer] Road connection not found." << std::endl;
        }

    } else if (subChoice == 4) {
        std::string src, dest;
        double multiplier;
        std::cout << "Enter Start Node: ";
        std::cin >> src;
        std::cout << "Enter End Node: ";
        std::cin >> dest;
        std::cout << "Enter Traffic Multiplier (1.0 = clear, 2.5 = heavy): ";
        std::cin >> multiplier;
        clearInput();

        if (mapGraph.setTrafficFactor(src, dest, multiplier)) {
            std::cout << "[RouteOptimizer] Traffic updated. Route calculations will adapt dynamically." << std::endl;
        } else {
            std::cout << "[RouteOptimizer] Road connection not found." << std::endl;
        }
    }
    
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}
}

void InteractiveCLI::runSearchEngineMenu() {
    int subChoice = 0;
    while (true) {
        system("cls");
        std::cout << "\n--- Search and Retrieval Engine (O(1) Indexed) ---" << std::endl;
        std::cout << "1. Lookup Order by ID" << std::endl;
        std::cout << "2. Lookup Rider by ID" << std::endl;
        std::cout << "3. Filter Orders by Customer ID" << std::endl;
        std::cout << "4. Show All Available Riders" << std::endl;
        std::cout << "5. Show Loaded Restaurants (Threshold)" << std::endl;
        std::cout << "0. Go Back" << std::endl;
        std::cout << "Choose option: ";
        
        if (!(std::cin >> subChoice)) {
            clearInput();
            continue;
        }
        clearInput();

        if (subChoice == 0) break;

        if (subChoice == 1) {
        std::string orderId;
        std::cout << "Enter Order ID: ";
        std::cin >> orderId;
        clearInput();

        Order* o = searchEngine.findOrder(orderId);
        if (o != nullptr) {
            std::cout << "Order Found: " << std::endl;
            std::cout << "  - ID: " << o->getOrderID() << std::endl;
            std::cout << "  - Customer ID: " << o->getCustomerID() << std::endl;
            std::cout << "  - Restaurant ID: " << o->getRestaurantID() << std::endl;
            std::cout << "  - Priority: " << o->getPriorityScore() << std::endl;
            std::cout << "  - Status: " << o->getStatus() << std::endl;
            std::cout << "  - Prep/Deadline: " << o->getPreparationTime() << "/" << o->getDeliveryDeadline() << " min" << std::endl;
        } else {
            std::cout << "Order ID not found." << std::endl;
        }

    } else if (subChoice == 2) {
        std::string riderId;
        std::cout << "Enter Rider ID: ";
        std::cin >> riderId;
        clearInput();

        Rider* r = searchEngine.findRider(riderId);
        if (r != nullptr) {
            std::cout << "Rider Found: " << r->getName() << std::endl;
            std::cout << "  - Location: " << r->getCurrentLocationNodeID() << std::endl;
            std::cout << "  - Load/Capacity: " << r->getCurrentLoad() << "/" << r->getMaxCapacity() << std::endl;
            std::cout << "  - Rating: " << r->getRating() << std::endl;
            std::cout << "  - Completed Deliveries: " << r->getDeliveriesCompleted() << std::endl;
        } else {
            std::cout << "Rider ID not found." << std::endl;
        }

    } else if (subChoice == 3) {
        std::string custId;
        std::cout << "Enter Customer ID: ";
        std::cin >> custId;
        clearInput();

        CustomList<Order*> results = searchEngine.getOrdersByCustomer(custId);
        if (results.isEmpty()) {
            std::cout << "No orders found for customer " << custId << "." << std::endl;
        } else {
            std::cout << "Found " << results.size() << " orders:" << std::endl;
            CustomList<Order*>::Node* node = results.getHead();
            while (node != nullptr) {
                std::cout << "  - " << node->data->getOrderID() << ": Status=" << node->data->getStatus() 
                          << ", Priority=" << node->data->getPriorityScore() << std::endl;
                node = node->next;
            }
        }

    } else if (subChoice == 4) {
        CustomList<Rider*> available = searchEngine.getAvailableRiders();
        if (available.isEmpty()) {
            std::cout << "No riders are currently available." << std::endl;
        } else {
            std::cout << "Available Riders:" << std::endl;
            CustomList<Rider*>::Node* node = available.getHead();
            while (node != nullptr) {
                std::cout << "  - " << node->data->getName() << " (" << node->data->getRiderID() 
                          << "): Location=" << node->data->getCurrentLocationNodeID() 
                          << ", Active Load=" << node->data->getCurrentLoad() << "/" << node->data->getMaxCapacity() << std::endl;
                node = node->next;
            }
        }

    } else if (subChoice == 5) {
        double threshold = 0.7;
        std::cout << "Enter workload threshold ratio (e.g. 0.7 = 70%): ";
        std::cin >> threshold;
        clearInput();

        CustomList<Restaurant*> loaded = searchEngine.getOverloadedRestaurants(threshold);
        if (loaded.isEmpty()) {
            std::cout << "No restaurants exceed a workload ratio of " << threshold << "." << std::endl;
        } else {
            std::cout << "Restaurants exceeding " << threshold << " workload ratio:" << std::endl;
            CustomList<Restaurant*>::Node* node = loaded.getHead();
            while (node != nullptr) {
                std::cout << "  - " << node->data->getName() << " (" << node->data->getRestaurantID() 
                          << "): Ratio=" << node->data->getWorkloadRatio() 
                          << " (Load: " << node->data->getCurrentLoad() << "/" << node->data->getMaxCapacity() << ")" << std::endl;
                node = node->next;
            }
        }
    }
    
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}
}

void InteractiveCLI::runOrderHistoryMenu() {
    int subChoice = 0;
    while (true) {
        system("cls");
        std::cout << "\n--- Order History and Tracking ---" << std::endl;
        std::cout << "1. Replay Order Status Timeline" << std::endl;
        std::cout << "2. Undo Last Status Transition (Revert State)" << std::endl;
        std::cout << "0. Go Back" << std::endl;
        std::cout << "Choose option: ";
        
        if (!(std::cin >> subChoice)) {
            clearInput();
            continue;
        }
        clearInput();

        if (subChoice == 0) break;

        std::string orderId;
    std::cout << "Enter Order ID: ";
    std::cin >> orderId;
    clearInput();

    Order* o = searchEngine.findOrder(orderId);
    OrderStatusHistory* hist = getOrCreateHistory(orderId);

    if (o == nullptr) {
        std::cout << "Order ID not found." << std::endl;
        std::cout << "\nPress Enter to continue...";
        std::cin.get();
        continue;
    }

    if (subChoice == 1) {
        hist->replayTimeline();
    } else if (subChoice == 2) {
        // If the order was popped from the scheduler heap because it was Assigned or Delivered,
        // and we revert it, we should re-insert it back into the heap!
        std::string initialStatus = o->getStatus();
        if (hist->undoLastTransition(o, currentSimulationTime)) {
            std::string finalStatus = o->getStatus();
            
            // Check if we need to put it back in the scheduling queue
            if ((initialStatus == "Assigned" || initialStatus == "Delivered") && 
                (finalStatus == "Queued" || finalStatus == "Delayed" || finalStatus == "Accepted")) {
                
                // If it was delivered, we decrement completions and restaurant load
                if (initialStatus == "Delivered") {
                    Restaurant* r = searchEngine.findRestaurant(o->getRestaurantID());
                    if (r != nullptr) r->incrementLoad();
                }
                
                // Put back in active queue
                schedulingEngine.getActiveOrdersQueue().insert(o);
                std::cout << "[OrderStatusHistory] Order re-inserted into active scheduling queue." << std::endl;
            }
        }
    }
        
        std::cout << "\nPress Enter to continue...";
        std::cin.get();
    }
}

void InteractiveCLI::runPerformanceAnalysisMenu() {
    int subChoice = 0;
    while (true) {
        system("cls");
        std::cout << "\n--- Performance Analysis ---" << std::endl;
        std::cout << "1. Run Scalability Benchmarks (Small, Medium, Large sizes)" << std::endl;
        std::cout << "2. View Theoretical Complexity Report & Justifications" << std::endl;
        std::cout << "0. Go Back" << std::endl;
        std::cout << "Choose option: ";
        
        if (!(std::cin >> subChoice)) {
            clearInput();
            continue;
        }
        clearInput();

        if (subChoice == 0) break;

        PerformanceAnalyzer analyzer;
    if (subChoice == 1) {
        analyzer.runScalabilitySimulation();
    } else if (subChoice == 2) {
        analyzer.printTheoreticalComplexityReport();
    }
    
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}
}

void InteractiveCLI::runScalabilitySimulationMenu() {
    std::cout << "\n--- Run Batch Scalability Simulation ---" << std::endl;
    std::cout << "Running bulk auto-dispatch simulation..." << std::endl;

    // Simulate bulk operations:
    // 1. Pop next order from heap
    // 2. Dispatch optimal rider
    // 3. Mark delivery completion
    // Repeat this V times and print stats.
    
    int processedCount = 0;
    while (!schedulingEngine.getActiveOrdersQueue().isEmpty()) {
        Order* order = schedulingEngine.getNextProcessableOrder();
        if (order != nullptr) {
            OrderStatusHistory* history = getOrCreateHistory(order->getOrderID());
            history->recordTransition(order, "Accepted", currentSimulationTime);

            Rider* optimal = dispatchManager.findOptimalRider(order, riders, restaurants, mapGraph, routeOptimizer);
            if (optimal != nullptr) {
                dispatchManager.assignRider(optimal, order);
                history->recordTransition(order, "Assigned", currentSimulationTime);

                // Auto complete delivery for test
                dispatchManager.completeDelivery(optimal, order, customers);
                history->recordTransition(order, "Delivered", currentSimulationTime);
                processedCount++;
            } else {
                // Re-heapify order
                schedulingEngine.getActiveOrdersQueue().insert(order);
                std::cout << "[Simulation] All riders are currently busy. Pausing batch simulation." << std::endl;
                break;
            }
        }
    }

    std::cout << "[Simulation] Batch run complete. Processed " << processedCount << " orders to final delivery." << std::endl;
}

void InteractiveCLI::printSystemReports() {
    std::cout << "\n================= FOODEXPRESS SYSTEM REPORT =================" << std::endl;
    std::cout << "Locations in City Map : " << mapGraph.getVertexCount() << std::endl;
    std::cout << "Registered Customers  : " << customers.size() << std::endl;
    std::cout << "Partner Restaurants   : " << restaurants.size() << std::endl;
    std::cout << "Active Delivery Riders: " << riders.size() << std::endl;
    
    int activeOrders = schedulingEngine.getActiveOrdersQueue().size();
    int totalOrders = schedulingEngine.getAllOrders().size();
    
    std::cout << "Active Scheduled Orders: " << activeOrders << std::endl;
    std::cout << "Total Sim Orders Seeded: " << totalOrders << std::endl;
    
    std::cout << "\nBlocked Roads List: " << std::endl;
    bool foundBlocked = false;
    for (int i = 0; i < mapGraph.getVertexCount(); i++) {
        std::string src = mapGraph.getVertexIdAt(i);
        Edge* edge = mapGraph.getEdgesFrom(src);
        while (edge != nullptr) {
            if (edge->isBlocked && src < edge->destinationNodeID) {
                std::cout << "  - " << src << " <-> " << edge->destinationNodeID << std::endl;
                foundBlocked = true;
            }
            edge = edge->next;
        }
    }
    if (!foundBlocked) {
        std::cout << "  - None" << std::endl;
    }
    std::cout << "=============================================================\n" << std::endl;
}

void InteractiveCLI::runEntityManagementMenu() {
    int subChoice = 0;
    while (true) {
        system("cls");
        std::cout << "\n--- Entity Management ---" << std::endl;
        std::cout << "1. Register New Restaurant" << std::endl;
        std::cout << "2. Register New Rider" << std::endl;
        std::cout << "0. Go Back" << std::endl;
        std::cout << "Choose option: ";
        
        if (!(std::cin >> subChoice)) {
            clearInput();
            continue;
        }
        clearInput();

        if (subChoice == 0) break;

        if (subChoice == 1) {
        std::string id, name, location;
        int capacity;
        std::cout << "Enter Restaurant ID (e.g. R99): ";
        std::cin >> id;
        std::cout << "Enter Name: ";
        std::cin.ignore();
        std::getline(std::cin, name);
        std::cout << "Enter Location Node ID (e.g. Downtown): ";
        std::cin >> location;
        std::cout << "Enter Max Kitchen Capacity: ";
        std::cin >> capacity;
        clearInput();

        if (searchEngine.findRestaurant(id) != nullptr) {
            std::cout << "Restaurant ID already exists!" << std::endl;
            return;
        }
        if (!mapGraph.locationExists(location)) {
            std::cout << "Warning: Location node does not exist in graph. Will fallback to default during dispatch." << std::endl;
        }

        Restaurant* r = new Restaurant(id, name, location, capacity, 0);
        restaurants.pushBack(r);
        searchEngine.indexRestaurant(r);
        std::cout << "Restaurant " << name << " registered successfully." << std::endl;
        addNotification("New Restaurant added: " + name);

    } else if (subChoice == 2) {
        std::string id, name, location;
        int capacity;
        std::cout << "Enter Rider ID (e.g. D99): ";
        std::cin >> id;
        std::cout << "Enter Name: ";
        std::cin.ignore();
        std::getline(std::cin, name);
        std::cout << "Enter Starting Location Node ID: ";
        std::cin >> location;
        std::cout << "Enter Carrying Capacity: ";
        std::cin >> capacity;
        clearInput();

        if (searchEngine.findRider(id) != nullptr) {
            std::cout << "Rider ID already exists!" << std::endl;
            return;
        }

        Rider* r = new Rider(id, name, location, capacity, 0, true, 5.0, 0);
        riders.pushBack(r);
        searchEngine.indexRider(r);
        std::cout << "Rider " << name << " registered successfully." << std::endl;
        addNotification("New Rider added: " + name);
    }
    
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}
}
