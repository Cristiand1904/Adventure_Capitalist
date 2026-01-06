#pragma once
#ifndef HEADLESS_BUILD
#include <SFML/Graphics.hpp>
#endif
#include "Game.h"
#include <memory>
#include <vector>
#include <string>
#include <deque>

class Application {
private:
    std::unique_ptr<Game> game;

#ifndef HEADLESS_BUILD
    sf::RenderWindow window;
    sf::Font font;
    sf::Clock clock;
#endif

    enum class AppState { MENU, GAME };
    AppState currentState;
    float stateTransitionTimer;

    struct Button {
#ifndef HEADLESS_BUILD
        sf::FloatRect rect;
        sf::Color color;
#else
        float rect[4];
        int color;
#endif
        std::string text;
        int businessIndex;
        enum Type { BUY, UPGRADE, MANAGER, START, NEW_GAME, LOAD_GAME, SAVE_EXIT, RESET } type;
        bool isPressed;
    };
    std::vector<Button> gameButtons;
    std::vector<Button> menuButtons;

    struct Notification {
        std::string text;
        float timer;
    };
    std::deque<Notification> notifications;

    void initUI();
    void initMenuUI();
    void initGameUI();

    void update(float dt);
    void draw();

    void updateMenu(float dt);
    void drawMenu();

    void updateGame(float dt);
    void updateGameNotifications(float dt);
    void updateGameInput();
    void updateGameButtonsState();
    void handleButtonClick(const Button& btn);

    void drawGame();
    void drawGameHeader();
    void drawGameBusinesses();
    void drawGameButtons();
    void drawGameNotifications();

    void createBusinessUI(int index, float yPos);

#ifndef HEADLESS_BUILD
    bool isButtonClicked(Button& btn, const sf::Vector2i& mousePos);
    void drawButton(const Button& btn, bool isCircle = false);
    void drawText(const std::string& text, float x, float y, int size, sf::Color color);
    int getTextWidth(const std::string& text, int size);
#endif

public:
    Application(bool headless = false);
    void run();
    void runHeadless();
};