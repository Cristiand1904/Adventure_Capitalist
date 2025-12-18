#include "../include/Manager.h"
#include <utility>

Manager::Manager(std::string name, double cost) : name(std::move(name)), cost(cost) {}

const std::string& Manager::getName() const {
    return name;
}

double Manager::getCost() const {
    return cost;
}