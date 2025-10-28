#ifndef MANAGER_H
#define MANAGER_H

#include <string>
#include <iostream>

class Manager {
    std::string name;
    double cost;
    bool hired;

public:
    Manager(const std::string& n, double c)
        : name(n), cost(c), hired(false) {}

    void hire() { hired = true; }
    bool isHired() const { return hired; }
    double getCost() const { return cost; }
    const std::string& getName() const { return name; }

    friend std::ostream& operator<<(std::ostream& os, const Manager& m) {
        os << (m.hired ? "[Angajat] " : "[Disponibil] ")
           << m.name << " - cost: " << m.cost << "$";
        return os;
    }
};
#endif