#include "../include/Player.h"
#include "../include/Exceptions.h"
#include <iostream>
#include <iomanip>
#include <utility>

Player::Player(const std::string& name, double money)
    : name(name), wallet(money) {
    initAchievements();
}

void Player::initAchievements() {
    // 1. Primul Dolar - 10$
    achievements.emplace_back("Primul Dolar", "Castiga primul tau dolar", 10.0, AchievementType::MONEY, 1.0);

    // 2. Primul Upgrade - 10$
    achievements.emplace_back("Primul Upgrade", "Fa un upgrade la o afacere", 10.0, AchievementType::HAS_UPGRADE, 1.0);

    // 3. Primul Manager - 10$
    achievements.emplace_back("Primul Manager", "Angajeaza un manager", 10.0, AchievementType::HAS_MANAGER, 1.0);

    // 4. 100 Dolari - 100$
    achievements.emplace_back("Suta de Dolari", "Strange 100$", 100.0, AchievementType::MONEY, 100.0);

    // 5. 1.000 Dolari - 500$
    achievements.emplace_back("Mie de Dolari", "Strange 1.000$", 500.0, AchievementType::MONEY, 1000.0);

    // 6. 100.000 Dolari - 50000$
    achievements.emplace_back("Magnat", "Strange 100.000$", 50000.0, AchievementType::MONEY, 100000.0);
}

Player::Player(const Player& other)
    : name(other.name), wallet(other.wallet), achievements(other.achievements) {
    businesses.reserve(other.businesses.size());
    for (const auto& b : other.businesses) {
        businesses.push_back(b->clone());
    }
}

void swap(Player& first, Player& second) noexcept {
    using std::swap;
    swap(first.name, second.name);
    swap(first.wallet, second.wallet);
    swap(first.businesses, second.businesses);
    swap(first.achievements, second.achievements);
}

Player& Player::operator=(Player other) {
    swap(*this, other);
    return *this;
}

void Player::addBusiness(std::unique_ptr<Business> business) {
    businesses.push_back(std::move(business));
}

const std::string& Player::getName() const { return name; }
double Player::getMoney() const { return wallet.getMoney(); }
const std::vector<std::unique_ptr<Business>>& Player::getBusinesses() const { return businesses; }
const std::vector<Achievement>& Player::getAchievements() const { return achievements; }

void Player::setMoney(double m) {
    wallet = Wallet(m);
}

void Player::unlockAchievement(const std::string& achievementName) {
    for (auto& ach : achievements) {
        if (ach.getName() == achievementName) {
            ach.unlock();
            break;
        }
    }
}

std::vector<std::string> Player::update(double deltaTime) {
    double totalProfit = 0;
    for (const auto& b : businesses) {
        if (b->isOwned()) {
            totalProfit += b->update(deltaTime);
        }
    }
    if (totalProfit > 0) {
        wallet.addMoney(totalProfit);
    }
    return checkAchievements();
}

std::vector<std::string> Player::checkAchievements() {
    std::vector<std::string> unlockedMessages;

    int totalLevels = 0;
    bool hasManager = false;
    bool hasUpgrade = false;

    for (const auto& b : businesses) {
        if (b->isOwned()) {
            totalLevels += b->getLevel();
            if (b->hasManagerHired()) hasManager = true;
            if (b->getLevel() > 1) hasUpgrade = true;
        }
    }

    for (auto& ach : achievements) {
        if (ach.checkCondition(wallet.getMoney(), totalLevels, hasManager, hasUpgrade)) {
            ach.unlock();
            wallet.addMoney(ach.getReward());
            unlockedMessages.push_back("ACHIEVEMENT: " + ach.getName() + "\nReward: $" + std::to_string((int)ach.getReward()));
        }
    }
    return unlockedMessages;
}

void Player::startBusinessProduction(int index) {
    if (index >= 0 && static_cast<size_t>(index) < businesses.size()) {
        businesses[index]->startProduction();
    }
}

void Player::displayBusinesses() const {
    // ...
}

void Player::purchaseBusiness(int index_int) {
    size_t index = static_cast<size_t>(index_int);
    if (index >= businesses.size()) {
        throw InvalidBusinessIndexException(index_int + 1);
    }
    const auto& business = businesses[index];
    if (business->isOwned()) {
        throw BusinessAlreadyOwnedException(business->getName());
    }
    const double cost = business->getPurchaseCost();
    wallet.spendMoney(cost);
    business->unlock();
}

void Player::upgradeBusiness(int index_int) {
    size_t index = static_cast<size_t>(index_int);
    if (index >= businesses.size()) {
        throw InvalidBusinessIndexException(index_int + 1);
    }
    const auto& business = businesses[index];
    if (!business->isOwned()) {
        throw BusinessNotOwnedException(business->getName());
    }
    const double cost = business->getUpgradeCost();
    wallet.spendMoney(cost);
    business->levelUp();
}

void Player::hireManager(int index_int) {
    size_t index = static_cast<size_t>(index_int);
    if (index >= businesses.size()) {
        throw InvalidBusinessIndexException(index_int + 1);
    }
    const auto& business = businesses[index];
    if (!business->isOwned()) {
        throw BusinessNotOwnedException(business->getName());
    }
    const double cost = business->getManagerCost();
    wallet.spendMoney(cost);
    business->hireManager();
}

void Player::upgradeManager(int index_int) {
    size_t index = static_cast<size_t>(index_int);
    if (index >= businesses.size()) {
        throw InvalidBusinessIndexException(index_int + 1);
    }
    const auto& business = businesses[index];
    if (!business->hasManagerHired()) {
        throw BusinessNotOwnedException(business->getName() + " Manager");
    }
    const double cost = business->getManagerUpgradeCost();
    wallet.spendMoney(cost);
    business->upgradeManager();
}