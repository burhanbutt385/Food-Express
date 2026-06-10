#include "Hasnain/InteractiveCLI.h"
#include <iostream>
using namespace std;
int main() {
  cout << "[FoodExpress] Initializing modules..." << endl;
  InteractiveCLI cli;
  cli.start();
  cout << "[FoodExpress] Shutdown clean. Goodbye!" << endl;
  return 0;
}
