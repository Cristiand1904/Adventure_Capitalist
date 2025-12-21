#include "../include/Application.h"
#include <iostream>
#include <string>
#include <cmath>
#include <cstdio>

// Culori personalizate inspirate din AdVenture Capitalist
#define COLOR_BG        CLITERAL(Color){ 40, 40, 45, 255 }      // Gri inchis fundal
#define COLOR_BAR_BG    CLITERAL(Color){ 20, 20, 20, 255 }      // Negru bara
#define COLOR_BAR_FILL  CLITERAL(Color){ 100, 200, 0, 255 }     // Verde crud bara
#define COLOR_BTN_BUY   CLITERAL(Color){ 255, 140, 0, 255 }     // Portocaliu Buy
#define COLOR_BTN_MNG   CLITERAL(Color){ 100, 100, 255, 255 }   // Albastru Manager
#define COLOR_ICON_IDLE CLITERAL(Color){ 255, 200, 0, 255 }     // Galben Icon (Ready)
#define COLOR_ICON_RUN  CLITERAL(Color){ 150, 150, 150, 255 }   // Gri Icon (Running)
#define COLOR_TEXT      CLITERAL(Color){ 240, 240, 240, 255 }   // Alb Text

Application::Application() {
    InitWindow(900, 700, "Adventure Capitalist - Raylib Edition");
    SetTargetFPS(60);

    // Incepem cu 0 bani, dar Limonada e gratis!
    game = std::make_unique<Game>("Capitalist", 0.0);
    initUI();
}

void Application::initUI() {
    // Generam UI pentru cele 3 afaceri
    for(int i=0; i<3; ++i) {
        createBusinessUI(i, 120 + i * 110); // Spatiere mai mare
    }
}

void Application::createBusinessUI(int index, float yPos) {
    // 1. Butonul rotund din stanga (START)
    Button startBtn;
    startBtn.rect = { 30, yPos, 80, 80 };
    startBtn.text = "GO!";
    startBtn.color = COLOR_ICON_IDLE;
    startBtn.type = Button::START;
    startBtn.businessIndex = index;
    buttons.push_back(startBtn);

    // 2. Butonul BUY (Dreapta)
    Button buyBtn;
    buyBtn.rect = { 680, yPos + 10, 180, 60 };
    buyBtn.text = "BUY";
    buyBtn.color = COLOR_BTN_BUY;
    buyBtn.type = Button::UPGRADE;
    buyBtn.businessIndex = index;
    buttons.push_back(buyBtn);

    // 3. Butonul MANAGER (Mic, langa Buy)
    Button mngBtn;
    mngBtn.rect = { 620, yPos + 10, 50, 60 };
    mngBtn.text = "M";
    mngBtn.color = COLOR_BTN_MNG;
    mngBtn.type = Button::MANAGER;
    mngBtn.businessIndex = index;
    buttons.push_back(mngBtn);
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
    game->update(GetFrameTime());

    // Input
    for (const auto& btn : buttons) {
        if (isButtonClicked(btn)) {
            try {
                if (btn.type == Button::START) {
                    game->getPlayer().startBusinessProduction(btn.businessIndex);
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

    // Update stari butoane
    const auto& businesses = game->getPlayer().getBusinesses();

    for (auto& btn : buttons) {
        if (btn.businessIndex >= 0 && static_cast<size_t>(btn.businessIndex) < businesses.size()) {
            const auto& b = businesses[btn.businessIndex];

            if (btn.type == Button::UPGRADE) {
                if (b->isOwned()) {
                    btn.text = "BUY x1\n$" + std::to_string((int)b->getUpgradeCost());
                    btn.color = COLOR_BTN_BUY;
                } else {
                    if (b->getPurchaseCost() == 0) {
                        btn.text = "FREE!";
                        btn.color = GREEN;
                    } else {
                        btn.text = "UNLOCK\n$" + std::to_string((int)b->getPurchaseCost());
                        btn.color = RED;
                    }
                }
            } else if (btn.type == Button::MANAGER) {
                if (b->hasManagerHired()) {
                    btn.color = GREEN; // Manager activ
                } else {
                    btn.color = GRAY; // Manager inactiv
                }
            } else if (btn.type == Button::START) {
                if (!b->isOwned()) {
                    btn.color = GRAY;
                } else if (b->hasManagerHired()) {
                    btn.color = GREEN; // Auto
                } else if (b->isActive()) {
                    btn.color = COLOR_ICON_RUN; // Running
                } else {
                    btn.color = COLOR_ICON_IDLE; // Ready
                }
            }
        }
    }
}

void Application::draw() {
    BeginDrawing();
    ClearBackground(COLOR_BG);

    // --- HEADER ---
    DrawRectangle(0, 0, 900, 80, BLACK);
    DrawText("AdVenture Capitalist", 20, 20, 30, WHITE);

    std::string moneyStr = "$" + std::to_string((long long)game->getPlayer().getMoney());
    int moneyWidth = MeasureText(moneyStr.c_str(), 40);
    DrawText(moneyStr.c_str(), 900 - moneyWidth - 20, 20, 40, YELLOW);

    const auto& businesses = game->getPlayer().getBusinesses();

    // --- DESENARE AFACERI ---
    for (size_t i = 0; i < businesses.size(); ++i) {
        float yPos = 120 + i * 110;
        const auto& b = businesses[i];

        // Fundal rand
        DrawRectangle(10, yPos - 10, 880, 100, DARKGRAY);

        // Bara de progres (Mijloc)
        float barX = 130;
        float barY = yPos + 35;
        float barW = 470;
        float barH = 30;

        DrawRectangle(barX, barY, barW, barH, COLOR_BAR_BG);
        if (b->isOwned()) {
            float progress = b->getProgress();
            DrawRectangle(barX, barY, barW * progress, barH, COLOR_BAR_FILL);
        }

        // Text Info
        std::string nameLvl = b->getName();
        if (b->isOwned()) nameLvl += " [" + std::to_string(b->getLevel()) + "]";
        DrawText(nameLvl.c_str(), barX, yPos, 24, WHITE);

        // Profit
        if (b->isOwned()) {
            std::string profitStr = "$" + std::to_string((int)b->getProfitPerCycle());
            DrawText(profitStr.c_str(), barX, barY + 5, 20, BLACK);

            // Timer
            char timeBuffer[16];
            snprintf(timeBuffer, sizeof(timeBuffer), "%.1fs", b->getProductionTime());
            DrawText(timeBuffer, barX + barW - 60, barY + 5, 20, WHITE);
        } else {
            DrawText("LOCKED", barX + 180, barY + 5, 20, RED);
        }
    }

    // --- DESENARE BUTOANE ---
    for (const auto& btn : buttons) {
        // Desenam butonul START ca un cerc
        if (btn.type == Button::START) {
            Vector2 center = { btn.rect.x + btn.rect.width/2, btn.rect.y + btn.rect.height/2 };
            DrawCircleV(center, btn.rect.width/2, btn.color);
            DrawCircleLines(center.x, center.y, btn.rect.width/2, BLACK);

            // Iconita text
            const char* iconText = "Click";
            if (btn.color.r == GREEN.r) iconText = "Auto"; // Hacky check
            int w = MeasureText(iconText, 20);
            DrawText(iconText, center.x - w/2, center.y - 10, 20, BLACK);
        }
        else {
            // Butoane normale (Buy, Manager)
            DrawRectangleRec(btn.rect, btn.color);
            DrawRectangleLinesEx(btn.rect, 2, BLACK);

            // Text centrat
            if (btn.text.find('\n') != std::string::npos) {
                size_t pos = btn.text.find('\n');
                std::string line1 = btn.text.substr(0, pos);
                std::string line2 = btn.text.substr(pos + 1);

                int w1 = MeasureText(line1.c_str(), 20);
                int w2 = MeasureText(line2.c_str(), 20);

                DrawText(line1.c_str(), btn.rect.x + btn.rect.width/2 - w1/2, btn.rect.y + 10, 20, WHITE);
                DrawText(line2.c_str(), btn.rect.x + btn.rect.width/2 - w2/2, btn.rect.y + 35, 20, WHITE);
            } else {
                int w = MeasureText(btn.text.c_str(), 20);
                DrawText(btn.text.c_str(),
                         btn.rect.x + btn.rect.width/2 - w/2,
                         btn.rect.y + btn.rect.height/2 - 10,
                         20, WHITE);
            }
        }
    }

    EndDrawing();
}