#include "../include/Player.h"
#include "../include/Exceptions.h"
#include <iostream>
#include <iomanip>
#include <utility>

Player::Player(const std::string& name, double money)
    : name(name), wallet(money) {}

Player::Player(const Player& other)
    : name(other.name), wallet(other.wallet) {
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

void Player::update(double deltaTime) {
    double totalProfit = 0;
    for (const auto& b : businesses) {
        if (b->isOwned()) {
            totalProfit += b->update(deltaTime);
        }
    }
    if (totalProfit > 0) {
        wallet.addMoney(totalProfit);
    }
}

void Player::startBusinessProduction(int index) {
    if (index >= 0 && static_cast<size_t>(index) < businesses.size()) {
        businesses[index]->startProduction();
    }
}

void Player::displayBusinesses() const {
    // ... (pastram pentru debug, dar nu e folosit in GUI)
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