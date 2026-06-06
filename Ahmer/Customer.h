#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>

class Customer {
private:
    std::string customerID;
    std::string name;
    std::string locationNodeID;
    bool isVIP;

public:
    Customer();
    Customer(const std::string& id, const std::string& customerName, const std::string& locationId, bool vip = false);

    std::string getCustomerID() const;
    std::string getName() const;
    std::string getLocationNodeID() const;
    bool getIsVIP() const;

    void setLocationNodeID(const std::string& locationId);
    void setIsVIP(bool vip);
};

#endif // CUSTOMER_H
