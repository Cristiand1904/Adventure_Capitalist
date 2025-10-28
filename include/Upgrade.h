#ifndef UPGRADE_H
#define UPGRADE_H

#include <string>

class Upgrade {
private:
    std::string name;
    double multiplier;
    double cost;
    bool purchased;

public:
    Upgrade(const std::string& name, double multiplier, double cost);
    bool canPurchase(double money) const;
    void purchase();
    bool isPurchased() const;
    double getMultiplier() const;
    double getCost() const;
    const std::string& getName() const;
};

#endif
