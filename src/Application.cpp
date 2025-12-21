#include "../include/Application.h"
#include <iostream>
#include <string>

Application::Application() {
    // Initializam fereastra Raylib
    InitWindow(800, 600, "Adventure Capitalist - Raylib Edition");
    SetTargetFPS(60);

    game = std::make_unique<Game>("Capitalist", 50.0);
    initUI();
}

void Application::initUI() {
    // Buton Profit
    Button profitBtn;
    profitBtn.rect = { 550, 50, 200, 50 };
    profitBtn.text = "COLLECT PROFIT";
    profitBtn.color = GREEN;
    profitBtn.type = Button::PROFIT;
    profitBtn.businessIndex = -1;
    buttons.push_back(profitBtn);

    for(int i=0; i<3; ++i) {
        createBusinessUI(i, 150 + i * 120);
    }
}

void Application::createBusinessUI(int index, float yPos) {
    // Buton Buy/Upgrade
    Button actionBtn;
    actionBtn.rect = { 500, yPos, 100, 40 };
    actionBtn.text = "Action";
    actionBtn.color = BLUE;
    actionBtn.type = Button::UPGRADE;
    actionBtn.businessIndex = index;
    buttons.push_back(actionBtn);

    // Buton Manager
    Button managerBtn;
    managerBtn.rect = { 620, yPos, 100, 40 };
    managerBtn.text = "Manager";
    managerBtn.color = PURPLE;
    managerBtn.type = Button::MANAGER;
    managerBtn.businessIndex = index;
    buttons.push_back(managerBtn);
}

void Application::run() {
    while (!WindowShouldClose()) {
        update();
        draw();
    }
    CloseWindow();
}

bool Application::isButtonClicked(const Button& btn) {
    Vector2 mousePoint = GetMousePosition();
    return CheckCollisionPointRec(mousePoint, btn.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

void Application::update() {
    // Verificam click-urile
    for (const auto& btn : buttons) {
        if (isButtonClicked(btn)) {
            try {
                if (btn.type == Button::PROFIT) {
                    game->getPlayer().earnProfit();
                } else if (btn.type == Button::UPGRADE) {
                    const auto& businesses = game->getPlayer().getBusinesses();
                    if (btn.businessIndex >= 0 && static_cast<size_t>(btn.businessIndex) < businesses.size()) {
                        if (businesses[btn.businessIndex]->isOwned()) {
                            game->getPlayer().upgradeBusiness(btn.businessIndex);
                        } else {
                            game->getPlayer().purchaseBusiness(btn.businessIndex);
                        }
                    }
                } else if (btn.type == Button::MANAGER) {
                    game->getPlayer().hireManager(btn.businessIndex);
                }
            } catch (const std::exception& e) {
                std::cout << "Eroare: " << e.what() << std::endl;
            }
        }
    }

    // Actualizam starea butoanelor (text si culoare)
    const auto& businesses = game->getPlayer().getBusinesses();
    size_t btnIdx = 1; // Sarim peste profit

    for (size_t i = 0; i < businesses.size(); ++i) {
        if (btnIdx < buttons.size()) {
            if (businesses[i]->isOwned()) {
                buttons[btnIdx].text = "Upgrade\n" + std::to_string((int)businesses[i]->getUpgradeCost()) + "$";
                buttons[btnIdx].color = BLUE;
            } else {
                buttons[btnIdx].text = "Buy\n" + std::to_string((int)businesses[i]->getPurchaseCost()) + "$";
                buttons[btnIdx].color = RED;
            }
            btnIdx++;
        }

        if (btnIdx < buttons.size()) {
            if (businesses[i]->hasManagerHired()) {
                buttons[btnIdx].text = "Manager\nActiv";
                buttons[btnIdx].color = GRAY;
            } else {
                buttons[btnIdx].text = "Hire Mng\n" + std::to_string((int)businesses[i]->getManagerCost()) + "$";
                buttons[btnIdx].color = PURPLE;
            }
            btnIdx++;
        }
    }
}

void Application::draw() {
    BeginDrawing();
    ClearBackground(DARKGRAY);

    DrawText("Adventure Capitalist", 250, 10, 30, WHITE);

    std::string moneyStr = "Bani: " + std::to_string((int)game->getPlayer().getMoney()) + "$";
    DrawText(moneyStr.c_str(), 20, 50, 24, YELLOW);

    // Desenam butoanele
    for (const auto& btn : buttons) {
        DrawRectangleRec(btn.rect, btn.color);
        // Centrare text simplificata
        int textWidth = MeasureText(btn.text.c_str(), 10); // Font mic default
        DrawText(btn.text.c_str(),
                 btn.rect.x + btn.rect.width/2 - textWidth/2,
                 btn.rect.y + btn.rect.height/2 - 5,
                 10, WHITE);
    }

    // Desenam info business
    const auto& businesses = game->getPlayer().getBusinesses();
    for (size_t i = 0; i < businesses.size(); ++i) {
        std::string info = businesses[i]->getName() + " (Lvl " + std::to_string(businesses[i]->getLevel()) + ")";
        info += "\nProfit: " + std::to_string((int)businesses[i]->getProfitPerCycle()) + "$";

        Color textColor = WHITE;
        if (!businesses[i]->isOwned()) {
            info += " [BLOCAT]";
            textColor = LIGHTGRAY;
        }

        DrawText(info.c_str(), 20, 150 + i * 120, 20, textColor);
    }

    EndDrawing();
}