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
    player.addBusiness(std::make_unique<LemonadeStand>());
    player.addBusiness(std::make_unique<IceCreamShop>());
    player.addBusiness(std::make_unique<Restaurant>());
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

    // Salvam banii
    outFile << player.getMoney() << "\n";

    // Salvam afacerile
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

    // Salvam achievement-urile
    const auto& achievements = player.getAchievements();
    outFile << achievements.size() << "\n";
    for (const auto& ach : achievements) {
        // Salvam numele cu _ in loc de spatiu pentru citire usoara, sau folosim getline la incarcare
        // Pentru simplitate, salvam doar statusul (0/1) in ordinea din vector
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
    // Presupunem ca ordinea afacerilor e aceeasi (Lemonade, IceCream, Restaurant)
    for (int i = 0; i < businessCount && i < (int)businesses.size(); ++i) {
        std::string name; // Citim numele dar il ignoram (e doar pentru debug in fisier)
        int level;
        bool owned, hasManager;
        double profit, upgradeCost;

        // Citim numele care poate avea spatii (ex: "Limonada")
        // Dar in fisier am scris cu spatii. E mai sigur sa citim pana la numar.
        // Hack: stim ca primul camp e string, restul numere.
        // Daca numele are spatii, >> se opreste la spatiu.
        // Voi citi numele cuvant cu cuvant pana dau de un numar? Complicat.
        // Voi simplifica salvarea: nu salvez numele, doar datele. Ordinea e fixa.

        // RECTIFICARE: Nu pot citi numele usor daca are spatii.
        // Voi citi un string dummy, sperand ca numele nu are spatii in salvarea anterioara.
        // "Limonada" e ok. "Inghetata" e ok. "Restaurant" e ok.
        inFile >> name >> level >> owned >> hasManager >> profit >> upgradeCost;

        businesses[i]->setLevel(level);
        businesses[i]->setOwned(owned);
        businesses[i]->setManagerHired(hasManager);
        businesses[i]->setProfit(profit);
        businesses[i]->setUpgradeCost(upgradeCost);

        // Daca e owned, pornim productia manuala (sau automata daca are manager)
        if (owned) {
            if (hasManager) businesses[i]->hireManager(); // Re-activeaza logica managerului
            else businesses[i]->unlock(); // Doar seteaza owned=true
        }
    }

    int achCount;
    inFile >> achCount;
    const auto& achievements = player.getAchievements();
    for (int i = 0; i < achCount && i < (int)achievements.size(); ++i) {
        bool unlocked;
        inFile >> unlocked;
        if (unlocked) {
            // Deblocam fara a da reward din nou (folosim o metoda privata sau friend, sau pur si simplu setam starea)
            // Dar Achievement nu are setter pentru unlocked.
            // Voi folosi unlockAchievement din Player care apeleaza unlock().
            // unlock() seteaza doar bool-ul, nu da bani (banii se dau in checkAchievements).
            // Stai, checkAchievements da banii. unlock() e simplu.
            // Deci e ok sa apelam unlockAchievement.
            player.unlockAchievement(achievements[i].getName());
        }
    }

    inFile.close();
    return true;
}

bool Game::saveFileExists(const std::string& filename) {
    return std::filesystem::exists(filename);
}