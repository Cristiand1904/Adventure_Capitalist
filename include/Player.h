#pragma once
#include <vector>
#include <string>
#include "Business.h"

class Player {
    std::string name;
    double money;
    std::vector<Business> businesses;

public:
    Player(const std::string& n, double m);

    const std::string& getName() const;
    double getMoney() const;
    void addMoney(double amount);
    void spendMoney(double amount);
    double& accessMoney();

    std::vector<Business>& getBusinesses();
    const std::vector<Business>& getBusinesses() const;

    void earnProfit();
    void unlockBusiness(int index);
};
