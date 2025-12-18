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
    std::cout << "=== Bine ai venit in Adventure Capitalist, "
              << player.getName() << "! ===\n";
    mainLoop();
}

void Game::displayGameInfo() const {
    const auto& businesses = player.getBusinesses();

    long ownedCount = std::count_if(businesses.begin(), businesses.end(),
        [](const std::unique_ptr<Business>& b) {
            return b->isOwned();
        }
    );

    std::cout << "\n======================================\n";
    std::cout << "[Jucator] " << player.getName()
              << " | Bani: " << std::fixed << std::setprecision(0) << player.getMoney() << "$\n";
    std::cout << "Afaceri detinute: " << ownedCount << "/" << businesses.size() << "\n";
    std::cout << "Afaceri active in joc: " << Business::getTotalBusinessesActive() << "\n";
    std::cout << "======================================\n";
    player.displayBusinesses();
    std::cout << "======================================\n";
}

void Game::analyzeBusinesses() const {
    std::cout << "\n--- ANALIZA AFACERI ---\n";
    for (const auto& business_ptr : player.getBusinesses()) {
        if (!business_ptr->isOwned()) continue;

        if (auto* lemonade = dynamic_cast<LemonadeStand*>(business_ptr.get())) {
            std::cout << "Sfat pentru '" << lemonade->getName() << "': Limonada se vinde excelent in zilele toride de vara!\n";
        } else if (auto* iceCream = dynamic_cast<IceCreamShop*>(business_ptr.get())) {
            std::cout << "Sfat pentru '" << iceCream->getName() << "': Ofera noi arome pentru a atrage mai multi clienti!\n";
        } else if (auto* restaurant = dynamic_cast<Restaurant*>(business_ptr.get())) {
            std::cout << "Sfat pentru '" << restaurant->getName() << "': O locatie buna este cheia succesului in acest domeniu!\n";
        }
    }
    std::cout << "--- SFARSIT ANALIZA ---\n";
}

void Game::mainLoop() {
    while (true) {
        try {
            displayGameInfo();
            player.earnProfit();
            handlePlayerInput();
        } catch (const GameException& e) {
            std::cerr << "\n[EROARE] " << e.what() << std::endl;
            std::cout << "Apasati Enter pentru a continua...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
        } catch (const std::exception& e) {
            std::cerr << "\n[EROARE NECUNOSCUTA] " << e.what() << std::endl;
            break;
        }
    }
}

void Game::handlePlayerInput() {
    std::cout << "\nOptiuni:\n";
    std::cout << "  1. Cumpara business\n";
    std::cout << "  2. Upgrade business\n";
    std::cout << "  3. Angajeaza manager\n";
    std::cout << "  4. Analizeaza afaceri\n";
    std::cout << "  0. Iesire\n";
    std::cout << "Optiunea: ";

    int choice;
    std::cin >> choice;
    int index;

    switch (choice) {
        case 1:
            std::cout << "Index business de cumparat: ";
            std::cin >> index;
            player.purchaseBusiness(index - 1);
            break;
        case 2:
            std::cout << "Index business de upgradat: ";
            std::cin >> index;
            player.upgradeBusiness(index - 1);
            break;
        case 3:
            std::cout << "Index business pentru angajat manager: ";
            std::cin >> index;
            player.hireManager(index - 1);
            break;
        case 4:
            analyzeBusinesses();
            break;
        case 0:
            std::cout << "Joc terminat!\n";
            exit(0);
        default:
            std::cout << "Optiune invalida.\n";
            break;
    }
}