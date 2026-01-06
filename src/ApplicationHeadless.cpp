#include "../include/ApplicationHeadless.h"
#include <iostream>

Application::Application(bool headless) {
    (void)headless;
    game = std::make_unique<Game>("Capitalist", 0.0);
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
}