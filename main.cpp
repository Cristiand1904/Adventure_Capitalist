#include "include/Game.h"
#include <fstream>
#include <iostream>
#include <string>

int main() {
    std::ifstream fin("tastatura.txt");
    if (!fin) {
        std::cerr << "Eroare: nu s-a putut deschide fisierul tastatura.txt\n";
        return 1;
    }

    std::string playerName;
    double initialMoney;
    fin >> playerName >> initialMoney;

    Player p(playerName, initialMoney);

    int n;
    fin >> n; // numărul de business-uri

    for (int i = 0; i < n; ++i) {
        std::string nume;
        double profit, upgrade, cost, managerCost;
        fin >> nume >> profit >> upgrade >> cost >> managerCost;
        p.addBusiness(Business(nume, profit, upgrade, cost, managerCost));
    }

    fin.close();

    Game game(p);
    game.start();

    return 0;
}
