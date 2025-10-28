#include "../include/Business.h"

Business::Business(const std::string& n, double profit, double upgrade, double cost, double manager)
    : name(n), profitPerCycle(profit), upgradeCost(upgrade),
      purchaseCost(cost), managerCost(manager),
      level(0), owned(false), hasManager(false) {}

void Business::levelUp() {
    if (owned) {
        level++;
        profitPerCycle *= 1.5;
    }
}

void Business::increaseUpgradeCost(double factor) {
    upgradeCost *= factor;
}

void Business::unlock(double& money) {
    if (!owned && money >= purchaseCost) {
        money -= purchaseCost;
        owned = true;
    }
}

void Business::unlockManager(double& money) {
    if (owned && !hasManager && money >= managerCost) {
        money -= managerCost;
        hasManager = true;
    }
}

bool Business::isOwned() const { return owned; }
bool Business::hasManagerUnlocked() const { return hasManager; }
const std::string& Business::getName() const { return name; }
int Business::getLevel() const { return level; }
double Business::getProfitPerCycle() const { return profitPerCycle; }
double Business::getUpgradeCost() const { return upgradeCost; }
double Business::getPurchaseCost() const { return purchaseCost; }
double Business::getManagerCost() const { return managerCost; }

std::ostream& operator<<(std::ostream& os, const Business& b) {
    os << b.name << " | " << (b.owned ? "OWNED" : "LOCKED")
       << " | lvl=" << b.level
       << " | profit=" << static_cast<int>(b.profitPerCycle)
       << " | manager=" << (b.hasManager ? "OWNED" : "LOCKED");
    return os;
}
