#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Business.h"
#include "Wallet.h"
#include "Achievement.h"

class Player {
private:
    std::string name;
    Wallet wallet;
    std::vector<std::unique_ptr<Business>> businesses;
    std::vector<Achievement> achievements;

    void initAchievements();

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
    const std::vector<Achievement>& getAchievements() const;

    // Returneaza o lista de mesaje pentru achievement-urile deblocate in acest frame
    std::vector<std::string> update(double deltaTime);
    std::vector<std::string> checkAchievements();

    void startBusinessProduction(int index);

    void displayBusinesses() const;

    void purchaseBusiness(int index);
    void upgradeBusiness(int index);
    void hireManager(int index);
    void upgradeManager(int index);

    friend void swap(Player& first, Player& second) noexcept;
};