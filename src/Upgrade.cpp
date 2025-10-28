#include "../include/Upgrade.h"

Upgrade::Upgrade(const std::string n, double m) : name(n), multiplier(m) {}
Upgrade::~Upgrade() = default;

double Upgrade::getMultiplier() const { return multiplier; }
const std::string& Upgrade::getName() const { return name; }

std::ostream& operator<<(std::ostream& os, const Upgrade& u) {
    os << "[Upgrade] "<< u.getName() << " x" << u.getMultiplier();
    return os;
}