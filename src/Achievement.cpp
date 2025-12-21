#include "../include/Achievement.h"
#include <iostream>

Achievement::Achievement(std::string name, std::string desc, double reward, AchievementType type, double target)
    : name(std::move(name)), description(std::move(desc)), unlocked(false),
      type(type), targetValue(target), reward(reward) {}

bool Achievement::isUnlocked() const {
    return unlocked;
}

const std::string& Achievement::getName() const {
    return name;
}

const std::string& Achievement::getDescription() const {
    return description;
}

double Achievement::getReward() const {
    return reward;
}

bool Achievement::checkCondition(double currentMoney, int totalLevels, bool hasManager, bool hasUpgrade) {
    if (unlocked) return false;

    switch (type) {
        case AchievementType::MONEY:
            return currentMoney >= targetValue;
        case AchievementType::TOTAL_LEVELS:
            return totalLevels >= static_cast<int>(targetValue);
        case AchievementType::HAS_MANAGER:
            return hasManager;
        case AchievementType::HAS_UPGRADE:
            return hasUpgrade;
        default:
            return false;
    }
}

void Achievement::unlock() {
    unlocked = true;
}

std::ostream& operator<<(std::ostream& os, const Achievement& ach) {
    os << ach.getName() << " [" << (ach.isUnlocked() ? "Unlocked" : "Locked") << "]";
    return os;
}