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
    int level;
    bool owned;
    std::unique_ptr<Manager> manager;
    std::vector<Upgrade> upgrades;

    // Noi atribute pentru mecanica de timp
    double productionTime; // Timpul total necesar pentru un ciclu (secunde)
    double currentTimer;   // Timpul scurs din ciclul curent
    bool isProducing;      // Daca afacerea produce in acest moment

    virtual void print(std::ostream& os) const;

public:
    Business(const std::string& name, double profit, double upgrade, double cost, double time);

    Business(const Business& other);
    Business& operator=(const Business& other);
    Business(Business&&) noexcept = default;
    Business& operator=(Business&&) noexcept = default;

    virtual ~Business() = default;

    virtual double calculateRevenue(double bonusMultiplier) const = 0;
    virtual std::unique_ptr<Business> clone() const = 0;

    // Metoda principala de update (apelata in fiecare frame)
    // Returneaza profitul generat in acest frame (0 daca nu s-a terminat ciclul)
    double update(double deltaTime);

    // Porneste productia manual
    void startProduction();

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

    // Getteri pentru UI
    double getProgress() const; // Returneaza 0.0 - 1.0
    double getProductionTime() const;
    bool isActive() const; // Daca produce

    friend void swap(Business& first, Business& second) noexcept;
    friend std::ostream& operator<<(std::ostream& os, const Business& b);
};