#pragma once
#include "Business.h"

class LemonadeStand : public Business {
protected:
    void print(std::ostream& os) const override;

public:
    LemonadeStand();
    LemonadeStand(const std::string& name, double profit, double upgrade, double cost, double time);

    double calculateRevenue(double bonusMultiplier) const override;
    std::unique_ptr<Business> clone() const override;
};