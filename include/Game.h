#pragma once
#include <iostream>
#include <vector>
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

    void run();

private:
    void displayGameInfo() const;
    void mainLoop();
    void handlePlayerInput();
    void analyzeBusinesses() const;
};