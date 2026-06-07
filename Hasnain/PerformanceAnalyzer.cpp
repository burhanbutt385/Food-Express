#include "PerformanceAnalyzer.h"
#include "../Burhan/Order.h"
#include "../Burhan/OrderPriorityQueue.h"
#include "../Burhan/CustomHashMap.h"
#include "../Ahmer/MapGraph.h"
#include "../Ahmer/RouteOptimizer.h"

#include <chrono>
#include <iostream>
#include <iomanip>

PerformanceAnalyzer::PerformanceAnalyzer() {}
PerformanceAnalyzer::~PerformanceAnalyzer() {}

void PerformanceAnalyzer::runScalabilitySimulation() {
    std::cout << "\n========================================================" << std::endl;
    std::cout << "        RUNNING SCALABILITY PERFORMANCE BENCHMARK        " << std::endl;
    std::cout << "========================================================\n" << std::endl;

    int sizes[] = {50, 500, 5000};

    // Setup map graph for route benchmarks
    MapGraph benchmarkGraph(15);
    for (int i = 0; i < 15; i++) {
        benchmarkGraph.addLocation("L" + std::to_string(i), "Location " + std::to_string(i));
    }
    // Connect them in a ring and cross edges
    for (int i = 0; i < 15; i++) {
        benchmarkGraph.addRoad("L" + std::to_string(i), "L" + std::to_string((i + 1) % 15), 5.0, 1.2);
        benchmarkGraph.addRoad("L" + std::to_string(i), "L" + std::to_string((i + 5) % 15), 12.0, 1.0);
    }
    RouteOptimizer routeOptimizer;

    std::cout << std::left << std::setw(15) << "Dataset Size" 
              << std::setw(15) << "Heap Insert" 
              << std::setw(15) << "Hash Put" 
              << std::setw(15) << "Hash Get (xN)" 
              << std::setw(15) << "Dijkstra (x50)" 
              << "Heap PopMax" << std::endl;
    std::cout << "----------------------------------------------------------------------------------------" << std::endl;

    for (int size : sizes) {
        // Pre-create N orders to avoid timing the new allocation
        Order** testOrders = new Order*[size];
        for (int i = 0; i < size; i++) {
            testOrders[i] = new Order("O" + std::to_string(i), "C" + std::to_string(i % 10), "R" + std::to_string(i % 5), 15, 30, 0);
            testOrders[i]->setPriorityScore(static_cast<double>(i % 100));
        }

        // 1. Heap Insertion Benchmark
        OrderPriorityQueue pq(size);
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < size; i++) {
            pq.insert(testOrders[i]);
        }
        auto end = std::chrono::high_resolution_clock::now();
        double heapInsertTime = std::chrono::duration<double, std::milli>(end - start).count();

        // 2. Hash Map Put Benchmark
        CustomHashMap<Order*> hashMap(size * 2);
        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < size; i++) {
            hashMap.put(testOrders[i]->getOrderID(), testOrders[i]);
        }
        end = std::chrono::high_resolution_clock::now();
        double hashPutTime = std::chrono::duration<double, std::milli>(end - start).count();

        // 3. Hash Map Get Benchmark (Lookup all elements)
        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < size; i++) {
            Order* out = nullptr;
            hashMap.get("O" + std::to_string(i), out);
        }
        end = std::chrono::high_resolution_clock::now();
        double hashGetTime = std::chrono::duration<double, std::milli>(end - start).count();

        // 4. Route Calculation Benchmark (Run Dijkstra 50 times)
        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 50; i++) {
            routeOptimizer.calculateShortestDistance("L" + std::to_string(i % 15), "L" + std::to_string((i + 7) % 15), benchmarkGraph, true);
        }
        end = std::chrono::high_resolution_clock::now();
        double routeTime = std::chrono::duration<double, std::milli>(end - start).count();

        // 5. Heap Extraction Benchmark (Extract all elements)
        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < size; i++) {
            pq.popMax();
        }
        end = std::chrono::high_resolution_clock::now();
        double heapPopTime = std::chrono::duration<double, std::milli>(end - start).count();

        // Output results
        std::cout << std::left << std::setw(15) << size 
                  << std::setw(15) << (std::to_string(heapInsertTime).substr(0, 6) + " ms")
                  << std::setw(15) << (std::to_string(hashPutTime).substr(0, 6) + " ms")
                  << std::setw(15) << (std::to_string(hashGetTime).substr(0, 6) + " ms")
                  << std::setw(15) << (std::to_string(routeTime).substr(0, 6) + " ms")
                  << (std::to_string(heapPopTime).substr(0, 6) + " ms") << std::endl;

        // Cleanup test orders
        for (int i = 0; i < size; i++) {
            delete testOrders[i];
        }
        delete[] testOrders;
    }
    std::cout << "========================================================\n" << std::endl;
}

void PerformanceAnalyzer::printTheoreticalComplexityReport() const {
    std::cout << "\n========================================================" << std::endl;
    std::cout << "      THEORETICAL COMPLEXITY ANALYSIS & JUSTIFICATIONS   " << std::endl;
    std::cout << "========================================================" << std::endl;
    std::cout << "1. DYNAMIC ORDER SCHEDULING (Max-Heap Priority Queue):" << std::endl;
    std::cout << "   - Insertion (insert): O(log N). Highly efficient for continuously arriving orders." << std::endl;
    std::cout << "   - Retrieval of Highest Priority (popMax): O(log N). O(1) read, O(log N) restore heap." << std::endl;
    std::cout << "   - Priority Update / Cancellation: O(N) lookup + O(log N) heapify." << std::endl;
    std::cout << "   Justification: Binary heap maintains sorted order dynamically with minimal overhead." << std::endl;
    std::cout << std::endl;
    std::cout << "2. ROUTE OPTIMIZATION (Dijkstra's Algorithm):" << std::endl;
    std::cout << "   - Shortest Path: O(V^2 + E) where V is locations, E is roads." << std::endl;
    std::cout << "   Justification: Custom Dijkstra provides precise paths avoiding blocked roads." << std::endl;
    std::cout << std::endl;
    std::cout << "3. SEARCH AND RETRIEVAL (Custom Chained Hash Map):" << std::endl;
    std::cout << "   - Lookup / Insertion (get/put): O(1) average. Independent of total items." << std::endl;
    std::cout << "   - Category Filter searches: O(N) since we traverse the table." << std::endl;
    std::cout << "   Justification: Hash map enables instant status verification and ID queries." << std::endl;
    std::cout << std::endl;
    std::cout << "4. MEMORY PERSISTENCE:" << std::endl;
    std::cout << "   - File Loading/Saving: O(N). Runs strictly at initialization and exit." << std::endl;
    std::cout << "========================================================\n" << std::endl;
}
