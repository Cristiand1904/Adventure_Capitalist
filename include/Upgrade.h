#pragma once
#include <string>

class Upgrade {
private:
    double cost;
    double multiplier;
    bool purchased;

public:
    Upgrade(double c = 0, double m = 1.0)
        : cost(c), multiplier(m), purchased(false) {}

    bool canPurchase(double money) const;
    void purchase();
    bool isPurchased() const;
    double getMultiplier() const;
    double getCost() const;
};
