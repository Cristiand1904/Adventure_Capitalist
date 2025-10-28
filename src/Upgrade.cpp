#include "../include/Upgrade.h"

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
