#include "../include/Achievement.h"

Achievement::Achievement(const std::string& n, const std::string& d)
    : name(n), description(d), unlocked(false) {}

void Achievement::unlock() {
    unlocked = true;
}

bool Achievement::isUnlocked() const {
    return unlocked;
}

const std::string& Achievement::getName() const {
    return name;
}

const std::string& Achievement::getDescription() const {
    return description;
}
