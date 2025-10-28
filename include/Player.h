#pragma once
#include <vector>
#include <string>
#include "Business.h"

class Player {
private:
    std::string name;
    double money;
    std::vector<Business> businesses;

public:
    Player(const std::string& name, double money);

    const std::string& getName() const;
    double getMoney() const;
    void spendMoney(double amount);
    void earnProfit();
    void unlockBusiness(int index);

    std::vector<Business>& getBusinesses();
    const std::vector<Business>& getBusinesses() const;
    std::vector<Business>& accessBusinesses(); // pentru citirea din fișier

    double& accessMoney(); // pentru referință directă în Game (buyManager etc.)
};
