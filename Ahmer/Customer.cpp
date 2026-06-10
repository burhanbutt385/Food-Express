#include "Customer.h"
using namespace std;

Customer::Customer()
    : customerID(""), name(""), locationNodeID(""), isVIP(false) {}

Customer::Customer(const string& id, const string& customerName, const string& locationId, bool vip)
    : customerID(id), name(customerName), locationNodeID(locationId), isVIP(vip) {}

string Customer::getCustomerID() const { return customerID; }
string Customer::getName() const { return name; }
string Customer::getLocationNodeID() const { return locationNodeID; }
bool Customer::getIsVIP() const { return isVIP; }

void Customer::setLocationNodeID(const string& locationId) { locationNodeID = locationId; }
void Customer::setIsVIP(bool vip) { isVIP = vip; }
