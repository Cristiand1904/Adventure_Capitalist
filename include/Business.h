#pragma once
#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include "Manager.h"
#include "Upgrade.h"

class Business {
private:
    static int totalBusinessesActive;

protected:
    std::string name;
    double profitPerCycle;
    double upgradeCost;
    double purchaseCost;
    int level;
    bool owned;
    std::unique_ptr<Manager> manager;
    std::vector<Upgrade> upgrades;

    virtual void print(std::ostream& os) const;

public:
    Business(const std::string& name, double profit, double upgrade, double cost);

    Business(const Business& other);
    Business& operator=(const Business& other);
    Business(Business&&) noexcept = default;
    Business& operator=(Business&&) noexcept = default;

    virtual ~Business();

    virtual double calculateRevenue(double bonusMultiplier) const = 0;
    virtual std::unique_ptr<Business> clone() const = 0;

    void display(std::ostream& os) const;

    void levelUp();
    void unlock();
    void hireManager();

    bool isOwned() const;
    bool hasManagerHired() const;
    const std::string& getName() const;
    int getLevel() const;
    double getProfitPerCycle() const;
    double getUpgradeCost() const;
    double getPurchaseCost() const;
    double getManagerCost() const;

    static int getTotalBusinessesActive();

    friend void swap(Business& first, Business& second) noexcept;
    friend std::ostream& operator<<(std::ostream& os, const Business& b);
};