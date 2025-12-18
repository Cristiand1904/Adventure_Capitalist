#include "../include/Business.h"
#include <iostream>
#include <iomanip>
#include <utility>

int Business::totalBusinessesActive = 0;

Business::Business(const std::string& name, double profit, double upgrade, double cost)
    : name(name),
      profitPerCycle(profit),
      upgradeCost(upgrade),
      purchaseCost(cost),
      level(0),
      owned(false),
      manager(nullptr) {
    totalBusinessesActive++;
}

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
    totalBusinessesActive++;
}

Business::~Business() {
    totalBusinessesActive--;
}

void swap(Business& first, Business& second) noexcept {
    using std::swap;
    swap(first.name, second.name);
    swap(first.profitPerCycle, second.profitPerCycle);
    swap(first.upgradeCost, second.upgradeCost);
    swap(first.purchaseCost, second.purchaseCost);
    swap(first.level, second.level);
    swap(first.owned, second.owned);
    swap(first.manager, second.manager);
    swap(first.upgrades, second.upgrades);
}

Business& Business::operator=(Business other) {
    swap(*this, other);
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

int Business::getTotalBusinessesActive() {
    return totalBusinessesActive;
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