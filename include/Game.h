#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Player.h"
#include "Business.h"
#include "LemonadeStand.h"
#include "Restaurant.h"
#include "IceCreamShop.h"

class Game {
private:
    Player player;
    void setupBusinesses();

public:
    Game(const std::string& playerName, double initialMoney);

    std::vector<std::string> update(double deltaTime); // Returneaza notificari

    Player& getPlayer() { return player; }
    const Player& getPlayer() const { return player; }
};