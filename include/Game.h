#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "Player.h"

class Game {
private:
    Player player;

public:
    explicit Game(const Player& p);

    void start();
    void displayBusinesses() const;
    void interactiveMenu();
    void interactiveUpgrade();
    void buyBusiness();
    void buyManager();
};
