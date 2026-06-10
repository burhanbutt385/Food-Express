#include "InteractiveCLI.h"
#include "../Ahmer/Customer.h"
#include <iostream>

#include <cstdlib>
using namespace std;

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
    cout << "[InteractiveCLI] Saving simulation state to text files..." << endl;
    filePersistence.saveMap(mapGraph);
    filePersistence.saveCustomers(customers);
    filePersistence.saveRestaurants(restaurants);
    filePersistence.saveRiders(riders);
    filePersistence.saveOrders(schedulingEngine);
    cout << "[InteractiveCLI] Save completed successfully." << endl;
}

OrderStatusHistory* InteractiveCLI::getOrCreateHistory(const string& orderId) {
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

void InteractiveCLI::addNotification(const string& msg) {
    notifications.enqueue(msg);
    if (notifications.size() > 5) {
        notifications.dequeue();
    }
}

static void clearInput() {
    cin.clear();
    cin.ignore(10000, '\n');
}

void InteractiveCLI::start() {
    int choice = 0;
    while (true) {
        system("cls");
        cout << "\n=========================================================" << endl;
        cout << "        FOODEXPRESS DISPATCH OPTIMIZATION ENGINE         " << endl;
        cout << "        [Simulated System Time: " << currentSimulationTime << " minutes]" << endl;
        cout << "=========================================================" << endl;
        
        if (!notifications.isEmpty()) {
            cout << "--- System Notifications ---" << endl;
            CustomQueue<string> tempQueue;
            while (!notifications.isEmpty()) {
                string msg = notifications.getFront();
                cout << " > " << msg << endl;
                tempQueue.enqueue(msg);
                notifications.dequeue();
            }
            while (!tempQueue.isEmpty()) {
                notifications.enqueue(tempQueue.getFront());
                tempQueue.dequeue();
            }
            cout << "---------------------------------------------------------" << endl;
        }

        cout << "1. Dynamic Order Scheduling" << endl;
        cout << "2. Kitchen Load Analysis" << endl;
        cout << "3. Rider Dispatch Optimization" << endl;
        cout << "4. Route Optimization" << endl;
        cout << "5. Search and Retrieval Engine" << endl;
        cout << "6. Order History Tracking" << endl;
        cout << "7. Performance Analysis" << endl;
        cout << "8. Scalability Simulation" << endl;
        cout << "9. System Reports" << endl;
        cout << "10. Entity Management (Restaurants & Riders)" << endl;
        cout << "11. Exit & Save" << endl;
        cout << "---------------------------------------------------------" << endl;
        cout << "Enter your choice (1-11): ";
        
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number." << endl;
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
                cout << "Invalid choice. Please select from options 1-11." << endl;
                break;
        }

        if (choice == 8 || choice == 9 || choice > 11) {
            cout << "\nPress Enter to return to main menu...";
            cin.get();
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
        cout << "\n--- Dynamic Order Scheduling ---" << endl;
        cout << "1. Insert New Food Order" << endl;
        cout << "2. Cancel Active Order" << endl;
        cout << "3. Update Order Priority Score" << endl;
        cout << "4. Delay Order (Extend Deadline)" << endl;
        cout << "5. Retrieve Next Processable Order" << endl;
        cout << "0. Go Back" << endl;
        cout << "Choose scheduling action: ";
        
        if (!(cin >> subChoice)) {
            clearInput();
            continue;
        }
        clearInput();

        if (subChoice == 0) break;

        if (subChoice == 1) {
        string orderId, custId, restId;
        int prepTime, deadline;
        
        cout << "Enter Order ID (e.g. O10): ";
        cin >> orderId;
        cout << "Enter Customer ID (e.g. C01): ";
        cin >> custId;
        cout << "Enter Restaurant ID (e.g. R01): ";
        cin >> restId;
        cout << "Enter Preparation Time (mins): ";
        cin >> prepTime;
        cout << "Enter Delivery Deadline (mins): ";
        cin >> deadline;
        clearInput();

        // Duplicate check
        if (searchEngine.findOrder(orderId) != nullptr) {
            cout << "[Scheduler] ERROR: Order ID " << orderId << " already exists!" << endl;
            return;
        }

        // Check customer validation
        Customer* customer = searchEngine.findCustomer(custId);
        bool isVIP = false;
        if (customer != nullptr) {
            isVIP = customer->getIsVIP();
        } else {
            cout << "Customer not found. Creating a generic customer account..." << endl;
            customers.pushBack(new Customer(custId, "Customer " + custId, "Downtown", false));
            searchEngine.rebuildCustomerIndex(customers);
        }

        // Check restaurant validation
        Restaurant* rest = searchEngine.findRestaurant(restId);
        if (rest == nullptr) {
            cout << "Restaurant not found. Defaulting to R01 (BurgerExpress)." << endl;
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
            
            cout << "[Scheduler] Order " << orderId << " placed at " << finalRest->getName() 
                      << " and inserted into priority scheduler. (Priority: " 
                      << order->getPriorityScore() << ")" << endl;
            addNotification("Order " + orderId + " queued at " + finalRest->getName());
        } else {
            delete order;
            cout << "[Scheduler] Failed to allocate restaurant capacity. Order aborted." << endl;
        }

    } else if (subChoice == 2) {
        string orderId;
        cout << "Enter Order ID to cancel: ";
        cin >> orderId;
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
                cout << "[Scheduler] Order " << orderId << " has been cancelled successfully." << endl;
                addNotification("Order " + orderId + " cancelled");
            } else {
                cout << "[Scheduler] Order cannot be cancelled (might already be delivered/cancelled)." << endl;
            }
        } else {
            cout << "[Scheduler] Order ID not found." << endl;
        }

    } else if (subChoice == 3) {
        string orderId;
        double score;
        cout << "Enter Order ID: ";
        cin >> orderId;
        cout << "Enter New Priority Score: ";
        cin >> score;
        clearInput();

        if (schedulingEngine.updateOrderPriority(orderId, score)) {
            cout << "[Scheduler] Priority updated successfully." << endl;
        } else {
            cout << "[Scheduler] Order not found or not currently active." << endl;
        }

    } else if (subChoice == 4) {
        string orderId;
        int delay;
        cout << "Enter Order ID: ";
        cin >> orderId;
        cout << "Enter delay minutes to add to deadline: ";
        cin >> delay;
        clearInput();

        Order* o = searchEngine.findOrder(orderId);
        if (o != nullptr && schedulingEngine.delayOrder(orderId, delay, currentSimulationTime)) {
            OrderStatusHistory* history = getOrCreateHistory(orderId);
            history->recordTransition(o, "Delayed", currentSimulationTime);
            cout << "[Scheduler] Order delay applied. New Priority: " << o->getPriorityScore() << endl;
        } else {
            cout << "[Scheduler] Order not found or not currently active." << endl;
        }

    } else if (subChoice == 5) {
        Order* nextOrder = schedulingEngine.getNextProcessableOrder();
        if (nextOrder != nullptr) {
            OrderStatusHistory* history = getOrCreateHistory(nextOrder->getOrderID());
            history->recordTransition(nextOrder, "Accepted", currentSimulationTime);

            cout << "[Scheduler] SUCCESS: Retrieved next processable order: " << endl;
            cout << "  - Order ID  : " << nextOrder->getOrderID() << endl;
            cout << "  - Customer  : " << nextOrder->getCustomerID() << endl;
            cout << "  - Restaurant: " << nextOrder->getRestaurantID() << endl;
            cout << "  - Priority  : " << nextOrder->getPriorityScore() << endl;
            cout << "  - Prep Time : " << nextOrder->getPreparationTime() << " mins" << endl;
        } else {
            cout << "[Scheduler] No active orders in priority queue." << endl;
        }
    }
    
    cout << "\nPress Enter to continue...";
    cin.get();
}
}

void InteractiveCLI::runKitchenLoadMenu() {
    int subChoice = 0;
    while (true) {
        system("cls");
        cout << "\n--- Kitchen Load Analysis ---" << endl;
        cout << "1. View Kitchen Workload Report" << endl;
        cout << "2. View Overloaded Kitchens" << endl;
        cout << "3. Estimate Waiting Time for an Order" << endl;
        cout << "0. Go Back" << endl;
        cout << "Choose option: ";
        
        if (!(cin >> subChoice)) {
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
            cout << "[KitchenLoadBalancer] Great news: No kitchens are currently overloaded!" << endl;
        } else {
            cout << "[KitchenLoadBalancer] Critical overload in: " << endl;
            CustomList<Restaurant*>::Node* node = overloaded.getHead();
            while (node != nullptr) {
                cout << "  - " << node->data->getName() << " (" << node->data->getRestaurantID() 
                          << "): Load = " << node->data->getCurrentLoad() << "/" << node->data->getMaxCapacity() << endl;
                node = node->next;
            }
        }
    } else if (subChoice == 3) {
        string orderId;
        cout << "Enter Order ID: ";
        cin >> orderId;
        clearInput();

        Order* order = searchEngine.findOrder(orderId);
        if (order != nullptr) {
            int wait = loadBalancer.estimateWaitingTime(order->getRestaurantID(), order, restaurants);
            cout << "[KitchenLoadBalancer] Estimated total waiting time at kitchen for Order " 
                      << orderId << " is " << wait << " minutes." << endl;
        } else {
            cout << "[KitchenLoadBalancer] Order ID not found." << endl;
        }
    }
    
    cout << "\nPress Enter to continue...";
    cin.get();
}
}

void InteractiveCLI::runRiderDispatchMenu() {
    int subChoice = 0;
    while (true) {
        system("cls");
        cout << "\n--- Rider Dispatch Optimization ---" << endl;
        cout << "1. View Rider Status Dashboard" << endl;
        cout << "2. Auto-Dispatch Optimal Rider for Next Processable Order" << endl;
        cout << "3. Mark Order Delivery Completion" << endl;
        cout << "4. Toggle Rider Availability" << endl;
        cout << "0. Go Back" << endl;
        cout << "Choose option: ";
        
        if (!(cin >> subChoice)) {
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
            cout << "[DispatchManager] No active orders available for assignment." << endl;
            return;
        }

        if (order->getStatus() != "Queued" && order->getStatus() != "Delayed" && order->getStatus() != "Accepted") {
            cout << "[DispatchManager] The highest priority order " << order->getOrderID() 
                      << " is in state '" << order->getStatus() << "' and cannot be dispatched." << endl;
            return;
        }

        cout << "[DispatchManager] Finding optimal rider for Order " << order->getOrderID() 
                  << " (Priority Score: " << order->getPriorityScore() << ")..." << endl;
        
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
            cout << "[DispatchManager] WARNING: No available riders have delivery capacity right now!" << endl;
        }

    } else if (subChoice == 3) {
        string orderId, riderId;
        cout << "Enter Order ID that was delivered: ";
        cin >> orderId;
        cout << "Enter Rider ID who delivered it: ";
        cin >> riderId;
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
                cout << "[DispatchManager] Order status is '" << o->getStatus() 
                          << "' but must be 'Assigned' or 'Picked' to complete delivery." << endl;
            }
        } else {
            cout << "[DispatchManager] Order or Rider ID not found." << endl;
        }
    } else if (subChoice == 4) {
        string riderId;
        cout << "Enter Rider ID to toggle availability: ";
        cin >> riderId;
        clearInput();

        Rider* r = searchEngine.findRider(riderId);
        if (r != nullptr) {
            r->setIsAvailable(!r->getIsAvailable());
            cout << "[DispatchManager] Rider " << r->getName() << " availability set to: " 
                      << (r->getIsAvailable() ? "AVAILABLE" : "UNAVAILABLE") << endl;
            addNotification("Rider " + r->getName() + " is now " + (r->getIsAvailable() ? "AVAILABLE" : "UNAVAILABLE"));
        } else {
            cout << "[DispatchManager] Rider ID not found." << endl;
        }
    }
    
    cout << "\nPress Enter to continue...";
    cin.get();
}
}

void InteractiveCLI::runRouteOptimizationMenu() {
    int subChoice = 0;
    while (true) {
        system("cls");
        cout << "\n--- Route Optimization ---" << endl;
        cout << "1. Calculate Shortest Path between locations" << endl;
        cout << "2. Block a Road (Dynamic Event)" << endl;
        cout << "3. Unblock a Road" << endl;
        cout << "4. Update Traffic Delay Multiplier" << endl;
        cout << "0. Go Back" << endl;
        cout << "Choose option: ";
        
        if (!(cin >> subChoice)) {
            clearInput();
            continue;
        }
        clearInput();

        if (subChoice == 0) break;

        if (subChoice == 1) {
        string start, end;
        cout << "Enter Start Node ID (e.g. Downtown): ";
        cin >> start;
        cout << "Enter End Node ID (e.g. Airport): ";
        cin >> end;
        clearInput();

        if (!mapGraph.locationExists(start) || !mapGraph.locationExists(end)) {
            cout << "[RouteOptimizer] Invalid Start or End Node ID." << endl;
        } else {
            routeOptimizer.printRouteDetails(start, end, mapGraph);
        }

    } else if (subChoice == 2) {
        string src, dest;
        cout << "Enter Start Node: ";
        cin >> src;
        cout << "Enter End Node: ";
        cin >> dest;
        clearInput();

        if (mapGraph.blockRoad(src, dest)) {
            cout << "[RouteOptimizer] Road block applied between " << src << " and " << dest << "!" << endl;
        } else {
            cout << "[RouteOptimizer] Road connection not found." << endl;
        }

    } else if (subChoice == 3) {
        string src, dest;
        cout << "Enter Start Node: ";
        cin >> src;
        cout << "Enter End Node: ";
        cin >> dest;
        clearInput();

        if (mapGraph.unblockRoad(src, dest)) {
            cout << "[RouteOptimizer] Road block cleared between " << src << " and " << dest << "." << endl;
        } else {
            cout << "[RouteOptimizer] Road connection not found." << endl;
        }

    } else if (subChoice == 4) {
        string src, dest;
        double multiplier;
        cout << "Enter Start Node: ";
        cin >> src;
        cout << "Enter End Node: ";
        cin >> dest;
        cout << "Enter Traffic Multiplier (1.0 = clear, 2.5 = heavy): ";
        cin >> multiplier;
        clearInput();

        if (mapGraph.setTrafficFactor(src, dest, multiplier)) {
            cout << "[RouteOptimizer] Traffic updated. Route calculations will adapt dynamically." << endl;
        } else {
            cout << "[RouteOptimizer] Road connection not found." << endl;
        }
    }
    
    cout << "\nPress Enter to continue...";
    cin.get();
}
}

void InteractiveCLI::runSearchEngineMenu() {
    int subChoice = 0;
    while (true) {
        system("cls");
        cout << "\n--- Search and Retrieval Engine (O(1) Indexed) ---" << endl;
        cout << "1. Lookup Order by ID" << endl;
        cout << "2. Lookup Rider by ID" << endl;
        cout << "3. Filter Orders by Customer ID" << endl;
        cout << "4. Show All Available Riders" << endl;
        cout << "5. Show Loaded Restaurants (Threshold)" << endl;
        cout << "0. Go Back" << endl;
        cout << "Choose option: ";
        
        if (!(cin >> subChoice)) {
            clearInput();
            continue;
        }
        clearInput();

        if (subChoice == 0) break;

        if (subChoice == 1) {
        string orderId;
        cout << "Enter Order ID: ";
        cin >> orderId;
        clearInput();

        Order* o = searchEngine.findOrder(orderId);
        if (o != nullptr) {
            cout << "Order Found: " << endl;
            cout << "  - ID: " << o->getOrderID() << endl;
            cout << "  - Customer ID: " << o->getCustomerID() << endl;
            cout << "  - Restaurant ID: " << o->getRestaurantID() << endl;
            cout << "  - Priority: " << o->getPriorityScore() << endl;
            cout << "  - Status: " << o->getStatus() << endl;
            cout << "  - Prep/Deadline: " << o->getPreparationTime() << "/" << o->getDeliveryDeadline() << " min" << endl;
        } else {
            cout << "Order ID not found." << endl;
        }

    } else if (subChoice == 2) {
        string riderId;
        cout << "Enter Rider ID: ";
        cin >> riderId;
        clearInput();

        Rider* r = searchEngine.findRider(riderId);
        if (r != nullptr) {
            cout << "Rider Found: " << r->getName() << endl;
            cout << "  - Location: " << r->getCurrentLocationNodeID() << endl;
            cout << "  - Load/Capacity: " << r->getCurrentLoad() << "/" << r->getMaxCapacity() << endl;
            cout << "  - Rating: " << r->getRating() << endl;
            cout << "  - Completed Deliveries: " << r->getDeliveriesCompleted() << endl;
        } else {
            cout << "Rider ID not found." << endl;
        }

    } else if (subChoice == 3) {
        string custId;
        cout << "Enter Customer ID: ";
        cin >> custId;
        clearInput();

        CustomList<Order*> results = searchEngine.getOrdersByCustomer(custId);
        if (results.isEmpty()) {
            cout << "No orders found for customer " << custId << "." << endl;
        } else {
            cout << "Found " << results.size() << " orders:" << endl;
            CustomList<Order*>::Node* node = results.getHead();
            while (node != nullptr) {
                cout << "  - " << node->data->getOrderID() << ": Status=" << node->data->getStatus() 
                          << ", Priority=" << node->data->getPriorityScore() << endl;
                node = node->next;
            }
        }

    } else if (subChoice == 4) {
        CustomList<Rider*> available = searchEngine.getAvailableRiders();
        if (available.isEmpty()) {
            cout << "No riders are currently available." << endl;
        } else {
            cout << "Available Riders:" << endl;
            CustomList<Rider*>::Node* node = available.getHead();
            while (node != nullptr) {
                cout << "  - " << node->data->getName() << " (" << node->data->getRiderID() 
                          << "): Location=" << node->data->getCurrentLocationNodeID() 
                          << ", Active Load=" << node->data->getCurrentLoad() << "/" << node->data->getMaxCapacity() << endl;
                node = node->next;
            }
        }

    } else if (subChoice == 5) {
        double threshold = 0.7;
        cout << "Enter workload threshold ratio (e.g. 0.7 = 70%): ";
        cin >> threshold;
        clearInput();

        CustomList<Restaurant*> loaded = searchEngine.getOverloadedRestaurants(threshold);
        if (loaded.isEmpty()) {
            cout << "No restaurants exceed a workload ratio of " << threshold << "." << endl;
        } else {
            cout << "Restaurants exceeding " << threshold << " workload ratio:" << endl;
            CustomList<Restaurant*>::Node* node = loaded.getHead();
            while (node != nullptr) {
                cout << "  - " << node->data->getName() << " (" << node->data->getRestaurantID() 
                          << "): Ratio=" << node->data->getWorkloadRatio() 
                          << " (Load: " << node->data->getCurrentLoad() << "/" << node->data->getMaxCapacity() << ")" << endl;
                node = node->next;
            }
        }
    }
    
    cout << "\nPress Enter to continue...";
    cin.get();
}
}

void InteractiveCLI::runOrderHistoryMenu() {
    int subChoice = 0;
    while (true) {
        system("cls");
        cout << "\n--- Order History and Tracking ---" << endl;
        cout << "1. Replay Order Status Timeline" << endl;
        cout << "2. Undo Last Status Transition (Revert State)" << endl;
        cout << "0. Go Back" << endl;
        cout << "Choose option: ";
        
        if (!(cin >> subChoice)) {
            clearInput();
            continue;
        }
        clearInput();

        if (subChoice == 0) break;

        string orderId;
    cout << "Enter Order ID: ";
    cin >> orderId;
    clearInput();

    Order* o = searchEngine.findOrder(orderId);
    OrderStatusHistory* hist = getOrCreateHistory(orderId);

    if (o == nullptr) {
        cout << "Order ID not found." << endl;
        cout << "\nPress Enter to continue...";
        cin.get();
        continue;
    }

    if (subChoice == 1) {
        hist->replayTimeline();
    } else if (subChoice == 2) {
        // If the order was popped from the scheduler heap because it was Assigned or Delivered,
        // and we revert it, we should re-insert it back into the heap!
        string initialStatus = o->getStatus();
        if (hist->undoLastTransition(o, currentSimulationTime)) {
            string finalStatus = o->getStatus();
            
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
                cout << "[OrderStatusHistory] Order re-inserted into active scheduling queue." << endl;
            }
        }
    }
        
        cout << "\nPress Enter to continue...";
        cin.get();
    }
}

void InteractiveCLI::runPerformanceAnalysisMenu() {
    int subChoice = 0;
    while (true) {
        system("cls");
        cout << "\n--- Performance Analysis ---" << endl;
        cout << "1. Run Scalability Benchmarks (Small, Medium, Large sizes)" << endl;
        cout << "2. View Theoretical Complexity Report & Justifications" << endl;
        cout << "0. Go Back" << endl;
        cout << "Choose option: ";
        
        if (!(cin >> subChoice)) {
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
    
    cout << "\nPress Enter to continue...";
    cin.get();
}
}

void InteractiveCLI::runScalabilitySimulationMenu() {
    cout << "\n--- Run Batch Scalability Simulation ---" << endl;
    cout << "Running bulk auto-dispatch simulation..." << endl;

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
                cout << "[Simulation] All riders are currently busy. Pausing batch simulation." << endl;
                break;
            }
        }
    }

    cout << "[Simulation] Batch run complete. Processed " << processedCount << " orders to final delivery." << endl;
}

void InteractiveCLI::printSystemReports() {
    cout << "\n================= FOODEXPRESS SYSTEM REPORT =================" << endl;
    cout << "Locations in City Map : " << mapGraph.getVertexCount() << endl;
    cout << "Registered Customers  : " << customers.size() << endl;
    cout << "Partner Restaurants   : " << restaurants.size() << endl;
    cout << "Active Delivery Riders: " << riders.size() << endl;
    
    int activeOrders = schedulingEngine.getActiveOrdersQueue().size();
    int totalOrders = schedulingEngine.getAllOrders().size();
    
    cout << "Active Scheduled Orders: " << activeOrders << endl;
    cout << "Total Sim Orders Seeded: " << totalOrders << endl;
    
    cout << "\nBlocked Roads List: " << endl;
    bool foundBlocked = false;
    for (int i = 0; i < mapGraph.getVertexCount(); i++) {
        string src = mapGraph.getVertexIdAt(i);
        Edge* edge = mapGraph.getEdgesFrom(src);
        while (edge != nullptr) {
            if (edge->isBlocked && src < edge->destinationNodeID) {
                cout << "  - " << src << " <-> " << edge->destinationNodeID << endl;
                foundBlocked = true;
            }
            edge = edge->next;
        }
    }
    if (!foundBlocked) {
        cout << "  - None" << endl;
    }
    cout << "=============================================================\n" << endl;
}

void InteractiveCLI::runEntityManagementMenu() {
    int subChoice = 0;
    while (true) {
        system("cls");
        cout << "\n--- Entity Management ---" << endl;
        cout << "1. Register New Restaurant" << endl;
        cout << "2. Register New Rider" << endl;
        cout << "0. Go Back" << endl;
        cout << "Choose option: ";
        
        if (!(cin >> subChoice)) {
            clearInput();
            continue;
        }
        clearInput();

        if (subChoice == 0) break;

        if (subChoice == 1) {
        string id, name, location;
        int capacity;
        cout << "Enter Restaurant ID (e.g. R99): ";
        cin >> id;
        cout << "Enter Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter Location Node ID (e.g. Downtown): ";
        cin >> location;
        cout << "Enter Max Kitchen Capacity: ";
        cin >> capacity;
        clearInput();

        if (searchEngine.findRestaurant(id) != nullptr) {
            cout << "Restaurant ID already exists!" << endl;
            return;
        }
        if (!mapGraph.locationExists(location)) {
            cout << "Warning: Location node does not exist in graph. Will fallback to default during dispatch." << endl;
        }

        Restaurant* r = new Restaurant(id, name, location, capacity, 0);
        restaurants.pushBack(r);
        searchEngine.indexRestaurant(r);
        cout << "Restaurant " << name << " registered successfully." << endl;
        addNotification("New Restaurant added: " + name);

    } else if (subChoice == 2) {
        string id, name, location;
        int capacity;
        cout << "Enter Rider ID (e.g. D99): ";
        cin >> id;
        cout << "Enter Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter Starting Location Node ID: ";
        cin >> location;
        cout << "Enter Carrying Capacity: ";
        cin >> capacity;
        clearInput();

        if (searchEngine.findRider(id) != nullptr) {
            cout << "Rider ID already exists!" << endl;
            return;
        }

        Rider* r = new Rider(id, name, location, capacity, 0, true, 5.0, 0);
        riders.pushBack(r);
        searchEngine.indexRider(r);
        cout << "Rider " << name << " registered successfully." << endl;
        addNotification("New Rider added: " + name);
    }
    
    cout << "\nPress Enter to continue...";
    cin.get();
}
}
