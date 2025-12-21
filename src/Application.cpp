#include "../include/Application.h"
#include <iostream>
#include <string>
#include <cmath>
#include <cstdio>

// Culori
#define COLOR_BG        CLITERAL(Color){ 30, 30, 35, 255 }
#define COLOR_PANEL     CLITERAL(Color){ 50, 50, 55, 255 }
#define COLOR_ACCENT    CLITERAL(Color){ 255, 200, 0, 255 } // Galben auriu
#define COLOR_GREEN     CLITERAL(Color){ 100, 220, 50, 255 }
#define COLOR_RED       CLITERAL(Color){ 220, 60, 60, 255 }
#define COLOR_BLUE      CLITERAL(Color){ 60, 120, 220, 255 }

Application::Application() {
    InitWindow(1000, 750, "Adventure Capitalist - Ultimate Edition");
    SetTargetFPS(60);

    game = std::make_unique<Game>("Capitalist", 0.0);
    initUI();
}

void Application::initUI() {
    for(int i=0; i<3; ++i) {
        createBusinessUI(i, 140 + i * 130);
    }
}

void Application::createBusinessUI(int index, float yPos) {
    // START (Cerc)
    Button startBtn;
    startBtn.rect = { 50, yPos, 90, 90 };
    startBtn.text = "GO!";
    startBtn.color = COLOR_ACCENT;
    startBtn.type = Button::START;
    startBtn.businessIndex = index;
    startBtn.isPressed = false;
    buttons.push_back(startBtn);

    // BUY
    Button buyBtn;
    buyBtn.rect = { 750, yPos + 15, 200, 60 };
    buyBtn.text = "BUY";
    buyBtn.color = COLOR_ACCENT;
    buyBtn.type = Button::UPGRADE;
    buyBtn.businessIndex = index;
    buyBtn.isPressed = false;
    buttons.push_back(buyBtn);

    // MANAGER
    Button mngBtn;
    mngBtn.rect = { 680, yPos + 15, 60, 60 };
    mngBtn.text = "M";
    mngBtn.color = COLOR_BLUE;
    mngBtn.type = Button::MANAGER;
    mngBtn.businessIndex = index;
    mngBtn.isPressed = false;
    buttons.push_back(mngBtn);
}

void Application::run() {
    while (!WindowShouldClose()) {
        update();
        draw();
    }
    CloseWindow();
}

bool Application::isButtonClicked(Button& btn) {
    Vector2 mousePoint = GetMousePosition();
    bool hover = CheckCollisionPointRec(mousePoint, btn.rect);

    if (hover && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        btn.isPressed = true;
    } else {
        btn.isPressed = false;
    }

    return hover && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
}

void Application::update() {
    std::vector<std::string> newNotifications = game->update(GetFrameTime());
    for (const auto& msg : newNotifications) {
        notifications.push_back({msg, 3.0f});
    }

    if (!notifications.empty()) {
        notifications.front().timer -= GetFrameTime();
        if (notifications.front().timer <= 0) {
            notifications.pop_front();
        }
    }

    for (auto& btn : buttons) {
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
                    const auto& businesses = game->getPlayer().getBusinesses();
                    if (businesses[btn.businessIndex]->hasManagerHired()) {
                        game->getPlayer().upgradeManager(btn.businessIndex);
                    } else {
                        game->getPlayer().hireManager(btn.businessIndex);
                    }
                }
            } catch (const std::exception& e) {
                std::cout << "Eroare: " << e.what() << std::endl;
            }
        }
    }

    const auto& businesses = game->getPlayer().getBusinesses();

    for (auto& btn : buttons) {
        if (btn.businessIndex >= 0 && static_cast<size_t>(btn.businessIndex) < businesses.size()) {
            const auto& b = businesses[btn.businessIndex];

            if (btn.type == Button::UPGRADE) {
                if (b->isOwned()) {
                    btn.text = "BUY x1\n$" + std::to_string((int)b->getUpgradeCost());
                    btn.color = COLOR_ACCENT;
                } else {
                    if (b->getPurchaseCost() == 0) {
                        btn.text = "FREE!";
                        btn.color = COLOR_GREEN;
                    } else {
                        btn.text = "UNLOCK\n$" + std::to_string((int)b->getPurchaseCost());
                        btn.color = COLOR_RED;
                    }
                }
            } else if (btn.type == Button::MANAGER) {
                if (b->hasManagerHired()) {
                    btn.text = "UPG\n$" + std::to_string((int)b->getManagerUpgradeCost());
                    btn.color = COLOR_GREEN;
                } else {
                    btn.text = "MNG\n$" + std::to_string((int)b->getManagerCost());
                    btn.color = GRAY;
                }
            } else if (btn.type == Button::START) {
                if (!b->isOwned()) {
                    btn.color = GRAY;
                } else if (b->hasManagerHired()) {
                    btn.color = COLOR_GREEN;
                } else if (b->isActive()) {
                    btn.color = LIGHTGRAY;
                } else {
                    btn.color = COLOR_ACCENT;
                }
            }
        }
    }
}

void Application::draw() {
    BeginDrawing();
    ClearBackground(COLOR_BG);

    // Header
    DrawRectangle(0, 0, 1000, 100, BLACK);
    DrawText("AdVenture Capitalist", 30, 30, 40, WHITE);

    std::string moneyStr = "$" + std::to_string((long long)game->getPlayer().getMoney());
    int moneyWidth = MeasureText(moneyStr.c_str(), 50);
    DrawText(moneyStr.c_str(), 1000 - moneyWidth - 30, 25, 50, COLOR_ACCENT);

    const auto& businesses = game->getPlayer().getBusinesses();

    for (size_t i = 0; i < businesses.size(); ++i) {
        float yPos = 140 + i * 130;
        const auto& b = businesses[i];

        // Panel
        DrawRectangleRounded({20, yPos - 10, 960, 110}, 0.1f, 10, COLOR_PANEL);
        // Eliminat parametrul de grosime care cauza eroarea
        DrawRectangleRoundedLines({20, yPos - 10, 960, 110}, 0.1f, 10, BLACK);

        // Bara Progres
        float barX = 160;
        float barY = yPos + 45;
        float barW = 500;
        float barH = 35;

        DrawRectangle(barX, barY, barW, barH, BLACK);
        if (b->isOwned()) {
            float progress = b->getProgress();
            DrawRectangle(barX, barY, barW * progress, barH, COLOR_GREEN);
            if (progress >= 1.0f) {
                DrawRectangleLinesEx({barX, barY, barW, barH}, 2, WHITE);
            }
        }

        // Info
        std::string nameLvl = b->getName();
        if (b->isOwned()) nameLvl += "  [Lv " + std::to_string(b->getLevel()) + "]";
        DrawText(nameLvl.c_str(), barX, yPos + 5, 30, WHITE);

        if (b->isOwned()) {
            std::string profitStr = "$" + std::to_string((int)b->getProfitPerCycle());
            DrawText(profitStr.c_str(), barX + 10, barY + 5, 24, WHITE);

            char timeBuffer[16];
            snprintf(timeBuffer, sizeof(timeBuffer), "%.1fs", b->getProductionTime());
            DrawText(timeBuffer, barX + barW - 80, barY + 5, 24, WHITE);
        } else {
            DrawText("LOCKED", barX + 200, barY + 5, 24, COLOR_RED);
        }
    }

    // Butoane
    for (const auto& btn : buttons) {
        Rectangle drawRect = btn.rect;
        if (btn.isPressed) {
            drawRect.x += 2;
            drawRect.y += 2;
            drawRect.width -= 4;
            drawRect.height -= 4;
        }

        if (btn.type == Button::START) {
            Vector2 center = { drawRect.x + drawRect.width/2, drawRect.y + drawRect.height/2 };
            DrawCircleV(center, drawRect.width/2, btn.color);
            DrawCircleLines(center.x, center.y, drawRect.width/2, BLACK);

            const char* iconText = "Click";
            if (btn.color.r == COLOR_GREEN.r) iconText = "Auto";
            int w = MeasureText(iconText, 20);
            DrawText(iconText, center.x - w/2, center.y - 10, 20, BLACK);
        }
        else {
            DrawRectangleRounded(drawRect, 0.2f, 10, btn.color);
            // Eliminat parametrul de grosime
            DrawRectangleRoundedLines(drawRect, 0.2f, 10, BLACK);

            if (btn.text.find('\n') != std::string::npos) {
                size_t pos = btn.text.find('\n');
                std::string line1 = btn.text.substr(0, pos);
                std::string line2 = btn.text.substr(pos + 1);

                int w1 = MeasureText(line1.c_str(), 20);
                int w2 = MeasureText(line2.c_str(), 20);

                DrawText(line1.c_str(), drawRect.x + drawRect.width/2 - w1/2, drawRect.y + 10, 20, WHITE);
                DrawText(line2.c_str(), drawRect.x + drawRect.width/2 - w2/2, drawRect.y + 35, 20, WHITE);
            } else {
                int w = MeasureText(btn.text.c_str(), 20);
                DrawText(btn.text.c_str(),
                         drawRect.x + drawRect.width/2 - w/2,
                         drawRect.y + drawRect.height/2 - 10,
                         20, WHITE);
            }
        }
    }

    // Notificari
    if (!notifications.empty()) {
        const auto& notif = notifications.front();
        int textWidth = MeasureText(notif.text.c_str(), 30);
        int boxW = textWidth + 60;
        int boxH = 120;
        int boxX = 500 - boxW / 2;
        int boxY = 375 - boxH / 2;

        DrawRectangle(boxX, boxY, boxW, boxH, BLACK);
        DrawRectangleLinesEx({(float)boxX, (float)boxY, (float)boxW, (float)boxH}, 4, COLOR_ACCENT);

        if (notif.text.find('\n') != std::string::npos) {
             size_t pos = notif.text.find('\n');
             std::string line1 = notif.text.substr(0, pos);
             std::string line2 = notif.text.substr(pos + 1);

             int w1 = MeasureText(line1.c_str(), 30);
             int w2 = MeasureText(line2.c_str(), 30);

             DrawText(line1.c_str(), boxX + boxW/2 - w1/2, boxY + 20, 30, COLOR_ACCENT);
             DrawText(line2.c_str(), boxX + boxW/2 - w2/2, boxY + 60, 30, WHITE);
        } else {
             DrawText(notif.text.c_str(), boxX + 30, boxY + 45, 30, COLOR_ACCENT);
        }
    }

    EndDrawing();
}