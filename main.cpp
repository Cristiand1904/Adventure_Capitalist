#include "include/Game.h"
#include "include/Upgrade.h"
#include "include/Achievement.h"
#include <fstream>
#include <iostream>
#include <vector>

int main() {
    Upgrade tempUpgrade(50.0, 1.1);
    static_cast<void>(tempUpgrade.getCost());
    static_cast<void>(tempUpgrade.canPurchase(0.0));

    Achievement tempAchievement("DummyAchievement");
    static_cast<void>(tempAchievement.isUnlocked());
    static_cast<void>(tempAchievement.getDescription());


    std::ifstream fin("date_intrare.txt");

    if (!fin.is_open()) {
        std::cout << "Eroare: fisierul 'date_intrare.txt' nu a putut fi deschis.\n";
        return 1;
    }

    std::string nume;
    double bani;
    fin >> nume >> bani;
    Player p(nume, bani);

    int n;
    fin >> n;
    for (int i = 0; i < n; ++i) {
        std::string numeBusiness;
        double profit, upgrade, cost, managerCost;
        fin >> numeBusiness >> profit >> upgrade >> cost >> managerCost;
        p.accessBusinesses().emplace_back(numeBusiness, profit, upgrade, cost, managerCost);
    }

    fin.close();

    Game game(p);
    game.start();

    std::cout << "\n\n--- STATUTUL FINAL AL JOCULUI ---\n";
    std::cout << game;

    return 0;
}