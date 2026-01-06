#include "../include/Application.h"
#include <iostream>
#include <string>
#include <cmath>
#include <cstdio>
#include <filesystem>

#define COLOR_BG        CLITERAL(Color){ 30, 30, 35, 255 }
#define COLOR_PANEL     CLITERAL(Color){ 50, 50, 55, 255 }
#define COLOR_ACCENT    CLITERAL(Color){ 255, 200, 0, 255 }
#define COLOR_GREEN     CLITERAL(Color){ 100, 220, 50, 255 }
#define COLOR_RED       CLITERAL(Color){ 220, 60, 60, 255 }
#define COLOR_BLUE      CLITERAL(Color){ 60, 120, 220, 255 }
#define COLOR_ICON_IDLE CLITERAL(Color){ 255, 200, 0, 255 }
#define COLOR_ICON_RUN  CLITERAL(Color){ 150, 150, 150, 255 }
#define COLOR_TEXT      CLITERAL(Color){ 240, 240, 240, 255 }

Application::Application(bool headless) {
    if (!headless) {
        InitWindow(1000, 750, "Adventure Capitalist - Ultimate Edition");
        SetTargetFPS(60);
    }

    currentState = AppState::MENU;
    stateTransitionTimer = 0.0f;

    game = std::make_unique<Game>("Capitalist", 0.0);
    initUI();
}

void Application::initUI() {
    initMenuUI();
}

void Application::initMenuUI() {
    menuButtons.clear();

    Button newGameBtn;
    newGameBtn.rect = { 350, 300, 300, 80 };
    newGameBtn.text = "NEW GAME";
    newGameBtn.color = COLOR_GREEN;
    newGameBtn.type = Button::NEW_GAME;
    newGameBtn.isPressed = false;
    menuButtons.push_back(newGameBtn);

    if (Game::saveFileExists()) {
        Button loadGameBtn;
        loadGameBtn.rect = { 350, 400, 300, 80 };
        loadGameBtn.text = "LOAD GAME";
        loadGameBtn.color = COLOR_BLUE;
        loadGameBtn.type = Button::LOAD_GAME;
        loadGameBtn.isPressed = false;
        menuButtons.push_back(loadGameBtn);
    }
}

void Application::initGameUI() {
    gameButtons.clear();
    for(int i=0; i<3; ++i) {
        createBusinessUI(i, 140 + i * 130);
    }

    Button saveBtn;
    saveBtn.rect = { 820, 20, 160, 50 };
    saveBtn.text = "SAVE & EXIT";
    saveBtn.color = COLOR_RED;
    saveBtn.type = Button::SAVE_EXIT;
    saveBtn.businessIndex = -1;
    saveBtn.isPressed = false;
    gameButtons.push_back(saveBtn);

    Button resetBtn;
    resetBtn.rect = { 20, 20, 100, 50 };
    resetBtn.text = "RESET";
    resetBtn.color = COLOR_BLUE;
    resetBtn.type = Button::RESET;
    resetBtn.businessIndex = -1;
    resetBtn.isPressed = false;
    gameButtons.push_back(resetBtn);
}

void Application::createBusinessUI(int index, float yPos) {
    Button startBtn;
    startBtn.rect = { 50, yPos, 90, 90 };
    startBtn.text = "GO!";
    startBtn.color = COLOR_ACCENT;
    startBtn.type = Button::START;
    startBtn.businessIndex = index;
    startBtn.isPressed = false;
    gameButtons.push_back(startBtn);

    Button buyBtn;
    buyBtn.rect = { 750, yPos + 15, 200, 60 };
    buyBtn.text = "BUY";
    buyBtn.color = COLOR_ACCENT;
    buyBtn.type = Button::UPGRADE;
    buyBtn.businessIndex = index;
    buyBtn.isPressed = false;
    gameButtons.push_back(buyBtn);

    Button mngBtn;
    mngBtn.rect = { 680, yPos + 15, 60, 60 };
    mngBtn.text = "M";
    mngBtn.color = COLOR_BLUE;
    mngBtn.type = Button::MANAGER;
    mngBtn.businessIndex = index;
    mngBtn.isPressed = false;
    gameButtons.push_back(mngBtn);
}

void Application::run() {
    while (!WindowShouldClose()) {
        update();
        draw();
    }
    CloseWindow();
}

void Application::runHeadless() {
    std::cout << "Running in HEADLESS mode (CI/CD)...\n";
    for (int i = 0; i < 10; ++i) {
        game->update(1.0f / 60.0f);
    }
    std::cout << "Headless run completed successfully.\n";
}

bool Application::isButtonClicked(Button& btn) {
    if (stateTransitionTimer > 0.0f) return false;

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
    if (stateTransitionTimer > 0.0f) {
        stateTransitionTimer -= GetFrameTime();
    }

    if (currentState == AppState::MENU) {
        updateMenu();
    } else {
        updateGame();
    }
}

void Application::updateMenu() {
    for (auto& btn : menuButtons) {
        if (isButtonClicked(btn)) {
            if (btn.type == Button::NEW_GAME) {
                game = std::make_unique<Game>("Capitalist", 0.0);
                initGameUI();
                currentState = AppState::GAME;
                stateTransitionTimer = 0.5f;
                return;
            } else if (btn.type == Button::LOAD_GAME) {
                game = std::make_unique<Game>("Capitalist", 0.0);
                if (game->loadGame()) {
                    initGameUI();
                    currentState = AppState::GAME;
                    stateTransitionTimer = 0.5f;
                    return;
                }
            }
        }
    }
}

void Application::updateGame() {
    updateGameNotifications();
    updateGameInput();
    updateGameButtonsState();
}

void Application::updateGameNotifications() {
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
}

void Application::updateGameInput() {
    for (auto& btn : gameButtons) {
        if (isButtonClicked(btn)) {
            handleButtonClick(btn);
        }
    }
}

void Application::handleButtonClick(const Button& btn) {
    try {
        if (btn.type == Button::SAVE_EXIT) {
            game->saveGame();
            CloseWindow();
            exit(0);
        } else if (btn.type == Button::RESET) {
            if (Game::saveFileExists()) {
                std::filesystem::remove("savegame.txt");
            }
            game = std::make_unique<Game>("Capitalist", 0.0);
            notifications.push_back({"GAME RESET!", 2.0f});
        } else if (btn.type == Button::START) {
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

void Application::updateGameButtonsState() {
    const auto& businesses = game->getPlayer().getBusinesses();

    for (auto& btn : gameButtons) {
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

    if (currentState == AppState::MENU) {
        drawMenu();
    } else {
        drawGame();
    }

    EndDrawing();
}

void Application::drawMenu() {
    DrawText("AdVenture Capitalist", 250, 150, 50, COLOR_ACCENT);

    for (const auto& btn : menuButtons) {
        Rectangle drawRect = btn.rect;
        if (btn.isPressed) {
            drawRect.x += 2; drawRect.y += 2;
            drawRect.width -= 4; drawRect.height -= 4;
        }
        DrawRectangleRounded(drawRect, 0.2f, 10, btn.color);
        DrawRectangleRoundedLines(drawRect, 0.2f, 10, BLACK);

        int w = MeasureText(btn.text.c_str(), 30);
        DrawText(btn.text.c_str(),
                 drawRect.x + drawRect.width/2 - w/2,
                 drawRect.y + drawRect.height/2 - 15,
                 30, WHITE);
    }
}

void Application::drawGame() {
    drawGameHeader();
    drawGameBusinesses();
    drawGameButtons();
    drawGameNotifications();
}

void Application::drawGameHeader() {
    DrawRectangle(0, 0, 1000, 100, BLACK);
    DrawText("AdVenture Capitalist", 150, 30, 40, WHITE);

    std::string moneyStr = "$" + std::to_string((long long)game->getPlayer().getMoney());
    int moneyWidth = MeasureText(moneyStr.c_str(), 50);
    DrawText(moneyStr.c_str(), 780 - moneyWidth, 25, 50, COLOR_ACCENT);
}

void Application::drawGameBusinesses() {
    const auto& businesses = game->getPlayer().getBusinesses();

    for (size_t i = 0; i < businesses.size(); ++i) {
        float yPos = 140 + i * 130;
        const auto& b = businesses[i];

        DrawRectangleRounded({20, yPos - 10, 960, 110}, 0.1f, 10, COLOR_PANEL);
        DrawRectangleRoundedLines({20, yPos - 10, 960, 110}, 0.1f, 10, BLACK);

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
}

void Application::drawGameButtons() {
    for (const auto& btn : gameButtons) {
        Rectangle drawRect = btn.rect;
        if (btn.isPressed) {
            drawRect.x += 2; drawRect.y += 2;
            drawRect.width -= 4; drawRect.height -= 4;
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
}

void Application::drawGameNotifications() {
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
}