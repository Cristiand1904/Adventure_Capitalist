#ifndef UPGRADE_H
#define UPGRADE_H

#include <string>
#include <iostream>

class Upgrade {
    std::string name;
    double multiplier;

public:
    Upgrade(const std::string& name, double multiplier);
    double getMultiplier() const;
    const std::string& getName() const;

    friend std::ostream& operator<<(std::ostream& os, const Upgrade& u);
};

#endif
