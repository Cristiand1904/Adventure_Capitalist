#include "../include/Application.h"
#include <iostream>
#include <cmath>
#include <filesystem>

#define COLOR_BG        sf::Color(30, 30, 35)
#define COLOR_PANEL     sf::Color(50, 50, 55)
#define COLOR_ACCENT    sf::Color(255, 200, 0)
#define COLOR_GREEN     sf::Color(100, 220, 50)
#define COLOR_RED       sf::Color(220, 60, 60)
#define COLOR_BLUE      sf::Color(60, 120, 220)
#define COLOR_ICON_IDLE sf::Color(255, 200, 0)
#define COLOR_ICON_RUN  sf::Color(150, 150, 150)
#define COLOR_TEXT      sf::Color(240, 240, 240)
#define COLOR_BLACK     sf::Color::Black
#define COLOR_WHITE     sf::Color::White
#define COLOR_GRAY      sf::Color(128, 128, 128)

Application::Application(bool headless) {
    (void)headless;

    window.create(sf::VideoMode(1000, 750), "Adventure Capitalist - Ultimate Edition");
    window.setFramerateLimit(60);

    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
            if (!font.loadFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
                std::cerr << "Failed to load font!\n";
            }
        }
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
    newGameBtn.rect = sf::FloatRect(350, 300, 300, 80);
    newGameBtn.text = "NEW GAME";
    newGameBtn.color = COLOR_GREEN;
    newGameBtn.type = Button::NEW_GAME;
    newGameBtn.isPressed = false;
    menuButtons.push_back(newGameBtn);

    if (Game::saveFileExists()) {
        Button loadGameBtn;
        loadGameBtn.rect = sf::FloatRect(350, 400, 300, 80);
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
    saveBtn.rect = sf::FloatRect(820, 20, 160, 50);
    saveBtn.text = "SAVE & EXIT";
    saveBtn.color = COLOR_RED;
    saveBtn.type = Button::SAVE_EXIT;
    saveBtn.businessIndex = -1;
    saveBtn.isPressed = false;
    gameButtons.push_back(saveBtn);

    Button resetBtn;
    resetBtn.rect = sf::FloatRect(20, 20, 100, 50);
    resetBtn.text = "RESET";
    resetBtn.color = COLOR_BLUE;
    resetBtn.type = Button::RESET;
    resetBtn.businessIndex = -1;
    resetBtn.isPressed = false;
    gameButtons.push_back(resetBtn);
}

void Application::createBusinessUI(int index, float yPos) {
    Button startBtn;
    startBtn.rect = sf::FloatRect(50, yPos, 90, 90);
    startBtn.text = "GO!";
    startBtn.color = COLOR_ACCENT;
    startBtn.type = Button::START;
    startBtn.businessIndex = index;
    startBtn.isPressed = false;
    gameButtons.push_back(startBtn);

    Button buyBtn;
    buyBtn.rect = sf::FloatRect(750, yPos + 15, 200, 60);
    buyBtn.text = "BUY";
    buyBtn.color = COLOR_ACCENT;
    buyBtn.type = Button::UPGRADE;
    buyBtn.businessIndex = index;
    buyBtn.isPressed = false;
    gameButtons.push_back(buyBtn);

    Button mngBtn;
    mngBtn.rect = sf::FloatRect(680, yPos + 15, 60, 60);
    mngBtn.text = "M";
    mngBtn.color = COLOR_BLUE;
    mngBtn.type = Button::MANAGER;
    mngBtn.businessIndex = index;
    mngBtn.isPressed = false;
    gameButtons.push_back(mngBtn);
}

void Application::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        float dt = clock.restart().asSeconds();
        update(dt);
        draw();
    }
}

void Application::runHeadless() {
    std::cout << "Running in HEADLESS mode (CI/CD)...\n";
    for (int i = 0; i < 10; ++i) {
        game->update(1.0f / 60.0f);
    }
    std::cout << "Headless run completed successfully.\n";
}

bool Application::isButtonClicked(Button& btn, const sf::Vector2i& mousePos) {
    if (stateTransitionTimer > 0.0f) return false;

    bool hover = btn.rect.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

    if (hover && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        btn.isPressed = true;
    } else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (btn.isPressed && hover) {
            btn.isPressed = false;
            return true;
        }
        btn.isPressed = false;
    }

    return false;
}

void Application::update(float dt) {
    if (stateTransitionTimer > 0.0f) {
        stateTransitionTimer -= dt;
    }

    if (currentState == AppState::MENU) {
        updateMenu(dt);
    } else {
        updateGame(dt);
    }
}

void Application::updateMenu(float dt) {
    (void)dt;
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    for (auto& btn : menuButtons) {
        if (isButtonClicked(btn, mousePos)) {
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

void Application::updateGame(float dt) {
    updateGameNotifications(dt);
    updateGameInput();
    updateGameButtonsState();
}

void Application::updateGameNotifications(float dt) {
    std::vector<std::string> newNotifications = game->update(dt);
    for (const auto& msg : newNotifications) {
        notifications.push_back({msg, 3.0f});
    }

    if (!notifications.empty()) {
        notifications.front().timer -= dt;
        if (notifications.front().timer <= 0) {
            notifications.pop_front();
        }
    }
}

void Application::updateGameInput() {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    for (auto& btn : gameButtons) {
        if (isButtonClicked(btn, mousePos)) {
            handleButtonClick(btn);
        }
    }
}

void Application::handleButtonClick(const Button& btn) {
    try {
        if (btn.type == Button::SAVE_EXIT) {
            game->saveGame();
            window.close();
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
                    btn.color = COLOR_GRAY;
                }
            } else if (btn.type == Button::START) {
                if (!b->isOwned()) {
                    btn.color = COLOR_GRAY;
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
    window.clear(COLOR_BG);

    if (currentState == AppState::MENU) {
        drawMenu();
    } else {
        drawGame();
    }

    window.display();
}

void Application::drawMenu() {
    drawText("AdVenture Capitalist", 250, 150, 50, COLOR_ACCENT);

    for (const auto& btn : menuButtons) {
        drawButton(btn);
    }
}

void Application::drawGame() {
    drawGameHeader();
    drawGameBusinesses();
    drawGameButtons();
    drawGameNotifications();
}

void Application::drawGameHeader() {
    sf::RectangleShape header(sf::Vector2f(1000, 100));
    header.setFillColor(COLOR_BLACK);
    window.draw(header);

    drawText("AdVenture Capitalist", 150, 30, 40, COLOR_WHITE);

    std::string moneyStr = "$" + std::to_string((long long)game->getPlayer().getMoney());
    int moneyWidth = getTextWidth(moneyStr, 50);
    drawText(moneyStr, 780 - moneyWidth, 25, 50, COLOR_ACCENT);
}

void Application::drawGameBusinesses() {
    const auto& businesses = game->getPlayer().getBusinesses();

    for (size_t i = 0; i < businesses.size(); ++i) {
        float yPos = 140 + i * 130;
        const auto& b = businesses[i];

        sf::RectangleShape panel(sf::Vector2f(960, 110));
        panel.setPosition(20, yPos - 10);
        panel.setFillColor(COLOR_PANEL);
        panel.setOutlineColor(COLOR_BLACK);
        panel.setOutlineThickness(2);
        window.draw(panel);

        float barX = 160;
        float barY = yPos + 45;
        float barW = 500;
        float barH = 35;

        sf::RectangleShape bar(sf::Vector2f(barW, barH));
        bar.setPosition(barX, barY);
        bar.setFillColor(COLOR_BLACK);
        window.draw(bar);

        if (b->isOwned()) {
            float progress = b->getProgress();
            sf::RectangleShape progressBar(sf::Vector2f(barW * progress, barH));
            progressBar.setPosition(barX, barY);
            progressBar.setFillColor(COLOR_GREEN);
            window.draw(progressBar);

            if (progress >= 1.0f) {
                sf::RectangleShape outline(sf::Vector2f(barW, barH));
                outline.setPosition(barX, barY);
                outline.setFillColor(sf::Color::Transparent);
                outline.setOutlineColor(COLOR_WHITE);
                outline.setOutlineThickness(2);
                window.draw(outline);
            }
        }

        std::string nameLvl = b->getName();
        if (b->isOwned()) nameLvl += "  [Lv " + std::to_string(b->getLevel()) + "]";
        drawText(nameLvl, barX, yPos + 5, 30, COLOR_WHITE);

        if (b->isOwned()) {
            std::string profitStr = "$" + std::to_string((int)b->getProfitPerCycle());
            drawText(profitStr, barX + 10, barY + 5, 24, COLOR_WHITE);

            char timeBuffer[16];
            snprintf(timeBuffer, sizeof(timeBuffer), "%.1fs", b->getProductionTime());
            drawText(timeBuffer, barX + barW - 80, barY + 5, 24, COLOR_WHITE);
        } else {
            drawText("LOCKED", barX + 200, barY + 5, 24, COLOR_RED);
        }
    }
}

void Application::drawGameButtons() {
    for (const auto& btn : gameButtons) {
        drawButton(btn, btn.type == Button::START);
    }
}

void Application::drawGameNotifications() {
    if (!notifications.empty()) {
        const auto& notif = notifications.front();
        int textWidth = getTextWidth(notif.text, 30);
        int boxW = textWidth + 60;
        int boxH = 120;
        int boxX = 500 - boxW / 2;
        int boxY = 375 - boxH / 2;

        sf::RectangleShape box(sf::Vector2f(boxW, boxH));
        box.setPosition(boxX, boxY);
        box.setFillColor(COLOR_BLACK);
        box.setOutlineColor(COLOR_ACCENT);
        box.setOutlineThickness(4);
        window.draw(box);

        if (notif.text.find('\n') != std::string::npos) {
            size_t pos = notif.text.find('\n');
            std::string line1 = notif.text.substr(0, pos);
            std::string line2 = notif.text.substr(pos + 1);

            int w1 = getTextWidth(line1, 30);
            int w2 = getTextWidth(line2, 30);

            drawText(line1, boxX + boxW/2 - w1/2, boxY + 20, 30, COLOR_ACCENT);
            drawText(line2, boxX + boxW/2 - w2/2, boxY + 60, 30, COLOR_WHITE);
        } else {
            drawText(notif.text, boxX + 30, boxY + 45, 30, COLOR_ACCENT);
        }
    }
}

void Application::drawButton(const Button& btn, bool isCircle) {
    sf::FloatRect drawRect = btn.rect;
    if (btn.isPressed) {
        drawRect.left += 2;
        drawRect.top += 2;
        drawRect.width -= 4;
        drawRect.height -= 4;
    }

    if (isCircle) {
        sf::CircleShape circle(drawRect.width / 2);
        circle.setPosition(drawRect.left, drawRect.top);
        circle.setFillColor(btn.color);
        circle.setOutlineColor(COLOR_BLACK);
        circle.setOutlineThickness(2);
        window.draw(circle);

        const char* iconText = "Click";
        if (btn.color == COLOR_GREEN) iconText = "Auto";
        int w = getTextWidth(iconText, 20);
        drawText(iconText, drawRect.left + drawRect.width/2 - w/2, drawRect.top + drawRect.height/2 - 10, 20, COLOR_BLACK);
    } else {
        sf::RectangleShape rect(sf::Vector2f(drawRect.width, drawRect.height));
        rect.setPosition(drawRect.left, drawRect.top);
        rect.setFillColor(btn.color);
        rect.setOutlineColor(COLOR_BLACK);
        rect.setOutlineThickness(2);
        window.draw(rect);

        if (btn.text.find('\n') != std::string::npos) {
            size_t pos = btn.text.find('\n');
            std::string line1 = btn.text.substr(0, pos);
            std::string line2 = btn.text.substr(pos + 1);

            int w1 = getTextWidth(line1, 20);
            int w2 = getTextWidth(line2, 20);

            drawText(line1, drawRect.left + drawRect.width/2 - w1/2, drawRect.top + 10, 20, COLOR_WHITE);
            drawText(line2, drawRect.left + drawRect.width/2 - w2/2, drawRect.top + 35, 20, COLOR_WHITE);
        } else {
            int w = getTextWidth(btn.text, 20);
            drawText(btn.text, drawRect.left + drawRect.width/2 - w/2, drawRect.top + drawRect.height/2 - 10, 20, COLOR_WHITE);
        }
    }
}

void Application::drawText(const std::string& text, float x, float y, int size, sf::Color color) {
    sf::Text sfText;
    sfText.setFont(font);
    sfText.setString(text);
    sfText.setCharacterSize(size);
    sfText.setFillColor(color);
    sfText.setPosition(x, y);
    window.draw(sfText);
}

int Application::getTextWidth(const std::string& text, int size) {
    sf::Text sfText;
    sfText.setFont(font);
    sfText.setString(text);
    sfText.setCharacterSize(size);
    return sfText.getLocalBounds().width;
}