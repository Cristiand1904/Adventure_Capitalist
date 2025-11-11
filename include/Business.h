#pragma once
#include <string>
#include <iostream>
#include <memory>
#include "Upgrade.h"

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
    std::unique_ptr<Upgrade> managerUpgrade;

public:
    Business(const std::string& name, double profit, double upgrade, double cost, double manager);

    Business(const Business& other);
    Business& operator=(const Business& other);
    Business(Business&&) noexcept = default;
    Business& operator=(Business&&) noexcept = default;

    ~Business() = default;

    void levelUp();
    void increaseUpgradeCost(double factor);
    void unlock(double& money);
    void unlockManager(double& money);

    void modifyProfit(double factor);
    void modifyUpgradeCost(double factor);

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
