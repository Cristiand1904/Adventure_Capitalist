#include <iostream>
#include <fstream>
#include <limits>
#include <cmath>
#include "include/Game.h"

void clearInput(std::istream& in) {
    in.clear();
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    std::ifstream fin("../tastatura.txt");
    if (!fin.is_open()) {
        std::cerr << "Eroare: nu s-a putut deschide fisierul tastatura.txt\n";
        return 1;
    }

    std::string playerName;
    double startMoney;
    std::getline(fin, playerName);
    fin >> startMoney;
    clearInput(fin);

    Player p(playerName, startMoney);

    int nrBusiness;
    fin >> nrBusiness;
    clearInput(fin);

    for (int i = 0; i < nrBusiness; ++i) {
        std::string nume;
        double profit, costUpgrade;
        std::getline(fin, nume);
        fin >> profit >> costUpgrade;
        clearInput(fin);
        p.buyBusiness(Business(nume, profit, costUpgrade), 0);
    }

    fin.close();

    Game game(p);

    bool running = true;
    while (running) {
        std::cout << "\n===== STATUS =====\n";
        game.showStatus();
        std::cout << "==================\n";
        std::cout << "1. Ruleaza un ciclu\n";
        std::cout << "2. Fa upgrade la un business\n";
        std::cout << "3. Iesi din joc\n";
        std::cout << "Optiunea: ";

        int opt;
        std::cin >> opt;

        if (std::cin.fail()) {
            clearInput(std::cin);
            continue;
        }

        switch (opt) {
            case 1: {
                double earned = game.runCycles(1);
                std::cout << "Ai castigat " << static_cast<int>(earned) << "$\n";
                break;
            }

            case 2: {
                auto& player = game.accessPlayer();
                auto& businesses = player.accessBusinesses();

                if (businesses.empty()) {
                    std::cout << "Nu ai business-uri\n";
                    break;
                }

                std::cout << "Alege business-ul:\n";
                for (size_t i = 0; i < businesses.size(); ++i) {
                    double dynamicCost = businesses[i].getUpgradeBaseCost() *
                                         (1 + 0.5 * (businesses[i].getLevel() - 1));
                    std::cout << i + 1 << ". " << businesses[i].getName()
                              << " (lvl " << businesses[i].getLevel() << ")"
                              << " - profit: " << static_cast<int>(businesses[i].getProfitPerCycle())
                              << "$/ciclu - cost upgrade: " << static_cast<int>(std::round(dynamicCost)) << "$\n";
                }
                std::cout << businesses.size() + 1 << ". Renunta\n";
                std::cout << "Optiunea: ";

                int choice;
                std::cin >> choice;

                if (std::cin.fail() || choice < 1 || choice > (int)businesses.size() + 1) {
                    clearInput(std::cin);
                    std::cout << "Optiune invalida\n";
                    break;
                }

                if (choice == (int)businesses.size() + 1)
                    break;

                int index = choice - 1;
                int times;
                std::cout << "Cate upgrade-uri vrei? ";
                std::cin >> times;
                if (std::cin.fail() || times <= 0) {
                    clearInput(std::cin);
                    std::cout << "Numar invalid\n";
                    break;
                }

                for (int i = 0; i < times; ++i) {
                    double cost = businesses[index].getUpgradeBaseCost() *
                                  (1 + 0.5 * (businesses[index].getLevel() - 1));
                    if (player.pay(cost)) {
                        businesses[index].levelUp();
                        std::cout << "Upgrade efectuat pentru "
                                  << businesses[index].getName()
                                  << " (lvl " << businesses[index].getLevel()
                                  << ") pentru " << static_cast<int>(std::round(cost)) << "$\n";
                    } else {
                        std::cout << "Nu ai bani pentru urmatorul upgrade.\n";
                        break;
                    }
                }
                std::cout << "Bani ramasi: " << static_cast<int>(player.getMoney()) << "$\n";
                break;
            }

            case 3:
                running = false;
                break;

            default:
                std::cout << "Optiune invalida\n";
        }
    }

    std::cout << "\nJoc terminat. Ai ramas cu " << static_cast<int>(game.getPlayer().getMoney()) << "$\n";
    return 0;
}
