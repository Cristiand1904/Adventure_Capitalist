#include "../include/Business.h"
#include <cmath>

Business::Business(const std::string& name, double profitPerCycle, double upgradeBaseCost)
    : name(name), profitPerCycle(profitPerCycle), level(1), upgradeBaseCost(upgradeBaseCost) {}

Business::Business(const Business& other)
    : name(other.name), profitPerCycle(other.profitPerCycle), level(other.level), upgradeBaseCost(other.upgradeBaseCost) {}

Business& Business::operator=(const Business& other) {
    if (this != &other) {
        name = other.name;
        profitPerCycle = other.profitPerCycle;
        level = other.level;
        upgradeBaseCost = other.upgradeBaseCost;
    }
    return *this;
}

Business::~Business() = default;

void Business::levelUp() {
    level++;
    profitPerCycle = static_cast<int>(profitPerCycle * 1.25);
}

double Business::projectedProfit() const {
    return profitPerCycle;
}

const std::string& Business::getName() const {
    return name;
}

double Business::getProfitPerCycle() const {
    return profitPerCycle;
}

int Business::getLevel() const {
    return level;
}

double Business::getUpgradeBaseCost() const {
    return upgradeBaseCost;
}

std::ostream& operator<<(std::ostream& os, const Business& b) {
    os << "[Business] " << b.name
       << " | lvl=" << b.level
       << " | profit/ciclu=" << static_cast<int>(b.profitPerCycle);
    return os;
}
