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
            case 1: player.earnCycle(); break;
            case 2: interactiveUpgrade(); break;
            case 3: buyBusiness(); break;
            case 4: buyManager(); break;
            case 0: std::cout << "Joc terminat!\n"; break;
            default: std::cout << "Optiune invalida.\n"; break;
        }
    }
}

void Game::interactiveUpgrade() {
    auto& businesses = player.getBusinesses();
    std::cout << "\nAlege business-ul pentru upgrade:\n";
    for (size_t i = 0; i < businesses.size(); ++i) {
        std::cout << i << ". " << businesses[i].getName()
                  << " (cost upgrade: " << static_cast<int>(businesses[i].getUpgradeCost()) << "$)\n";
    }

    int idx;
    std::cout << "Optiunea: ";
    std::cin >> idx;

    if (idx >= 0 && idx < (int)businesses.size()) {
        Business& b = businesses[idx];
        double cost = b.getUpgradeCost();

        if (!b.isOwned()) {
            std::cout << "Nu ai deblocat acest business!\n";
            return;
        }

        if (player.pay(cost)) {
            b.levelUp();
            std::cout << "Upgrade efectuat pentru " << b.getName() << "!\n";
        } else {
            std::cout << "Nu ai destui bani pentru upgrade.\n";
        }
    } else {
        std::cout << "Optiune invalida.\n";
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
            std::cout << "Nu poti cumpara manager pentru un business neblocat!\n";
            return;
        }
        b.unlockManager(player.getMoney());
    } else {
        std::cout << "Optiune invalida.\n";
    }
}

void Game::start() {
    std::cout << "=== Bine ai venit in Adventure Capitalist, "
              << player.getName() << "! ===\n";
    interactiveMenu();
}