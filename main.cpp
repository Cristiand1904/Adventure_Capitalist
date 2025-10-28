#include <iostream>
#include <fstream>
#include "include/Game.h"
#include "include/Business.h"
#include "include/Player.h"

int main() {
    std::ifstream fin("../tastatura.txt");
    if (!fin.is_open()) {
        std::cerr << "Eroare: nu s-a putut deschide fisierul tastatura.txt\n";
        return 1;
    }

    std::string playerName;
    double startMoney;
    fin >> playerName >> startMoney;

    Player p(playerName, startMoney);
    int nrBusiness;
    fin >> nrBusiness;

    for (int i = 0; i < nrBusiness; i++) {
        std::string nume;
        double profit, costUpgrade;
        fin >> nume >> profit >> costUpgrade;
        p.addBusiness(Business(nume, profit, costUpgrade));
    }

    Game game(p);

    while (true) {
        game.showStatus();
        std::cout << "1. Ruleaza un ciclu\n";
        std::cout << "2. Fa upgrade la un business\n";
        std::cout << "3. Iesi din joc\n";
        std::cout << "Optiunea: ";

        int opt;
        std::cin >> opt;

        if (opt == 1) {
            game.runCycle();
        } else if (opt == 2) {
            game.interactiveUpgrade();
        } else if (opt == 3) {
            break;
        } else {
            std::cout << "Optiune invalida.\n";
        }
    }

    std::cout << "\nJoc terminat. Ai ramas cu " << static_cast<int>(p.getBusinesses().size()) << " afaceri active.\n";
    return 0;
}
