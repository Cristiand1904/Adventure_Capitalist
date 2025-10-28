#ifndef GAME_H
#define GAME_H

#include <vector>
#include <iostream>
#include "Player.h"

class Game {
    Player player;

public:
    Game(const Player& p);
    void showStatus() const;
    void runCycle();
    void interactiveUpgrade();
    bool upgradeBusiness(int index, int times, double pricePerLevel);
};

#endif
