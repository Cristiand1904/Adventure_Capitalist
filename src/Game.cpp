#include "../include/Game.h"
#include <iomanip>

Game::Game(const Player& p) : player(p) {}

void Game::showStatus() const {
    std::cout << "===== STATUS =====\n";
    std::cout << player;
    std::cout << "==================\n";
}

void Game::runCycle() {
    player.earnCycle(1);
}

bool Game::upgradeBusiness(int index, int times, double pricePerLevel) {
    auto& businesses = player.getBusinesses();
    if (index < 0 || index >= (int)businesses.size())
        return false;

    double cost = pricePerLevel * times;
    if (player.pay(cost)) {
        for (int i = 0; i < times; ++i)
            businesses[index].levelUp();
        return true;
    }
    return false;
}

void Game::interactiveUpgrade() {
    while (true) {
        std::cout << "\nAlege business-ul:\n";
        const auto& businesses = player.getBusinesses();
        int i = 1;
        for (const auto& b : businesses) {
            std::cout << i++ << ". " << b.getName()
                      << " (lvl " << b.getLevel()
                      << ") - profit: " << (int)b.getProfitPerCycle()
                      << "$/ciclu - cost upgrade: " << b.getUpgradeCost() << "$\n";
        }
        std::cout << i << ". Renunta\n";
        std::cout << "Optiunea: ";

        int opt;
        std::cin >> opt;
        if (opt == i) break;

        if (opt > 0 && opt < i) {
            auto& b = player.getBusinesses()[opt - 1];
            double cost = b.getUpgradeCost();
            if (player.pay(cost)) {
                b.levelUp();
                b.increaseUpgradeCost(1.5);
                std::cout << "Upgrade efectuat la " << b.getName() << "!\n";
            } else {
                std::cout << "Fonduri insuficiente pentru upgrade la " << b.getName() << ".\n";
            }
        }
    }
}
