#include "../include/Player.h"
#include <iostream>

Player::Player(const std::string& name, double money)
    : name(name), money(money) {}

const std::string& Player::getName() const {
    return name;
}

double Player::getMoney() const {
    return money;
}

void Player::spendMoney(double amount) {
    if (money >= amount) {
        money -= amount;
    } else {
        std::cout << "Nu ai destui bani!\n";
    }
}

void Player::addMoney(double amount) {
    money += amount;
}

void Player::earnProfit() {
    for (auto& b : businesses) {
        if (b.isOwned()) {
            money += b.getProfitPerCycle();
        }
    }
}

void Player::unlockBusiness(int index) {
    if (index < 0 || index >= static_cast<int>(businesses.size())) {
        std::cout << "Index invalid!\n";
        return;
    }

    businesses[index].unlock(money);
}

std::vector<Business>& Player::accessBusinesses() {
    return businesses;
}

std::vector<Business>& Player::getBusinesses() {
    return businesses;
}

double& Player::accessMoney() {
    return money;
}
const std::vector<Business>& Player::getBusinesses() const {
    return businesses;
}
