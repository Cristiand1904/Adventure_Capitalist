#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>
#include "Business.h"
#include "Upgrade.h"

class Player {
private:
    std::string name;
    double money;
    std::vector<Business> businesses;
    std::vector<Upgrade> upgrades;

public:
    Player(const std::string& name, double money);
    ~Player();

    bool pay(double amount);

    void buyBusiness(const Business& b, double price);
    double earnCycle(int cycles);
    double getMoney() const;
    const std::vector<Business>& getBusinesses() const;
    std::vector<Business>& accessBusinesses();

    friend std::ostream& operator<<(std::ostream& os, const Player& p);
};

#endif
