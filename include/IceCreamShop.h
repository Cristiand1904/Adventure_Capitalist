#pragma once
#include "Business.h"

class IceCreamShop : public Business {
protected:
    void print(std::ostream& os) const override;

public:
    IceCreamShop();
    IceCreamShop(const std::string& name, double profit, double upgrade, double cost);

    double calculateRevenue(double bonusMultiplier) const override;
    std::unique_ptr<Business> clone() const override;
};