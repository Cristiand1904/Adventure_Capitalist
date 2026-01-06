#include "../include/Game.h"
#include "../include/Exceptions.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <fstream>
#include <filesystem>

Game::Game(const std::string& playerName, double initialMoney)
    : player(playerName, initialMoney) {
    setupBusinesses();
}

void Game::setupBusinesses() {
    player.addBusiness(std::make_unique<Business>("Limonada", BusinessType::LEMONADE, 1, 4, 0, 1.0, 100.0));
    player.addBusiness(std::make_unique<Business>("Inghetata", BusinessType::ICE_CREAM, 10, 40, 100, 3.0, 1000.0));
    player.addBusiness(std::make_unique<Business>("Restaurant", BusinessType::RESTAURANT, 100, 400, 1000, 10.0, 10000.0));
}

std::vector<std::string> Game::update(double deltaTime) {
    return player.update(deltaTime);
}

void Game::saveGame(const std::string& filename) const {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Eroare la salvarea jocului!\n";
        return;
    }

    outFile << player.getMoney() << "\n";

    const auto& businesses = player.getBusinesses();
    outFile << businesses.size() << "\n";
    for (const auto& b : businesses) {
        outFile << b->getName() << " "
                << b->getLevel() << " "
                << b->isOwned() << " "
                << b->hasManagerHired() << " "
                << b->getProfitPerCycle() << " "
                << b->getUpgradeCost() << "\n";
    }

    const auto& achievements = player.getAchievements();
    outFile << achievements.size() << "\n";
    for (const auto& ach : achievements) {
        outFile << ach.isUnlocked() << "\n";
    }

    outFile.close();
    std::cout << "Joc salvat cu succes!\n";
}

bool Game::loadGame(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        return false;
    }

    double money;
    inFile >> money;
    player.setMoney(money);

    int businessCount;
    inFile >> businessCount;

    const auto& businesses = player.getBusinesses();
    for (int i = 0; i < businessCount && i < (int)businesses.size(); ++i) {
        std::string name;
        int level;
        bool owned, hasManager;
        double profit, upgradeCost;

        inFile >> name >> level >> owned >> hasManager >> profit >> upgradeCost;

        businesses[i]->setLevel(level);
        businesses[i]->setOwned(owned);
        businesses[i]->setManagerHired(hasManager);
        businesses[i]->setProfit(profit);
        businesses[i]->setUpgradeCost(upgradeCost);

        if (owned) {
            if (hasManager) businesses[i]->hireManager();
            else businesses[i]->unlock();
        }
    }

    int achCount;
    inFile >> achCount;
    const auto& achievements = player.getAchievements();
    for (int i = 0; i < achCount && i < (int)achievements.size(); ++i) {
        bool unlocked;
        inFile >> unlocked;
        if (unlocked) {
            player.unlockAchievement(achievements[i].getName());
        }
    }

    inFile.close();
    return true;
}

bool Game::saveFileExists(const std::string& filename) {
    return std::filesystem::exists(filename);
}