#include "../include/Game.h"

Game::Game(const Player& p) : player(p) {}

void Game::displayBusinesses() const {
    std::cout << "\n==============================\n";
    std::cout << "[Jucator] " << player.getName()
              << " | Bani: " << static_cast<int>(player.getMoney()) << "$\n";
    std::cout << "==============================\n";
    std::cout << "Lista business-uri:\n";
    for (const auto& b : player.getBusinesses()) {
        std::cout << "  " << b << "\n";
    }
    std::cout << "==============================\n";
}

void Game::interactiveMenu() {
    int opt = -1;
    while (opt != 0) {
        displayBusinesses();
        std::cout << "\nOptiuni:\n";
        std::cout << "  1. Castiga bani (ciclu)\n";
        std::cout << "  2. Upgrade business\n";
        std::cout << "  3. Cumpara business\n";
        std::cout << "  4. Cumpara manager\n";
        std::cout << "  0. Iesire\n";
        std::cout << "Optiunea: ";
        std::cin >> opt;

        switch (opt) {
            case 1: player.earnProfit(); break;
            case 2: interactiveUpgrade(); break;
            case 3: buyBusiness(); break;
            case 4: buyManager(); break;
            case 0: std::cout << "Joc terminat!\n"; break;
            default: std::cout << "Optiune invalida.\n"; break;
        }
    }
}

void Game::interactiveUpgrade() {
    std::vector<Business>& businesses = player.getBusinesses();

    std::cout << "\n=== UPGRADE BUSINESS ===\n";
    for (size_t i = 0; i < businesses.size(); ++i) {
        const auto& b = businesses[i];
        std::cout << i << ". " << b.getName()
                  << " | Status: " << (b.isOwned() ? "OWNED" : "LOCKED")
                  << " | lvl=" << b.getLevel()
                  << " | profit=" << static_cast<int>(b.getProfitPerCycle())
                  << " | cost upgrade: " << static_cast<int>(b.getUpgradeCost()) << "$\n";
    }

    int idx;
    std::cout << "\nAlege business-ul pentru upgrade: ";
    std::cin >> idx;

    if (idx < 0 || idx >= (int)businesses.size()) {
        std::cout << "Index invalid.\n";
        return;
    }

    Business& b = businesses[idx];
    if (!b.isOwned()) {
        std::cout << "Nu poti face upgrade la un business blocat.\n";
        return;
    }

    int numUpgrades;
    std::cout << "Cate upgrade-uri vrei sa faci? ";
    std::cin >> numUpgrades;

    if (numUpgrades <= 0) {
        std::cout << "Numar invalid de upgrade-uri.\n";
        return;
    }

    double availableMoney = player.getMoney();
    double currentCost = b.getUpgradeCost();
    int upgradesDone = 0;
    double totalCost = 0.0;

    for (int i = 0; i < numUpgrades; ++i) {
        if (availableMoney >= currentCost) {
            availableMoney -= currentCost;
            totalCost += currentCost;
            b.levelUp();
            b.increaseUpgradeCost(1.5);
            upgradesDone++;
            currentCost = b.getUpgradeCost();
        } else {
            std::cout << "Nu mai ai bani pentru mai multe upgrade-uri.\n";
            break;
        }
    }

    if (upgradesDone > 0) {
        player.spendMoney(totalCost);
        std::cout << "Ai facut " << upgradesDone << " upgrade-uri la " << b.getName()
                  << " pentru un total de " << static_cast<int>(totalCost) << "$!\n";
    } else {
        std::cout << "Nu ai avut bani pentru niciun upgrade.\n";
    }
}


void Game::buyBusiness() {
    auto& businesses = player.getBusinesses();
    std::cout << "\nAlege business-ul de cumparat:\n";
    for (size_t i = 0; i < businesses.size(); ++i) {
        std::cout << i << ". " << businesses[i].getName()
                  << " (cost: " << static_cast<int>(businesses[i].getPurchaseCost()) << "$)\n";
    }

    int idx;
    std::cout << "Optiunea: ";
    std::cin >> idx;
    player.unlockBusiness(idx);
}

void Game::buyManager() {
    auto& businesses = player.getBusinesses();
    std::cout << "\nAlege business-ul pentru manager:\n";
    for (size_t i = 0; i < businesses.size(); ++i) {
        std::cout << i << ". " << businesses[i].getName()
                  << " (cost manager: " << static_cast<int>(businesses[i].getManagerCost()) << "$)\n";
    }

    int idx;
    std::cout << "Optiunea: ";
    std::cin >> idx;

    if (idx >= 0 && idx < (int)businesses.size()) {
        Business& b = businesses[idx];
        if (!b.isOwned()) {
            std::cout << "Nu poti cumpara manager pentru un business blocat!\n";
            return;
        }
        double& moneyRef = player.accessMoney();
        b.unlockManager(moneyRef);
    } else {
        std::cout << "Optiune invalida.\n";
    }
}

void Game::start() {
    std::cout << "=== Bine ai venit in Adventure Capitalist, "
              << player.getName() << "! ===\n";
    interactiveMenu();
}