#include "../include/Player.h"
#include <iostream>

Player::Player(const std::string& n, double m) : name(n), money(m) {}

const std::string& Player::getName() const { return name; }
double Player::getMoney() const { return money; }
void Player::addMoney(double amount) { money += amount; }
void Player::spendMoney(double amount) { money -= amount; }
double& Player::accessMoney() { return money; }

std::vector<Business>& Player::getBusinesses() { return businesses; }
const std::vector<Business>& Player::getBusinesses() const { return businesses; }

void Player::earnProfit() {
    for (auto& b : businesses) {
        if (b.isOwned())
            money += b.getProfitPerCycle();
    }
}

void Player::unlockBusiness(int index) {
    if (index < 0 || index >= (int)businesses.size()) {
        std::cout << "Index invalid.\n";
        return;
    }

    Business& b = businesses[index];
    if (b.isOwned()) {
        std::cout << "Business-ul este deja detinut.\n";
        return;
    }

    double cost = b.getPurchaseCost();
    if (money >= cost) {
        b.unlock(money);
        std::cout << "Ai cumparat " << b.getName() << " pentru " << (int)cost << "$.\n";
    } else {
        std::cout << "Nu ai destui bani pentru " << b.getName() << ".\n";
    }
}
