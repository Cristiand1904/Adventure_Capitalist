#include "../include/Manager.h"
#include <utility>
#include <cmath>

Manager::Manager(std::string name, double cost)
    : name(std::move(name)), cost(cost), level(1) {}

const std::string& Manager::getName() const {
    return name;
}

double Manager::getCost() const {
    return cost;
}

int Manager::getLevel() const {
    return level;
}

void Manager::upgrade() {
    level++;
    cost *= 3.0; // Costul de upgrade al managerului creste mult
}

double Manager::getDiscountFactor() const {
    // Fiecare nivel reduce costul upgrade-urilor cu 10%
    return std::pow(0.9, level - 1);
}