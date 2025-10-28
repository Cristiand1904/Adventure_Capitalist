#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include "Business.h"

class Player {
    std::string name;
    double money;
    std::vector<Business> businesses;

public:
    Player(const std::string& name, double money);
    void addBusiness(const Business& b);
    void earnCycle(int cycles);
    bool pay(double amount);
    const std::vector<Business>& getBusinesses() const;
    std::vector<Business>& getBusinesses(); // suprasarcină non-const

    friend std::ostream& operator<<(std::ostream& os, const Player& p);
};

#endif
