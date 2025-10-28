#include "../include/Player.h"
#include <iostream>

Player::Player(const std::string& n, double m) : name(n), money(m) {}

void Player::addBusiness(const Business& b) {
    businesses.push_back(b);
}

void Player::earnCycle(int cycles) {
    double total = 0;
    for (const auto& b : businesses)
        total += b.getProfitPerCycle() * cycles;
    money += total;
}

bool Player::pay(double amount) {
    if (amount <= money) {
        money -= amount;
        return true;
    }
    return false;
}

const std::vector<Business>& Player::getBusinesses() const {
    return businesses;
}

std::vector<Business>& Player::getBusinesses() {
    return businesses;
}

std::ostream& operator<<(std::ostream& os, const Player& p) {
    os << "[Player] " << p.name << " | money=" << (int)p.money << "\n";
    os << "  Businesses:\n";
    for (const auto& b : p.businesses)
        os << "   - " << b << "\n";
    return os;
}
