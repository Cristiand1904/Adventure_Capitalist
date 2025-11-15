#include "../include/Upgrade.h"
#include <iostream>
#include <iomanip>

bool Upgrade::canPurchase(double money) const {
    return money >= cost;
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

std::ostream& operator<<(std::ostream& os, const Upgrade& u) {
    os << std::fixed << std::setprecision(0);
    os << "Cost: " << u.getCost() << "$";
    os << " | Multiplicator: x" << u.getMultiplier();
    os << " | Status: " << (u.isPurchased() ? "CUMPARAT" : "DISPONIBIL");
    return os;
}