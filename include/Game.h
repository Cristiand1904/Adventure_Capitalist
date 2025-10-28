#ifndef GAME_H
#define GAME_H

#include "Player.h"

class Game {
private:
    Player player;

public:
    Game(const Player& p);
    ~Game();

    bool upgradeBusiness(int index, int times, double pricePerLevel);
    double runCycles(int cycles);

    void showStatus() const;

    Player& accessPlayer();
    const Player& getPlayer() const;

    friend std::ostream& operator<<(std::ostream& os, const Game& g);
};

#endif
