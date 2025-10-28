#include "../include/Business.h"

void Business::levelUp() {
    level++;
    profitPerCycle *= 1.5;
    upgradeCost *= 1.4;
}

void Business::unlock(double& money) {
    if (!owned && money >= purchaseCost) {
        money -= purchaseCost;
        owned = true;
        std::cout << "Ai cumparat business-ul: " << name << "!\n";
    } else if (owned) {
        std::cout << "Deja detii acest business.\n";
    } else {
        std::cout << "Nu ai destui bani pentru a cumpara " << name << ".\n";
    }
}

void Business::unlockManager(double money) {
    if (!owned) {
        std::cout << "Trebuie sa cumperi mai intai business-ul.\n";
        return;
    }

    if (managerUnlocked) {
        std::cout << "Deja ai manager pentru " << name << ".\n";
        return;
    }

    if (money >= managerCost) {
        managerUnlocked = true;
        std::cout << "Manager cumparat pentru " << name << "!\n";
    } else {
        std::cout << "Nu ai destui bani pentru managerul la " << name << ".\n";
    }
}

double Business::getProfitPerCycle() const { return profitPerCycle; }
double Business::getUpgradeCost() const { return upgradeCost; }
double Business::getPurchaseCost() const { return purchaseCost; }
double Business::getManagerCost() const { return managerCost; }
const std::string& Business::getName() const { return name; }
int Business::getLevel() const { return level; }
bool Business::isOwned() const { return owned; }
bool Business::hasManager() const { return managerUnlocked; }

std::ostream& operator<<(std::ostream& os, const Business& b) {
    os << "[Business] " << b.name
       << " | Status: " << (b.owned ? "OWNED" : "LOCKED")
       << " | lvl=" << b.level
       << " | profit=" << static_cast<int>(b.profitPerCycle)
       << " | Manager: " << (b.managerUnlocked ? "Unlock" : "Lock");
    return os;
}