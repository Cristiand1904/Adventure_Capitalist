#include "../include/LemonadeStand.h"
#include <iostream>

LemonadeStand::LemonadeStand()
    : Business("Limonada", 1, 4, 10) {}

LemonadeStand::LemonadeStand(const std::string& name, double profit, double upgrade, double cost)
    : Business(name, profit, upgrade, cost) {}

double LemonadeStand::calculateRevenue(double bonusMultiplier) const {

    return getProfitPerCycle() * bonusMultiplier * 1.5;
}

std::unique_ptr<Business> LemonadeStand::clone() const {
    return std::make_unique<LemonadeStand>(*this);
}

void LemonadeStand::print(std::ostream& os) const {
    Business::print(os);
    os << " | Tip: Bauturi racoritoare";
}