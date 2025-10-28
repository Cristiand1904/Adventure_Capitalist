#include "../include/Game.h"
#include <iostream>

Game::Game(const Player& p) : player(p) {}
Game::~Game() = default;

bool Game::upgradeBusiness(int index, int times, double pricePerLevel) {
    auto& list = player.accessBusinesses();
    if (index < 0 || index >= (int)list.size()) {
        std::cout << "Index invalid.\n";
        return false;
    }

    double totalCost = times * pricePerLevel;
    if (!player.pay(totalCost)) {
        std::cout << "Fonduri insuficiente pentru " << times << " upgrade-uri.\n";
        return false;
    }

    for (int i = 0; i < times; ++i)
        list[index].levelUp();

    std::cout << "Ai facut " << times << " upgrade-uri la " << list[index].getName()
              << " pentru " << totalCost << "$.\n";
    return true;
}

double Game::runCycles(int cycles) {
    return player.earnCycle(cycles);
}

void Game::showStatus() const {
    std::cout << player;
}

Player& Game::accessPlayer() { return player; }
const Player& Game::getPlayer() const { return player; }

std::ostream& operator<<(std::ostream& os, const Game& g) {
    os << "[Game] Player money=" << g.getPlayer().getMoney();
    return os;
}
