#pragma once
#include <string>
#include <iostream>

enum class AchievementType {
    MONEY,
    TOTAL_LEVELS,
    HAS_MANAGER,
    HAS_UPGRADE
};

class Achievement {
private:
    std::string name;
    std::string description;
    bool unlocked;

    AchievementType type;
    double targetValue;
    double reward;

public:
    Achievement(std::string name, std::string desc, double reward, AchievementType type, double target);

    bool isUnlocked() const;
    const std::string& getName() const;
    const std::string& getDescription() const;
    double getReward() const;

    bool checkCondition(double currentMoney, int totalLevels, bool hasManager, bool hasUpgrade);
    void unlock();

    friend std::ostream& operator<<(std::ostream& os, const Achievement& ach);
};