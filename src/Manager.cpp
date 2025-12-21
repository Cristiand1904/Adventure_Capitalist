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
    cost *= 5.0; // Costul de upgrade al managerului creste de 5x
}

double Manager::getDiscountFactor() const {
    // Fiecare nivel reduce costul upgrade-urilor cu 2%
    return std::pow(0.98, level - 1);
}