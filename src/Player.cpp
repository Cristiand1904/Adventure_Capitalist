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
    if (money >= amount) money -= amount;
}

void Player::earnProfit() {
    for (auto& b : businesses) {
        if (b.isOwned()) {
            money += b.getProfitPerCycle();
        }
    }
}

void Player::unlockBusiness(int index) {
    if (index >= 0 && index < (int)businesses.size()) {
        businesses[index].unlock(money);
    } else {
        std::cout << "Index invalid.\n";
    }
}

std::vector<Business>& Player::getBusinesses() {
    return businesses;
}

const std::vector<Business>& Player::getBusinesses() const {
    return businesses;
}

std::vector<Business>& Player::accessBusinesses() {
    return businesses;
}

double& Player::accessMoney() {
    return money;
}
