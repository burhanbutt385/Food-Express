#include "Hasnain/InteractiveCLI.h"
#include <iostream>

int main() {
  std::cout << "[FoodExpress] Initializing modules..." << std::endl;
  InteractiveCLI cli;
  cli.start();
  std::cout << "[FoodExpress] Shutdown clean. Goodbye!" << std::endl;
  return 0;
}
