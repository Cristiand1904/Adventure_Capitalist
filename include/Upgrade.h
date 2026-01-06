#pragma once
#include <string>
#include <iostream>

class Upgrade {
private:
    double cost;
    double multiplier;
    bool purchased;

public:
    explicit Upgrade(double c = 0, double m = 1.0);

    bool isPurchased() const;
    double getMultiplier() const;
    double getCost() const;

    friend std::ostream& operator<<(std::ostream& os, const Upgrade& u);
};