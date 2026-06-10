#ifndef PERFORMANCE_ANALYZER_H
#define PERFORMANCE_ANALYZER_H
using namespace std;

class PerformanceAnalyzer {
public:
    PerformanceAnalyzer();
    ~PerformanceAnalyzer();

    // Runs the scalability benchmark and outputs a report
    void runScalabilitySimulation();

    // Prints a discussion of theoretical complexities
    void printTheoreticalComplexityReport() const;
};

#endif // PERFORMANCE_ANALYZER_H
