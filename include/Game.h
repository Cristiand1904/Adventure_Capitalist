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

    void run(); // Pastrat pentru consistenta, dar nefolosit in GUI

    // Getter necesar pentru ca Application sa acceseze starea jocului
    Player& getPlayer() { return player; }
    const Player& getPlayer() const { return player; }

private:
    void displayGameInfo() const;
    void mainLoop();
    void handlePlayerInput();
    void analyzeBusinesses() const;
};