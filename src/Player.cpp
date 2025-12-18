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

void Player::earnProfit() {
    const double bonusMultiplier = 1.2;
    double totalProfit = 0;
    for (const auto& b : businesses) {
        if (b->isOwned()) {
            totalProfit += b->calculateRevenue(bonusMultiplier);
        }
    }
    wallet.addMoney(totalProfit);
}

void Player::displayBusinesses() const {
    std::cout << "\n--- LISTA BUSINESS-URI ---\n";
    for (size_t i = 0; i < businesses.size(); ++i) {
        std::cout << "[" << (i + 1) << "] " << *businesses[i] << "\n";
    }
}

void Player::purchaseBusiness(int index) {
    if (index < 0 || index >= businesses.size()) {
        throw InvalidBusinessIndexException(index + 1);
    }
    const auto& business = businesses[index];
    if (business->isOwned()) {
        throw BusinessAlreadyOwnedException(business->getName());
    }
    const double cost = business->getPurchaseCost();
    wallet.spendMoney(cost);
    business->unlock();
}

void Player::upgradeBusiness(int index) {
    if (index < 0 || index >= businesses.size()) {
        throw InvalidBusinessIndexException(index + 1);
    }
    const auto& business = businesses[index];
    if (!business->isOwned()) {
        throw BusinessNotOwnedException(business->getName());
    }
    const double cost = business->getUpgradeCost();
    wallet.spendMoney(cost);
    business->levelUp();
}

void Player::hireManager(int index) {
    if (index < 0 || index >= businesses.size()) {
        throw InvalidBusinessIndexException(index + 1);
    }
    const auto& business = businesses[index];
    if (!business->isOwned()) {
        throw BusinessNotOwnedException(business->getName());
    }
    const double cost = business->getManagerCost();
    wallet.spendMoney(cost);
    business->hireManager();
}