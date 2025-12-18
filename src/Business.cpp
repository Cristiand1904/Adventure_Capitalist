#include "../include/Business.h"
#include <iostream>
#include <iomanip>
#include <utility>

Business::Business(const std::string& name, double profit, double upgrade, double cost)
    : name(name),
      profitPerCycle(profit),
      upgradeCost(upgrade),
      purchaseCost(cost),
      level(0),
      owned(false),
      manager(nullptr) {}

Business::Business(const Business& other)
    : name(other.name),
      profitPerCycle(other.profitPerCycle),
      upgradeCost(other.upgradeCost),
      purchaseCost(other.purchaseCost),
      level(other.level),
      owned(other.owned),
      upgrades(other.upgrades) {
    if (other.manager) {
        manager = std::make_unique<Manager>(*other.manager);
    } else {
        manager = nullptr;
    }
}

Business& Business::operator=(const Business& other) {
    if (this == &other) {
        return *this;
    }
    name = other.name;
    profitPerCycle = other.profitPerCycle;
    upgradeCost = other.upgradeCost;
    purchaseCost = other.purchaseCost;
    level = other.level;
    owned = other.owned;
    upgrades = other.upgrades;
    if (other.manager) {
        manager = std::make_unique<Manager>(*other.manager);
    } else {
        manager = nullptr;
    }
    return *this;
}

void Business::levelUp() {
    level++;
    profitPerCycle += 10 * level;
}

void Business::unlock() {
    owned = true;
    level = 1;
    std::cout << "Ai cumparat " << name << "!\n";
}

void Business::hireManager() {
    manager = std::make_unique<Manager>(name + " Manager", getManagerCost());
    std::cout << "Manager angajat pentru " << name << "!\n";
}

bool Business::isOwned() const { return owned; }
bool Business::hasManagerHired() const { return manager != nullptr; }
const std::string& Business::getName() const { return name; }
int Business::getLevel() const { return level; }
double Business::getProfitPerCycle() const { return profitPerCycle; }
double Business::getUpgradeCost() const { return upgradeCost; }
double Business::getPurchaseCost() const { return purchaseCost; }
double Business::getManagerCost() const {
    return purchaseCost * 2;
}

void Business::display(std::ostream& os) const {
    print(os);
}

void Business::print(std::ostream& os) const {
    os << std::fixed << std::setprecision(0);
    os << getName() << " (Lvl " << getLevel() << ")";
    os << " | Profit/Ciclu: " << getProfitPerCycle() << "$";

    if (isOwned()) {
        os << " | Status: DETINUT";
        os << " | Cost Upgrade: " << getUpgradeCost() << "$";
        os << " | Manager: " << (hasManagerHired() ? "ANGAJAT" : "NEANGAJAT");
    } else {
        os << " | Status: BLOCAT";
        os << " | Cost Cumparare: " << getPurchaseCost() << "$";
    }
}

std::ostream& operator<<(std::ostream& os, const Business& b) {
    b.display(os);
    return os;
}