#include <iostream>
#include <iomanip>
#include "../include/Game.h"

Game::Game(const Player& p) : player(p) {}

void Game::displayBusinesses() const {
    std::cout << "\n======================================\n";
    std::cout << "[Jucator] " << player.getName()
              << " | Bani: " << static_cast<int>(player.getMoney()) << "$\n";
    std::cout << "======================================\n";

    std::cout << std::left;
    std::cout << std::setw(3) << "#"
              << std::setw(14) << "Business"
              << std::setw(12) << "Status"
              << std::setw(8) << "Lvl"
              << std::setw(12) << "Profit"
              << std::setw(12) << "Manager" << "\n";

    const auto& businesses = player.getBusinesses();
    for (size_t i = 0; i < businesses.size(); ++i) {
        const auto& b = businesses[i];
        std::cout << std::setw(3) << (i + 1)
                  << std::setw(14) << b.getName()
                  << std::setw(12) << (b.isOwned() ? "OWNED" : "LOCKED")
                  << std::setw(8) << b.getLevel()
                  << std::setw(12) << static_cast<int>(b.getProfitPerCycle())
                  << std::setw(12) << (b.hasManagerUnlocked() ? "OWNED" : "LOCKED")
                  << "\n";
    }
    std::cout << "======================================\n";
}

void Game::interactiveMenu() {
    int opt = -1;
    while (opt != 0) {
        displayBusinesses();
        std::cout << "\nOptiuni:\n";
        std::cout << "  1. Ruleaza ciclu (castiga bani)\n";
        std::cout << "  2. Upgrade business\n";
        std::cout << "  3. Cumpara business\n";
        std::cout << "  4. Cumpara manager\n";
        std::cout << "  5. Upgrade manager\n";
        std::cout << "  0. Iesire\n";
        std::cout << "Optiunea: ";
        std::cin >> opt;

        switch (opt) {
            case 1: player.earnProfit(); break;
            case 2: interactiveUpgrade(); break;
            case 3: buyBusiness(); break;
            case 4: buyManager(); break;
            case 5: upgradeManager(); break;
            case 0: std::cout << "Joc terminat!\n"; break;
            default: std::cout << "Optiune invalida.\n"; break;
        }

        // Managerii rulează automat cicluri
        for (auto& b : player.accessBusinesses()) {
            if (b.hasManagerUnlocked() && b.isOwned()) {
                player.addMoney(b.getProfitPerCycle());
            }
        }
    }
}

void Game::interactiveUpgrade() {
    auto& businesses = player.accessBusinesses();

    while (true) {
        std::cout << "\n=== UPGRADE BUSINESS ===\n";
        std::cout << std::left << std::setw(3) << "#"
                  << std::setw(14) << "Business"
                  << std::setw(12) << "Status"
                  << std::setw(8) << "Lvl"
                  << std::setw(12) << "Profit"
                  << std::setw(16) << "Cost Upgrade" << "\n";

        for (size_t i = 0; i < businesses.size(); ++i) {
            const auto& b = businesses[i];
            std::cout << std::setw(3) << (i + 1)
                      << std::setw(14) << b.getName()
                      << std::setw(12) << (b.isOwned() ? "OWNED" : "LOCKED")
                      << std::setw(8) << b.getLevel()
                      << std::setw(12) << static_cast<int>(b.getProfitPerCycle())
                      << std::setw(16) << static_cast<int>(b.getUpgradeCost()) << "$\n";
        }
        std::cout << "0. Inapoi la meniul principal\n";

        int idx;
        std::cout << "\nOptiunea: ";
        std::cin >> idx;

        if (idx == 0) return;
        if (idx < 1 || idx > (int)businesses.size()) {
            std::cout << "Index invalid.\n";
            continue;
        }

        Business& b = businesses[idx - 1];
        if (!b.isOwned()) {
            std::cout << "Nu poti face upgrade la un business blocat.\n";
            continue;
        }

        int numUpgrades;
        std::cout << "Cate upgrade-uri vrei sa faci? ";
        std::cin >> numUpgrades;
        if (numUpgrades <= 0) continue;

        double availableMoney = player.getMoney();
        double currentCost = b.getUpgradeCost();
        int upgradesDone = 0;
        double totalCost = 0.0;

        for (int i = 0; i < numUpgrades; ++i) {
            if (availableMoney >= currentCost) {
                availableMoney -= currentCost;
                totalCost += currentCost;
                b.levelUp();
                if (b.hasManagerUnlocked()) {
                    b.modifyProfit(1.03);      // +3% profit
                    b.modifyUpgradeCost(0.9);  // -10% cost upgrade
                } else {
                    b.increaseUpgradeCost(1.5);
                }
                upgradesDone++;
                currentCost = b.getUpgradeCost();
            } else break;
        }

        if (upgradesDone > 0) {
            player.spendMoney(totalCost);
            std::cout << "Ai facut " << upgradesDone << " upgrade-uri la " << b.getName()
                      << " pentru " << static_cast<int>(totalCost) << "$!\n";
        } else std::cout << "Nu ai avut bani pentru niciun upgrade.\n";

        for (auto& biz : player.accessBusinesses())
            if (biz.hasManagerUnlocked()) player.addMoney(biz.getProfitPerCycle());
    }
}

void Game::buyBusiness() {
    auto& businesses = player.accessBusinesses();

    while (true) {
        std::cout << "\n=== CUMPARA BUSINESS ===\n";
        std::cout << std::left << std::setw(3) << "#"
                  << std::setw(14) << "Business"
                  << std::setw(12) << "Status"
                  << std::setw(16) << "Cost Cumparare" << "\n";

        for (size_t i = 0; i < businesses.size(); ++i) {
            const auto& b = businesses[i];
            std::cout << std::setw(3) << (i + 1)
                      << std::setw(14) << b.getName()
                      << std::setw(12) << (b.isOwned() ? "OWNED" : "LOCKED")
                      << std::setw(16) << static_cast<int>(b.getPurchaseCost()) << "$\n";
        }
        std::cout << "0. Inapoi la meniul principal\n";

        int idx;
        std::cout << "\nOptiunea: ";
        std::cin >> idx;

        if (idx == 0) return;
        if (idx < 1 || idx > (int)businesses.size()) {
            std::cout << "Index invalid.\n";
            continue;
        }

        player.unlockBusiness(idx - 1);

        for (auto& b : player.accessBusinesses())
            if (b.hasManagerUnlocked()) player.addMoney(b.getProfitPerCycle());
    }
}

void Game::buyManager() {
    auto& businesses = player.accessBusinesses();

    while (true) {
        std::cout << "\n=== CUMPARA MANAGER ===\n";
        std::cout << std::left << std::setw(3) << "#"
                  << std::setw(14) << "Business"
                  << std::setw(12) << "Status"
                  << std::setw(16) << "Cost Manager" << "\n";

        for (size_t i = 0; i < businesses.size(); ++i) {
            const auto& b = businesses[i];
            std::cout << std::setw(3) << (i + 1)
                      << std::setw(14) << b.getName()
                      << std::setw(12) << (b.hasManagerUnlocked() ? "OWNED" : "LOCKED")
                      << std::setw(16) << static_cast<int>(b.getManagerCost()) << "$\n";
        }
        std::cout << "0. Inapoi la meniul principal\n";

        int idx;
        std::cout << "\nOptiunea: ";
        std::cin >> idx;

        if (idx == 0) return;
        if (idx < 1 || idx > (int)businesses.size()) {
            std::cout << "Index invalid.\n";
            continue;
        }

        Business& b = businesses[idx - 1];
        if (!b.isOwned()) {
            std::cout << "Nu poti cumpara manager pentru un business blocat!\n";
            continue;
        }

        double& moneyRef = player.accessMoney();
        b.unlockManager(moneyRef);

        for (auto& biz : player.accessBusinesses())
            if (biz.hasManagerUnlocked()) player.addMoney(biz.getProfitPerCycle());
    }
}

void Game::upgradeManager() {
    auto& businesses = player.accessBusinesses();

    while (true) {
        std::cout << "\n=== UPGRADE MANAGER ===\n";
        std::cout << std::left << std::setw(3) << "#"
                  << std::setw(14) << "Business"
                  << std::setw(12) << "Status"
                  << std::setw(8) << "Lvl"
                  << std::setw(12) << "Profit"
                  << std::setw(16) << "Cost Upgrade Mng" << "\n";

        for (size_t i = 0; i < businesses.size(); ++i) {
            const auto& b = businesses[i];
            std::cout << std::setw(3) << (i + 1)
                      << std::setw(14) << b.getName()
                      << std::setw(12) << (b.hasManagerUnlocked() ? "OWNED" : "LOCKED")
                      << std::setw(8) << b.getLevel()
                      << std::setw(12) << static_cast<int>(b.getProfitPerCycle())
                      << std::setw(16) << static_cast<int>(b.getManagerCost() * 0.5) << "$\n";
        }
        std::cout << "0. Inapoi la meniul principal\n";

        int idx;
        std::cout << "\nOptiunea: ";
        std::cin >> idx;

        if (idx == 0) return;
        if (idx < 1 || idx > (int)businesses.size()) {
            std::cout << "Index invalid.\n";
            continue;
        }

        Business& b = businesses[idx - 1];
        if (!b.hasManagerUnlocked()) {
            std::cout << "Nu ai manager la acest business.\n";
            continue;
        }

        double cost = b.getManagerCost() * 0.5;
        if (player.getMoney() < cost) {
            std::cout << "Nu ai destui bani (" << static_cast<int>(cost) << "$ necesari).\n";
            continue;
        }

        player.spendMoney(cost);
        b.modifyProfit(1.03);
        b.modifyUpgradeCost(0.9);
        std::cout << "Managerul de la " << b.getName() << " a fost imbunatatit!\n";

        for (auto& biz : player.accessBusinesses())
            if (biz.hasManagerUnlocked()) player.addMoney(biz.getProfitPerCycle());
    }
}

void Game::start() {
    std::cout << "=== Bine ai venit in Adventure Capitalist, "
              << player.getName() << "! ===\n";
    interactiveMenu();
}
