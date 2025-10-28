#include "include/Game.h"
#include <fstream>
#include <iostream>

int main() {
    std::ifstream fin("../tastatura.txt"); // sau "tastatura.txt" dacă fișierul e în cmake-build-debug
    if (!fin.is_open()) {
        std::cout << "Eroare: fisierul 'tastatura.txt' nu a putut fi deschis.\n";
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

    return 0;
}
