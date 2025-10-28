#pragma once
#include <string>

class Achievement {
private:
    std::string description;
    bool unlocked;

public:
    explicit Achievement(const std::string& desc = "")
        : description(desc), unlocked(false) {}

    bool isUnlocked() const;
    const std::string& getDescription() const;
    void unlock();
};
