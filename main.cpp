#ifdef USE_HEADLESS_APP
#include "include/ApplicationHeadless.h"
#else
#include "include/Application.h"
#endif

#include <iostream>

int main() {
    std::cout << "Pornire aplicatie...\n";

    Application app;
    std::cout << "Aplicatie initializata. Rulare...\n";

#ifdef USE_HEADLESS_APP
    auto& game = app.getGame();
    game.getPlayer();
    game.saveGame("test.txt");
    game.loadGame("test.txt");
    game.saveFileExists("test.txt");

    auto& player = game.getPlayer();
    const auto& businesses = player.getBusinesses();
    if (!businesses.empty()) {
        player.startBusinessProduction(0);
        player.purchaseBusiness(0);
        player.upgradeBusiness(0);
    }

    const auto& achievements = player.getAchievements();
    if (!achievements.empty()) {
        achievements[0].getDescription();
    }

    if (!businesses.empty()) {
        businesses[0]->setManagerHired(false);
        businesses[0]->getProgress();
        businesses[0]->getProductionTime();
        businesses[0]->isActive();
    }
#endif

    app.run();

    std::cout << "Aplicatie terminata cu succes.\n";
    return 0;
}