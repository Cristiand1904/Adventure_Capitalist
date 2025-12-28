#include "../include/Business.h"
#include <iostream>
#include <iomanip>
#include <utility>
#include <cmath>

Business::Business(const std::string& name, double profit, double upgrade, double cost, double time, double mngCost)
    : name(name),
      profitPerCycle(profit),
      upgradeCost(upgrade),
      purchaseCost(cost),
      managerBaseCost(mngCost),
      level(0),
      owned(false),
      manager(nullptr),
      productionTime(time),
      currentTimer(0.0),
      isProducing(false) {

    if (managerBaseCost <= 0) {
        if (purchaseCost > 0) {
            managerBaseCost = purchaseCost * 10;
        } else {
            managerBaseCost = 100.0;
        }
    }

    if (upgradeCost <= 0) {
        upgradeCost = 10.0;
    }
}

Business::Business(const Business& other)
    : name(other.name),
      profitPerCycle(other.profitPerCycle),
      upgradeCost(other.upgradeCost),
      purchaseCost(other.purchaseCost),
      managerBaseCost(other.managerBaseCost),
      level(other.level),
      owned(other.owned),
      upgrades(other.upgrades),
      productionTime(other.productionTime),
      currentTimer(other.currentTimer),
      isProducing(other.isProducing) {
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
    managerBaseCost = other.managerBaseCost;
    level = other.level;
    owned = other.owned;
    upgrades = other.upgrades;
    productionTime = other.productionTime;
    currentTimer = other.currentTimer;
    isProducing = other.isProducing;
    if (other.manager) {
        manager = std::make_unique<Manager>(*other.manager);
    } else {
        manager = nullptr;
    }
    return *this;
}

double Business::update(double deltaTime) {
    if (!owned) return 0.0;

    if (hasManagerHired() && !isProducing) {
        isProducing = true;
    }

    if (isProducing) {
        currentTimer += deltaTime;
        if (currentTimer >= productionTime) {
            currentTimer = 0.0;
            if (!hasManagerHired()) {
                isProducing = false;
            }
            return std::floor(profitPerCycle);
        }
    }
    return 0.0;
}

void Business::startProduction() {
    if (owned && !isProducing) {
        isProducing = true;
    }
}

void Business::levelUp() {
    level++;

    double profitFactor = 1.15;
    if (level > 50) {
        profitFactor = 1.05;
    }
    profitPerCycle *= profitFactor;

    upgradeCost *= 1.15;

    if (level % 25 == 0) {
        productionTime /= 2.0;
    }
}

void Business::unlock() {
    owned = true;
    level = 1;
    isProducing = false;
    std::cout << "Ai cumparat " << name << "!\n";
}

void Business::hireManager() {
    manager = std::make_unique<Manager>(name + " Manager", getManagerCost());
    isProducing = true;
    std::cout << "Manager angajat pentru " << name << "!\n";
}

void Business::upgradeManager() {
    if (manager) {
        manager->upgrade();
    }
}

// Setteri
void Business::setLevel(int lvl) { level = lvl; }
void Business::setOwned(bool o) { owned = o; }
void Business::setManagerHired(bool h) {
    if (h && !manager) {
        manager = std::make_unique<Manager>(name + " Manager", getManagerCost());
        isProducing = true;
    } else if (!h) {
        manager = nullptr;
    }
}
void Business::setProfit(double p) { profitPerCycle = p; }
void Business::setUpgradeCost(double c) { upgradeCost = c; }

bool Business::isOwned() const { return owned; }
bool Business::hasManagerHired() const { return manager != nullptr; }
const std::string& Business::getName() const { return name; }
int Business::getLevel() const { return level; }
double Business::getProfitPerCycle() const { return profitPerCycle; }

double Business::getUpgradeCost() const {
    double cost = upgradeCost;
    if (manager) {
        cost *= manager->getDiscountFactor();
    }
    return std::floor(cost);
}

double Business::getPurchaseCost() const { return std::floor(purchaseCost); }

double Business::getManagerCost() const {
    return std::floor(managerBaseCost);
}

double Business::getManagerUpgradeCost() const {
    if (manager) {
        return std::floor(manager->getCost());
    }
    return 0.0;
}

double Business::getProgress() const {
    if (!isProducing) return 0.0;
    if (currentTimer >= productionTime - 0.1) {
        return 1.0;
    }
    return std::min(1.0, currentTimer / productionTime);
}

double Business::getProductionTime() const { return productionTime; }
bool Business::isActive() const { return isProducing; }

void Business::display(std::ostream& os) const {
    print(os);
}

void Business::print(std::ostream& os) const {
    os << std::fixed << std::setprecision(0);
    os << getName() << " (Lvl " << getLevel() << ")";
    os << " | Profit: " << getProfitPerCycle() << "$";
    os << " | Timp: " << std::setprecision(1) << productionTime << "s";
}

std::ostream& operator<<(std::ostream& os, const Business& b) {
    b.display(os);
    return os;
}