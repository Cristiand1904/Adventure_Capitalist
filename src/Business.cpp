#include "../include/Business.h"

Business::Business(const std::string& name, double profit, double upgrade, double cost, double manager)
    : name(name),
      profitPerCycle(profit),
      upgradeCost(upgrade),
      purchaseCost(cost),
      managerCost(manager),
      level(0),
      owned(false),
      hasManager(false),
      managerUpgrade(nullptr) {}

Business::Business(const Business& other)
    : name(other.name),
      profitPerCycle(other.profitPerCycle),
      upgradeCost(other.upgradeCost),
      purchaseCost(other.purchaseCost),
      managerCost(other.managerCost),
      level(other.level),
      owned(other.owned),
      hasManager(other.hasManager) {
    if (other.managerUpgrade)
        managerUpgrade = std::make_unique<Upgrade>(*other.managerUpgrade);
}

Business& Business::operator=(const Business& other) {
    if (this != &other) {
        name = other.name;
        profitPerCycle = other.profitPerCycle;
        upgradeCost = other.upgradeCost;
        purchaseCost = other.purchaseCost;
        managerCost = other.managerCost;
        level = other.level;
        owned = other.owned;
        hasManager = other.hasManager;
        managerUpgrade = other.managerUpgrade
            ? std::make_unique<Upgrade>(*other.managerUpgrade)
            : nullptr;
    }
    return *this;
}

void Business::levelUp() {
    level++;
    profitPerCycle += 10 * level;
    if (managerUpgrade && managerUpgrade->isPurchased())
        profitPerCycle *= managerUpgrade->getMultiplier();
}

void Business::increaseUpgradeCost(double factor) {
    upgradeCost *= factor;
}

void Business::unlock(double& money) {
    if (!owned && money >= purchaseCost) {
        money -= purchaseCost;
        owned = true;
        std::cout << "Ai cumparat " << name << "!\n";
    } else if (owned) {
        std::cout << "Business-ul " << name << " este deja detinut.\n";
    } else {
        std::cout << "Nu ai destui bani pentru " << name << ".\n";
    }
}

void Business::unlockManager(double& money) {
    if (!hasManager && money >= managerCost) {
        money -= managerCost;
        hasManager = true;
        std::cout << "Manager cumparat pentru " << name << "!\n";

        if (!managerUpgrade)
            managerUpgrade = std::make_unique<Upgrade>(managerCost * 0.5, 1.03);
        managerUpgrade->purchase();
    } else if (hasManager) {
        std::cout << "Managerul pentru " << name << " este deja activ.\n";
    } else {
        std::cout << "Nu ai destui bani pentru managerul " << name << ".\n";
    }
}

void Business::modifyProfit(double factor) {
    profitPerCycle *= factor;
}

void Business::modifyUpgradeCost(double factor) {
    upgradeCost *= factor;
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
    os << b.name << " | Status: " << (b.owned ? "OWNED" : "LOCKED")
       << " | lvl=" << b.level
       << " | profit=" << static_cast<int>(b.profitPerCycle);
    return os;
}
