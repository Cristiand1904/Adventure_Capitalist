#pragma once
#include "raylib.h"
#include "Game.h"
#include <memory>
#include <vector>
#include <string>
#include <deque>

class Application {
private:
    std::unique_ptr<Game> game;

    enum class AppState { MENU, GAME };
    AppState currentState;
    float stateTransitionTimer;

    struct Button {
        Rectangle rect;
        std::string text;
        Color color;
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

    void update();
    void draw();

    void updateMenu();
    void drawMenu();

    void updateGame();
    void updateGameNotifications();
    void updateGameInput();
    void updateGameButtonsState();
    void handleButtonClick(const Button& btn);

    void drawGame();
    void drawGameHeader();
    void drawGameBusinesses();
    void drawGameButtons();
    void drawGameNotifications();

    void createBusinessUI(int index, float yPos);
    bool isButtonClicked(Button& btn);

public:
    Application();
    void run();
};