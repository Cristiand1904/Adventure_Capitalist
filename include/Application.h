#pragma once
#include "raylib.h"
#include "Game.h"
#include <memory>
#include <vector>
#include <string>

class Application {
private:
    std::unique_ptr<Game> game;

    // Structura simpla pentru butoane
    struct Button {
        Rectangle rect;
        std::string text;
        Color color;
        int businessIndex; // -1 pentru butoane generale
        enum Type { BUY, UPGRADE, MANAGER, PROFIT } type;
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