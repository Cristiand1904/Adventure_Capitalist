#include "../include/Achievement.h"

bool Achievement::isUnlocked() const {
    return unlocked;
}

std::string Achievement::getDescription() const {
    return description;
}

void Achievement::unlock() {
    unlocked = true;
}
