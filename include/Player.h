#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Business.h"

class Player {
private:
    std::string name;
    double money;
    std::vector<Business> businesses;

public:
    Player(const std::string& n, double initialMoney);

    // Getters
    const std::string& getName() const;
    double getMoney() const;
    std::vector<Business>& getBusinesses();
    const std::vector<Business>& getBusinesses() const;

    // Actions
    void addBusiness(const Business& b);
    void unlockBusiness(int index);
    void earnCycle();
    bool pay(double amount);
    void printBusinesses() const;
};
