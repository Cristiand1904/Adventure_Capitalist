#pragma once
#include <string>
#include <iostream>

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

    friend std::ostream& operator<<(std::ostream& os, const Achievement& ach);
};