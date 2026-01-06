#include "../include/StandardBusiness.h"
#include <iostream>

StandardBusiness::StandardBusiness(std::string name, BusinessType type, double profit, double upgrade, double cost, double time, double mngCost)
    : Business(std::move(name), profit, upgrade, cost, time, mngCost), type(type) {}

std::unique_ptr<Business> StandardBusiness::clone() const {
    return std::make_unique<StandardBusiness>(*this);
}

double StandardBusiness::calculateRevenue(double bonusMultiplier) const {
    double baseRevenue = getProfitPerCycle() * bonusMultiplier;

    switch (type) {
        case BusinessType::LEMONADE:
            return baseRevenue * 1.5;
        case BusinessType::ICE_CREAM:
            return baseRevenue * 1.2;
        case BusinessType::RESTAURANT:
            return getProfitPerCycle() * (1.0 + (bonusMultiplier - 1.0) * 0.5);
        default:
            return baseRevenue;
    }
}

void StandardBusiness::print(std::ostream& os) const {
    Business::display(os);
}