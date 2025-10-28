#include "../include/Player.h"
#include <iostream>
#include <cmath>

Player::Player(const std::string& name, double money)
    : name(name), money(money) {}

Player::~Player() = default;

bool Player::pay(double amount) {
    if (amount <= money) {
        money -= amount;
        return true;
    }
    return false;
}

void Player::buyBusiness(const Business& b, double price) {
    if (money >= price) {
        money -= price;
        businesses.push_back(b);
    }
}

double Player::earnCycle(int cycles) {
    double base = 0;
    for (const auto& b : businesses) {
        base += b.projectedProfit() * cycles;
    }
    money += base;
    return base;
}

double Player::getMoney() const { return money; }

const std::vector<Business>& Player::getBusinesses() const { return businesses; }

std::vector<Business>& Player::accessBusinesses() { return businesses; }

std::ostream& operator<<(std::ostream& os, const Player& p) {
    os << "[Player] " << p.name << " | money=" << static_cast<int>(p.money) << "\n";
    os << "  Businesses:\n";
    for (const auto& b : p.businesses)
        os << "    - [Business] " << b.getName()
           << " | lvl=" << b.getLevel()
           << " | profit/ciclu=" << static_cast<int>(b.getProfitPerCycle())
           << "\n";

    os << "  Upgrades:\n";
    for (const auto& b : p.businesses) {
        double cost = b.getUpgradeBaseCost();
        // crește cu 50% pentru fiecare nivel
        double totalCost = cost * (1 + 0.5 * (b.getLevel() - 1));
        os << "    * " << b.getName()
           << " - upgrade cost curent: " << static_cast<int>(std::round(totalCost)) << "$\n";
    }

    return os;
}
