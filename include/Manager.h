#pragma once
#include <string>

class Manager {
private:
    std::string name;
    double cost;

public:
    Manager(std::string name, double cost);

    const std::string& getName() const;
    double getCost() const;
};