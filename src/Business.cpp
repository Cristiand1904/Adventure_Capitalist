#include "../include/Business.h"

Business::Business(const std::string& n, double profit, double upgrade)
    : name(n), profitPerCycle(profit), upgradeCost(upgrade), level(1) {}

void Business::levelUp() {
    level++;
    profitPerCycle = static_cast<int>(profitPerCycle * 1.25);
}

void Business::increaseUpgradeCost(double factor) {
    upgradeCost = static_cast<int>(upgradeCost * factor);
}

double Business::getProfitPerCycle() const {
    return profitPerCycle;
}

int Business::getLevel() const {
    return level;
}

const std::string& Business::getName() const {
    return name;
}

double Business::getUpgradeCost() const {
    return upgradeCost;
}

std::ostream& operator<<(std::ostream& os, const Business& b) {
    os << "[Business] " << b.name
       << " | lvl=" << b.level
       << " | profit/ciclu=" << static_cast<int>(b.profitPerCycle);
    return os;
}
