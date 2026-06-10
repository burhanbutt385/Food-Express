#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
using namespace std;

class Customer {
private:
    string customerID;
    string name;
    string locationNodeID;
    bool isVIP;

public:
    Customer();
    Customer(const string& id, const string& customerName, const string& locationId, bool vip = false);

    string getCustomerID() const;
    string getName() const;
    string getLocationNodeID() const;
    bool getIsVIP() const;

    void setLocationNodeID(const string& locationId);
    void setIsVIP(bool vip);
};

#endif // CUSTOMER_H
