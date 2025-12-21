#include "../include/Game.h"
#include "../include/Exceptions.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <algorithm>

Game::Game(const std::string& playerName, double initialMoney)
    : player(playerName, initialMoney) {
    setupBusinesses();
}

void Game::setupBusinesses() {
    player.addBusiness(std::make_unique<LemonadeStand>());
    player.addBusiness(std::make_unique<IceCreamShop>());
    player.addBusiness(std::make_unique<Restaurant>());
}

void Game::run() {
    std::cout << "=== Adventure Capitalist CLI ===\n";
    std::cout << "Bun venit, " << player.getName() << "!\n";
    mainLoop();
}

void Game::displayGameInfo() const {
    const auto& businesses = player.getBusinesses();

    long ownedCount = std::count_if(businesses.begin(), businesses.end(),
        [](const std::unique_ptr<Business>& b) {
            return b->isOwned();
        }
    );

    std::cout << "\n--------------------------------------------------\n";
    std::cout << "STATUS JUCATOR\n";
    std::cout << "Nume: " << player.getName() << "\n";
    std::cout << "Bani: " << std::fixed << std::setprecision(0) << player.getMoney() << "$\n";
    std::cout << "Afaceri detinute: " << ownedCount << "/" << businesses.size() << "\n";
    std::cout << "--------------------------------------------------\n";

    std::cout << "LISTA AFACERI:\n";
    std::cout << std::left
              << std::setw(4) << "ID"
              << std::setw(20) << "Nume"
              << std::setw(8) << "Level"
              << std::setw(15) << "Profit"
              << std::setw(12) << "Stare"
              << std::setw(15) << "Cost"
              << "\n";

    for (size_t i = 0; i < businesses.size(); ++i) {
        const auto& b = businesses[i];
        std::cout << std::left << std::setw(4) << (i + 1);
        std::cout << std::setw(20) << b->getName();

        if (b->isOwned()) {
            std::cout << std::setw(8) << b->getLevel();
            std::cout << std::setw(15) << (std::to_string((int)b->getProfitPerCycle()) + "$");

            if (b->hasManagerHired()) {
                std::cout << std::setw(12) << "Manager";
            } else {
                std::cout << std::setw(12) << "Activ";
            }

            std::cout << std::setw(15) << (std::to_string((int)b->getUpgradeCost()) + "$ (Upg)");
        } else {
            std::cout << std::setw(8) << "-";
            std::cout << std::setw(15) << "-";
            std::cout << std::setw(12) << "Blocat";
            std::cout << std::setw(15) << (std::to_string((int)b->getPurchaseCost()) + "$ (Buy)");
        }
        std::cout << "\n";
    }
    std::cout << "--------------------------------------------------\n";
}

void Game::analyzeBusinesses() const {
    std::cout << "\n[ANALIZA]\n";
    bool found = false;
    for (const auto& business_ptr : player.getBusinesses()) {
        if (!business_ptr->isOwned()) continue;

        if (const auto* lemonade = dynamic_cast<LemonadeStand*>(business_ptr.get())) {
            std::cout << " - " << lemonade->getName() << ": Vanzari bune vara.\n";
            found = true;
        } else if (const auto* iceCream = dynamic_cast<IceCreamShop*>(business_ptr.get())) {
            std::cout << " - " << iceCream->getName() << ": Popular printre copii.\n";
            found = true;
        } else if (const auto* restaurant = dynamic_cast<Restaurant*>(business_ptr.get())) {
            std::cout << " - " << restaurant->getName() << ": Locatie premium.\n";
            found = true;
        }
    }
    if (!found) std::cout << "Nu detii afaceri pentru analiza.\n";
}

void Game::mainLoop() {
    while (true) {
        try {
            displayGameInfo();
            handlePlayerInput();
        } catch (const GameException& e) {
            std::cerr << "Eroare: " << e.what() << "\n";
            // Pauza simpla
            std::cout << "Apasa Enter...\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
        } catch (const std::exception& e) {
            std::cerr << "Eroare critica: " << e.what() << "\n";
            break;
        }
    }
}

void Game::handlePlayerInput() {
    std::cout << "\nCOMENZI:\n";
    std::cout << " 1 - Cumpara\n";
    std::cout << " 2 - Upgrade\n";
    std::cout << " 3 - Manager\n";
    std::cout << " 4 - Analiza\n";
    std::cout << " 5 - Incaseaza Profit\n";
    std::cout << " 0 - Iesire\n";
    std::cout << "> ";

    int choice;
    if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    int index;

    switch (choice) {
        case 1:
            std::cout << "ID afacere: ";
            std::cin >> index;
            player.purchaseBusiness(index - 1);
            break;
        case 2:
            std::cout << "ID afacere: ";
            std::cin >> index;
            player.upgradeBusiness(index - 1);
            break;
        case 3:
            std::cout << "ID afacere: ";
            std::cin >> index;
            player.hireManager(index - 1);
            break;
        case 4:
            analyzeBusinesses();
            std::cout << "Apasa Enter...\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            break;
        case 5:
            player.earnProfit();
            std::cout << "Profit incasat.\n";
            break;
        case 0:
            std::cout << "La revedere!\n";
            exit(0);
        default:
            std::cout << "Comanda necunoscuta.\n";
            break;
    }
}