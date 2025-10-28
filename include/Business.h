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
    int level;
    bool owned;
    bool hasManager;

public:
    Business(const std::string& name, double profit, double upgrade, double cost, double manager);

    void levelUp();
    void increaseUpgradeCost(double factor);
    void unlock(double& money);
    void unlockManager(double& money);

    bool isOwned() const;
    bool hasManagerUnlocked() const;
    const std::string& getName() const;
    int getLevel() const;
    double getProfitPerCycle() const;
    double getUpgradeCost() const;
    double getPurchaseCost() const;
    double getManagerCost() const;

    friend std::ostream& operator<<(std::ostream& os, const Business& b);
};
