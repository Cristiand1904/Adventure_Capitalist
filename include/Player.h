#pragma once
#include <string>
#include <vector>
#include <iostream>
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
    void addMoney(double amount);

    void earnProfit();
    void unlockBusiness(int index);

    std::vector<Business>& accessBusinesses();
    std::vector<Business>& getBusinesses();
    const std::vector<Business>& getBusinesses() const;

    double& accessMoney();

    friend std::ostream& operator<<(std::ostream& os, const Player& p);
};