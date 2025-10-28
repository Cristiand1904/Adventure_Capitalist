#include "../include/Player.h"

Player::Player(const std::string& n, double initialMoney)
    : name(n), money(initialMoney) {}

const std::string& Player::getName() const { return name; }
double Player::getMoney() const { return money; }
std::vector<Business>& Player::getBusinesses() { return businesses; }
const std::vector<Business>& Player::getBusinesses() const { return businesses; }

void Player::addBusiness(const Business& b) {
    businesses.push_back(b);
}

void Player::unlockBusiness(int index) {
    if (index >= 0 && index < (int)businesses.size() && !businesses[index].isOwned()) {
        double cost = businesses[index].getPurchaseCost();
        if (money >= cost) {
            money -= cost;
            businesses[index].unlock(money);
            std::cout << "Ai cumparat " << businesses[index].getName() << "!\n";
        } else {
            std::cout << "Nu ai destui bani pentru a cumpara " << businesses[index].getName() << ".\n";
        }
    } else {
        std::cout << "Business invalid sau deja cumparat.\n";
    }
}

void Player::earnCycle() {
    for (auto& b : businesses) {
        if (b.isOwned()) {
            money += b.getProfitPerCycle();
        }
    }
}

bool Player::pay(double amount) {
    if (amount <= money) {
        money -= amount;
        return true;
    }
    return false;
}

void Player::printBusinesses() const {
    std::cout << "\n[Jucator] " << name << " | Bani: " << static_cast<int>(money) << "$\n";
    std::cout << "Lista business-uri:\n";
    for (const auto& b : businesses) {
        std::cout << "  " << b << "\n";
    }
}
