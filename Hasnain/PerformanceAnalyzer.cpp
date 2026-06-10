#include "PerformanceAnalyzer.h"
#include "../Burhan/Order.h"
#include "../Burhan/OrderPriorityQueue.h"
#include "../Burhan/CustomHashMap.h"
#include "../Ahmer/MapGraph.h"
#include "../Ahmer/RouteOptimizer.h"

#include <chrono>
#include <iostream>

using namespace std;

PerformanceAnalyzer::PerformanceAnalyzer() {}
PerformanceAnalyzer::~PerformanceAnalyzer() {}

static string padString(string s, int width) {
    if (s.length() >= width) return s;
    return s + string(width - s.length(), ' ');
}

void PerformanceAnalyzer::runScalabilitySimulation() {
    cout << "\n========================================================" << endl;
    cout << "        RUNNING SCALABILITY PERFORMANCE BENCHMARK        " << endl;
    cout << "========================================================\n" << endl;

    int sizes[] = {50, 500, 5000};

    // Setup map graph for route benchmarks
    MapGraph benchmarkGraph(15);
    for (int i = 0; i < 15; i++) {
        benchmarkGraph.addLocation("L" + to_string(i), "Location " + to_string(i));
    }
    // Connect them in a ring and cross edges
    for (int i = 0; i < 15; i++) {
        benchmarkGraph.addRoad("L" + to_string(i), "L" + to_string((i + 1) % 15), 5.0, 1.2);
        benchmarkGraph.addRoad("L" + to_string(i), "L" + to_string((i + 5) % 15), 12.0, 1.0);
    }
    RouteOptimizer routeOptimizer;

    cout << padString("Dataset Size", 15) 
         << padString("Heap Insert", 15) 
         << padString("Hash Put", 15) 
         << padString("Hash Get (xN)", 15) 
         << padString("Dijkstra (x50)", 15) 
         << "Heap PopMax" << endl;
    cout << "----------------------------------------------------------------------------------------" << endl;

    for (int size : sizes) {
        // Pre-create N orders to avoid timing the new allocation
        Order** testOrders = new Order*[size];
        for (int i = 0; i < size; i++) {
            testOrders[i] = new Order("O" + to_string(i), "C" + to_string(i % 10), "R" + to_string(i % 5), 15, 30, 0);
            testOrders[i]->setPriorityScore(static_cast<double>(i % 100));
        }

        // 1. Heap Insertion Benchmark
        OrderPriorityQueue pq(size);
        auto start = chrono::high_resolution_clock::now();
        for (int i = 0; i < size; i++) {
            pq.insert(testOrders[i]);
        }
        auto end = chrono::high_resolution_clock::now();
        double heapInsertTime = chrono::duration<double, milli>(end - start).count();

        // 2. Hash Map Put Benchmark
        CustomHashMap<Order*> hashMap(size * 2);
        start = chrono::high_resolution_clock::now();
        for (int i = 0; i < size; i++) {
            hashMap.put(testOrders[i]->getOrderID(), testOrders[i]);
        }
        end = chrono::high_resolution_clock::now();
        double hashPutTime = chrono::duration<double, milli>(end - start).count();

        // 3. Hash Map Get Benchmark (Lookup all elements)
        start = chrono::high_resolution_clock::now();
        for (int i = 0; i < size; i++) {
            Order* out = nullptr;
            hashMap.get("O" + to_string(i), out);
        }
        end = chrono::high_resolution_clock::now();
        double hashGetTime = chrono::duration<double, milli>(end - start).count();

        // 4. Route Calculation Benchmark (Run Dijkstra 50 times)
        start = chrono::high_resolution_clock::now();
        for (int i = 0; i < 50; i++) {
            routeOptimizer.calculateShortestDistance("L" + to_string(i % 15), "L" + to_string((i + 7) % 15), benchmarkGraph, true);
        }
        end = chrono::high_resolution_clock::now();
        double routeTime = chrono::duration<double, milli>(end - start).count();

        // 5. Heap Extraction Benchmark (Extract all elements)
        start = chrono::high_resolution_clock::now();
        for (int i = 0; i < size; i++) {
            pq.popMax();
        }
        end = chrono::high_resolution_clock::now();
        double heapPopTime = chrono::duration<double, milli>(end - start).count();

        // Output results
        cout << padString(to_string(size), 15) 
             << padString(to_string(heapInsertTime).substr(0, 6) + " ms", 15)
             << padString(to_string(hashPutTime).substr(0, 6) + " ms", 15)
             << padString(to_string(hashGetTime).substr(0, 6) + " ms", 15)
             << padString(to_string(routeTime).substr(0, 6) + " ms", 15)
             << (to_string(heapPopTime).substr(0, 6) + " ms") << endl;

        // Cleanup test orders
        for (int i = 0; i < size; i++) {
            delete testOrders[i];
        }
        delete[] testOrders;
    }
    cout << "========================================================\n" << endl;
}

void PerformanceAnalyzer::printTheoreticalComplexityReport() const {
    cout << "\n========================================================" << endl;
    cout << "      THEORETICAL COMPLEXITY ANALYSIS & JUSTIFICATIONS   " << endl;
    cout << "========================================================" << endl;
    cout << "1. DYNAMIC ORDER SCHEDULING (Max-Heap Priority Queue):" << endl;
    cout << "   - Insertion (insert): O(log N). Highly efficient for continuously arriving orders." << endl;
    cout << "   - Retrieval of Highest Priority (popMax): O(log N). O(1) read, O(log N) restore heap." << endl;
    cout << "   - Priority Update / Cancellation: O(N) lookup + O(log N) heapify." << endl;
    cout << "   Justification: Binary heap maintains sorted order dynamically with minimal overhead." << endl;
    cout << endl;
    cout << "2. ROUTE OPTIMIZATION (Dijkstra's Algorithm):" << endl;
    cout << "   - Shortest Path: O(V^2 + E) where V is locations, E is roads." << endl;
    cout << "   Justification: Custom Dijkstra provides precise paths avoiding blocked roads." << endl;
    cout << endl;
    cout << "3. SEARCH AND RETRIEVAL (Custom Chained Hash Map):" << endl;
    cout << "   - Lookup / Insertion (get/put): O(1) average. Independent of total items." << endl;
    cout << "   - Category Filter searches: O(N) since we traverse the table." << endl;
    cout << "   Justification: Hash map enables instant status verification and ID queries." << endl;
    cout << endl;
    cout << "4. MEMORY PERSISTENCE:" << endl;
    cout << "   - File Loading/Saving: O(N). Runs strictly at initialization and exit." << endl;
    cout << "========================================================\n" << endl;
}
