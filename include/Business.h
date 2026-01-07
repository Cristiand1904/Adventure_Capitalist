#pragma once
#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include "Manager.h"
#include "Upgrade.h"

class Business {
protected:
    std::string name;
    double profitPerCycle;
    double upgradeCost;
    double purchaseCost;
    double managerBaseCost;
    int level;
    bool owned;
    std::unique_ptr<Manager> manager;
    std::vector<Upgrade> upgrades;

    double productionTime;
    double currentTimer;
    bool isProducing;

    virtual void print(std::ostream& os) const;

public:
    Business(std::string name, double profit, double upgrade, double cost, double time, double mngCost = 0);

    Business(const Business& other);
    Business& operator=(const Business& other);
    Business(Business&&) noexcept = default;
    Business& operator=(Business&&) noexcept = default;

    virtual ~Business() = default;

    virtual std::unique_ptr<Business> clone() const = 0;
    virtual double calculateRevenue(double bonusMultiplier) const = 0;

    double update(double deltaTime);
    void startProduction();

    void display(std::ostream& os) const;

    void levelUp();
    void unlock();
    void hireManager();
    void upgradeManager();

    void setLevel(int lvl);
    void setOwned(bool o);
    void setManagerHired(bool h);
    void setProfit(double p);
    void setUpgradeCost(double c);

    bool isOwned() const;
    bool hasManagerHired() const;
    const std::string& getName() const;
    int getLevel() const;
    double getProfitPerCycle() const;
    double getUpgradeCost() const;
    double getPurchaseCost() const;
    double getManagerCost() const;
    double getManagerUpgradeCost() const;

    double getProgress() const;
    double getProductionTime() const;
    bool isActive() const;

    int getManagerLevel() const;

    friend void swap(Business& first, Business& second) noexcept;
    friend std::ostream& operator<<(std::ostream& os, const Business& b);
};