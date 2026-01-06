#pragma once
#include "Game.h"
#include <memory>

class Application {
private:
    std::unique_ptr<Game> game;

public:
    explicit Application(bool headless = false);
    void run();
    void runHeadless();
    Game& getGame() { return *game; }
};