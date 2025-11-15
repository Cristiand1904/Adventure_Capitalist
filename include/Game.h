#pragma once
#include <iostream>
#include <vector>
#include "Player.h"
#include "Business.h"

class Game {
private:
    Player player;

public:
    explicit Game(const Player& p);

    void displayBusinesses() const;
    void interactiveMenu();
    void interactiveUpgrade();
    void buyBusiness();
    void buyManager();
    void upgradeManager();
    void start();

    friend std::ostream& operator<<(std::ostream& os, const Game& game);
};