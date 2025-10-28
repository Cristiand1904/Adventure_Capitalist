#include "../include/Business.h"

Business::Business(const std::string& name, double profit, double upgrade, double cost, double manager)
    : name(name), profitPerCycle(profit), upgradeCost(upgrade), purchaseCost(cost),
      managerCost(manager), level(0), owned(false), hasManager(false) {}

void Business::levelUp() {
    level++;
    profitPerCycle *= 1.2;
}

void Business::increaseUpgradeCost(double factor) {
    upgradeCost *= factor;
}

void Business::modifyProfit(double factor) {
    profitPerCycle *= factor;
}

void Business::modifyUpgradeCost(double factor) {
    upgradeCost *= factor;
}

void Business::unlock(double& money) {
    if (!owned && money >= purchaseCost) {
        money -= purchaseCost;
        owned = true;
        std::cout << "Ai cumparat " << name << "!\n";
    } else {
        std::cout << "Nu ai destui bani pentru a cumpara " << name << "!\n";
    }
}

void Business::unlockManager(double& money) {
    if (!hasManager && money >= managerCost) {
        money -= managerCost;
        hasManager = true;
        std::cout << "Managerul pentru " << name << " a fost cumparat!\n";
    } else if (hasManager) {
        std::cout << "Managerul pentru " << name << " este deja activ!\n";
    } else {
        std::cout << "Nu ai destui bani pentru managerul " << name << "!\n";
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
    os << b.name
       << " | Status: " << (b.owned ? "OWNED" : "LOCKED")
       << " | lvl=" << b.level
       << " | profit=" << static_cast<int>(b.profitPerCycle)
       << " | cost upgrade=" << static_cast<int>(b.upgradeCost) << "$";
    return os;
}
