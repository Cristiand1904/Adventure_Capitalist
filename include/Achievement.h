#ifndef ACHIEVEMENT_H
#define ACHIEVEMENT_H

#include <string>

class Achievement {
private:
    std::string name;
    std::string description;
    bool unlocked;

public:
    Achievement(const std::string& name, const std::string& description);
    void unlock();
    bool isUnlocked() const;
    const std::string& getName() const;
    const std::string& getDescription() const;
};

#endif
