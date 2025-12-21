#include "../include/Application.h"
#include <iostream>
#include <string>
#include <cmath>
#include <cstdio>

#define COLOR_BG        CLITERAL(Color){ 40, 40, 45, 255 }
#define COLOR_BAR_BG    CLITERAL(Color){ 20, 20, 20, 255 }
#define COLOR_BAR_FILL  CLITERAL(Color){ 100, 200, 0, 255 }
#define COLOR_BTN_BUY   CLITERAL(Color){ 255, 140, 0, 255 }
#define COLOR_BTN_MNG   CLITERAL(Color){ 100, 100, 255, 255 }
#define COLOR_ICON_IDLE CLITERAL(Color){ 255, 200, 0, 255 }
#define COLOR_ICON_RUN  CLITERAL(Color){ 150, 150, 150, 255 }
#define COLOR_TEXT      CLITERAL(Color){ 240, 240, 240, 255 }

Application::Application() {
    InitWindow(900, 700, "Adventure Capitalist - Raylib Edition");
    SetTargetFPS(60);

    game = std::make_unique<Game>("Capitalist", 0.0);
    initUI();
}

void Application::initUI() {
    for(int i=0; i<3; ++i) {
        createBusinessUI(i, 120 + i * 110);
    }
}

void Application::createBusinessUI(int index, float yPos) {
    Button startBtn;
    startBtn.rect = { 30, yPos, 80, 80 };
    startBtn.text = "GO!";
    startBtn.color = COLOR_ICON_IDLE;
    startBtn.type = Button::START;
    startBtn.businessIndex = index;
    buttons.push_back(startBtn);

    Button buyBtn;
    buyBtn.rect = { 680, yPos + 10, 180, 60 };
    buyBtn.text = "BUY";
    buyBtn.color = COLOR_BTN_BUY;
    buyBtn.type = Button::UPGRADE;
    buyBtn.businessIndex = index;
    buttons.push_back(buyBtn);

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
    // Actualizam jocul si primim notificari
    std::vector<std::string> newNotifications = game->update(GetFrameTime());
    for (const auto& msg : newNotifications) {
        notifications.push_back({msg, 3.0f}); // 3 secunde
    }

    // Actualizam timer-ul notificarilor
    if (!notifications.empty()) {
        notifications.front().timer -= GetFrameTime();
        if (notifications.front().timer <= 0) {
            notifications.pop_front();
        }
    }

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
                    btn.text = "UPG\n$" + std::to_string((int)b->getManagerUpgradeCost());
                    btn.color = GREEN;
                } else {
                    btn.text = "MNG\n$" + std::to_string((int)b->getManagerCost());
                    btn.color = GRAY;
                }
            } else if (btn.type == Button::START) {
                if (!b->isOwned()) {
                    btn.color = GRAY;
                } else if (b->hasManagerHired()) {
                    btn.color = GREEN;
                } else if (b->isActive()) {
                    btn.color = COLOR_ICON_RUN;
                } else {
                    btn.color = COLOR_ICON_IDLE;
                }
            }
        }
    }
}

void Application::draw() {
    BeginDrawing();
    ClearBackground(COLOR_BG);

    DrawRectangle(0, 0, 900, 80, BLACK);
    DrawText("AdVenture Capitalist", 20, 20, 30, WHITE);

    std::string moneyStr = "$" + std::to_string((long long)game->getPlayer().getMoney());
    int moneyWidth = MeasureText(moneyStr.c_str(), 40);
    DrawText(moneyStr.c_str(), 900 - moneyWidth - 20, 20, 40, YELLOW);

    const auto& businesses = game->getPlayer().getBusinesses();

    for (size_t i = 0; i < businesses.size(); ++i) {
        float yPos = 120 + i * 110;
        const auto& b = businesses[i];

        DrawRectangle(10, yPos - 10, 880, 100, DARKGRAY);

        float barX = 130;
        float barY = yPos + 35;
        float barW = 470;
        float barH = 30;

        DrawRectangle(barX, barY, barW, barH, COLOR_BAR_BG);
        if (b->isOwned()) {
            float progress = b->getProgress();
            DrawRectangle(barX, barY, barW * progress, barH, COLOR_BAR_FILL);
        }

        std::string nameLvl = b->getName();
        if (b->isOwned()) nameLvl += " [" + std::to_string(b->getLevel()) + "]";
        DrawText(nameLvl.c_str(), barX, yPos, 24, WHITE);

        if (b->isOwned()) {
            std::string profitStr = "$" + std::to_string((int)b->getProfitPerCycle());
            // Profitul este acum ALB
            DrawText(profitStr.c_str(), barX, barY + 5, 20, WHITE);

            char timeBuffer[16];
            snprintf(timeBuffer, sizeof(timeBuffer), "%.1fs", b->getProductionTime());
            DrawText(timeBuffer, barX + barW - 60, barY + 5, 20, WHITE);
        } else {
            DrawText("LOCKED", barX + 180, barY + 5, 20, RED);
        }
    }

    for (const auto& btn : buttons) {
        if (btn.type == Button::START) {
            Vector2 center = { btn.rect.x + btn.rect.width/2, btn.rect.y + btn.rect.height/2 };
            DrawCircleV(center, btn.rect.width/2, btn.color);
            DrawCircleLines(center.x, center.y, btn.rect.width/2, BLACK);

            const char* iconText = "Click";
            if (btn.color.r == GREEN.r) iconText = "Auto";
            int w = MeasureText(iconText, 20);
            DrawText(iconText, center.x - w/2, center.y - 10, 20, BLACK);
        }
        else {
            DrawRectangleRec(btn.rect, btn.color);
            DrawRectangleLinesEx(btn.rect, 2, BLACK);

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

    // Afisare Notificari (Pop-up)
    if (!notifications.empty()) {
        const auto& notif = notifications.front();
        int textWidth = MeasureText(notif.text.c_str(), 30);
        int boxWidth = textWidth + 40;
        int boxHeight = 100;
        int boxX = 450 - boxWidth / 2;
        int boxY = 350 - boxHeight / 2;

        DrawRectangle(boxX, boxY, boxWidth, boxHeight, BLACK);
        DrawRectangleLines(boxX, boxY, boxWidth, boxHeight, YELLOW);
        DrawText(notif.text.c_str(), boxX + 20, boxY + 35, 30, YELLOW);
    }

    EndDrawing();
}