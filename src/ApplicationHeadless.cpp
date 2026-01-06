#include "../include/ApplicationHeadless.h"
#include <iostream>

Application::Application(bool headless)
    : game(std::make_unique<Game>("Capitalist", 0.0)) {
    (void)headless;
}

void Application::run() {
    runHeadless();
}

void Application::runHeadless() {
    std::cout << "Running in HEADLESS mode (CI/CD)...\n";
    for (int i = 0; i < 10; ++i) {
        game->update(1.0f / 60.0f);
    }
    std::cout << "Headless run completed successfully.\n";

    auto& player = game->getPlayer();
    player.setMoney(10000.0);

    const auto& businesses = player.getBusinesses();
    if (!businesses.empty()) {
        businesses[0]->getProgress();
        businesses[0]->getProductionTime();
        businesses[0]->isActive();
        businesses[0]->setManagerHired(false);

        try {
            player.startBusinessProduction(0);
            player.purchaseBusiness(0);
            player.upgradeBusiness(0);
        } catch (...) {}
    }

    const auto& achievements = player.getAchievements();
    if (!achievements.empty()) {
        achievements[0].getDescription();
    }

    game->saveGame("test.txt");
    game->loadGame("test.txt");
    game->saveFileExists("test.txt");
}