#include "Customer.h"

Customer::Customer()
    : customerID(""), name(""), locationNodeID(""), isVIP(false) {}

Customer::Customer(const std::string& id, const std::string& customerName, const std::string& locationId, bool vip)
    : customerID(id), name(customerName), locationNodeID(locationId), isVIP(vip) {}

std::string Customer::getCustomerID() const { return customerID; }
std::string Customer::getName() const { return name; }
std::string Customer::getLocationNodeID() const { return locationNodeID; }
bool Customer::getIsVIP() const { return isVIP; }

void Customer::setLocationNodeID(const std::string& locationId) { locationNodeID = locationId; }
void Customer::setIsVIP(bool vip) { isVIP = vip; }
