#include "../include/Business.h"

Business::Business(const std::string& name, double profit, double upgrade, double cost, double manager)
    : name(name),
      profitPerCycle(profit),
      upgradeCost(upgrade),
      purchaseCost(cost),
      managerCost(manager),
      managerUpgradeCost(manager * 0.5),
      level(0),
      managerLevel(0),
      owned(false),
      hasManager(false) {}

void Business::levelUp() {
    if (owned) {
        level++;
        profitPerCycle *= 1.2; // +20% profit per level
    }
}

void Business::increaseUpgradeCost(double factor) {
    upgradeCost *= factor;
}

void Business::unlock(double& money) {
    if (owned) {
        std::cout << "Business-ul " << name << " este deja cumparat.\n";
        return;
    }

    if (money >= purchaseCost) {
        money -= purchaseCost;
        owned = true;
        std::cout << "Ai cumparat business-ul: " << name << "!\n";
    } else {
        std::cout << "Nu ai destui bani pentru " << name << ".\n";
    }
}

void Business::unlockManager(double& money) {
    if (!owned) {
        std::cout << "Nu poti cumpara manager pentru un business blocat.\n";
        return;
    }
    if (hasManager) {
        std::cout << "Managerul pentru " << name << " este deja activ.\n";
        return;
    }

    if (money >= managerCost) {
        money -= managerCost;
        hasManager = true;
        std::cout << "Managerul pentru " << name << " a fost angajat!\n";
    } else {
        std::cout << "Nu ai destui bani pentru a angaja managerul la " << name << ".\n";
    }
}

void Business::upgradeManager(double& money) {
    if (!hasManager) {
        std::cout << "Nu poti face upgrade la un manager inexistent.\n";
        return;
    }

    if (money < managerUpgradeCost) {
        std::cout << "Nu ai destui bani pentru upgrade-ul managerului.\n";
        return;
    }

    money -= managerUpgradeCost;
    managerLevel++;
    managerUpgradeCost *= 2.0;

    profitPerCycle *= 1.03;  // +3% profit per nivel de manager
    upgradeCost *= 0.9;      // -10% cost upgrade

    std::cout << "Managerul pentru " << name << " a fost imbunatatit la nivelul "
              << managerLevel << "! Profit crescut si upgrade-uri mai ieftine.\n";
}

bool Business::canUpgradeManager() const {
    return hasManager;
}

bool Business::isOwned() const {
    return owned;
}

bool Business::hasManagerUnlocked() const {
    return hasManager;
}

const std::string& Business::getName() const {
    return name;
}

int Business::getLevel() const {
    return level;
}

int Business::getManagerLevel() const {
    return managerLevel;
}

double Business::getProfitPerCycle() const {
    return profitPerCycle;
}

double Business::getUpgradeCost() const {
    return upgradeCost;
}

double Business::getPurchaseCost() const {
    return purchaseCost;
}

double Business::getManagerCost() const {
    return managerCost;
}

double Business::getManagerUpgradeCost() const {
    return managerUpgradeCost;
}

std::ostream& operator<<(std::ostream& os, const Business& b) {
    os << b.getName()
       << " | Status: " << (b.isOwned() ? "OWNED" : "LOCKED")
       << " | lvl=" << b.getLevel()
       << " | profit=" << static_cast<int>(b.getProfitPerCycle())
       << " | Manager: " << (b.hasManagerUnlocked() ? "ACTIVE" : "NONE")
       << " (lvl " << b.getManagerLevel() << ")";
    return os;
}
