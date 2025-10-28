#pragma once
#include <string>
#include <iostream>

class Business {
private:
    std::string name;
    double profitPerCycle;
    double upgradeCost;
    double purchaseCost;
    double managerCost;
    double managerUpgradeCost;
    int level;
    int managerLevel;
    bool owned;
    bool hasManager;

public:
    Business(const std::string& name, double profit, double upgrade, double cost, double manager);

    void levelUp();
    void increaseUpgradeCost(double factor);
    void unlock(double& money);
    void unlockManager(double& money);

    // manager upgrade system
    void upgradeManager(double& money);
    bool canUpgradeManager() const;

    // getters
    bool isOwned() const;
    bool hasManagerUnlocked() const;
    const std::string& getName() const;
    int getLevel() const;
    int getManagerLevel() const;
    double getProfitPerCycle() const;
    double getUpgradeCost() const;
    double getPurchaseCost() const;
    double getManagerCost() const;
    double getManagerUpgradeCost() const;

    friend std::ostream& operator<<(std::ostream& os, const Business& b);
};
