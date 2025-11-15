#include "../include/Achievement.h"
#include <iostream>

bool Achievement::isUnlocked() const {
    return unlocked;
}

const std::string& Achievement::getDescription() const {
    return description;
}

void Achievement::unlock() {
    unlocked = true;
}

std::ostream& operator<<(std::ostream& os, const Achievement& ach) {
    os << "Achievement: " << ach.getDescription();
    os << " [" << (ach.isUnlocked() ? "Unlocked" : "Locked") << "]";

    return os;
}