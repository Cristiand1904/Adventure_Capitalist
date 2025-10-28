#ifndef BUSINESS_H
#define BUSINESS_H

#include <string>
#include <iostream>

class Business {
    std::string name;
    double profitPerCycle;
    double upgradeCost;
    int level;

public:
    Business(const std::string& name, double profit, double upgradeCost);

    void levelUp();
    void increaseUpgradeCost(double factor);
    double getProfitPerCycle() const;
    int getLevel() const;
    const std::string& getName() const;
    double getUpgradeCost() const;

    friend std::ostream& operator<<(std::ostream& os, const Business& b);
};

#endif
