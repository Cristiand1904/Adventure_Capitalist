#ifndef BUSINESS_H
#define BUSINESS_H

#include <string>
#include <iostream>

class Business {
private:
    std::string name;
    double profitPerCycle;
    int level;
    double upgradeBaseCost;

public:
    Business(const std::string& name, double profitPerCycle, double upgradeBaseCost);
    Business(const Business& other);
    Business& operator=(const Business& other);
    ~Business();

    void levelUp();
    double projectedProfit() const;
    const std::string& getName() const;
    double getProfitPerCycle() const;
    int getLevel() const;
    double getUpgradeBaseCost() const;

    friend std::ostream& operator<<(std::ostream& os, const Business& b);
};

#endif
