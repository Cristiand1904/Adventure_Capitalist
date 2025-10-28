#include "../include/Upgrade.h"

Upgrade::Upgrade(const std::string& n, double m, double c)
    : name(n), multiplier(m), cost(c), purchased(false) {}

bool Upgrade::canPurchase(double money) const {
    return !purchased && money >= cost;
}

void Upgrade::purchase() {
    purchased = true;
}

bool Upgrade::isPurchased() const {
    return purchased;
}

double Upgrade::getMultiplier() const {
    return multiplier;
}

double Upgrade::getCost() const {
    return cost;
}

const std::string& Upgrade::getName() const {
    return name;
}
