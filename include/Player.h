#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Business.h"
#include "Wallet.h"

class Player {
private:
    std::string name;
    Wallet wallet;
    std::vector<std::unique_ptr<Business>> businesses;

public:
    Player(const std::string& name, double money);

    Player(const Player& other);
    Player& operator=(Player other);
    Player(Player&&) noexcept = default;
    Player& operator=(Player&&) noexcept = default;

    void addBusiness(std::unique_ptr<Business> business);

    const std::string& getName() const;
    double getMoney() const;
    const std::vector<std::unique_ptr<Business>>& getBusinesses() const;

    // Actualizeaza starea afacerilor si colecteaza profitul
    void update(double deltaTime);

    // Porneste productia manuala pentru o afacere
    void startBusinessProduction(int index);

    void displayBusinesses() const;

    void purchaseBusiness(int index);
    void upgradeBusiness(int index);
    void hireManager(int index);

    friend void swap(Player& first, Player& second) noexcept;
};