#ifndef UPGRADE_H
#define UPGRADE_H

#include <string.h>
#include <iostream>

class Upgrade {
private:
    std::string name;
    double multiplier;

public:
    Upgrade(const std::string name, double multiplier);
    ~Upgrade();

    double getMultiplier() const;
    const std::string& getName() const;

    friend std::ostream& operator<<(std::ostream& os, const Upgrade& u);
};

#endif