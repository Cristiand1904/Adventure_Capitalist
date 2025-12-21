#pragma once
#include "raylib.h"
#include "Game.h"
#include <memory>
#include <vector>
#include <string>

class Application {
private:
    std::unique_ptr<Game> game;

    struct Button {
        Rectangle rect;
        std::string text;
        Color color;
        int businessIndex;
        enum Type { BUY, UPGRADE, MANAGER, START } type; // Am adaugat START
    };
    std::vector<Button> buttons;

    void initUI();
    void update();
    void draw();
    void createBusinessUI(int index, float yPos);
    bool isButtonClicked(const Button& btn);

public:
    Application();
    void run();
};