#pragma once
#include <iostream>
#include <string>

class Business {
private:
    std::string name;
    double profitPerCycle;
    double upgradeCost;
    double purchaseCost;
    double managerCost;
    int level;
    bool owned;
    bool managerUnlocked;

public:
    Business(const std::string& name, double profit, double upgrade, double cost, double manager)
        : name(name), profitPerCycle(profit), upgradeCost(upgrade),
          purchaseCost(cost), managerCost(manager),
          level(0), owned(false), managerUnlocked(false) {}

    void levelUp();
    void unlock(double& money);
    void unlockManager(double money);
    double getProfitPerCycle() const;
    double getUpgradeCost() const;
    double getPurchaseCost() const;
    double getManagerCost() const;
    const std::string& getName() const;
    int getLevel() const;
    bool isOwned() const;
    bool hasManager() const;

    friend std::ostream& operator<<(std::ostream& os, const Business& b);
};
