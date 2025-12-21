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

void Game::update(double deltaTime) {
    player.update(deltaTime);
}