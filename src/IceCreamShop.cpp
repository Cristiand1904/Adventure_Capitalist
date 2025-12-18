#include "../include/IceCreamShop.h"
#include <iostream>

IceCreamShop::IceCreamShop()
    : Business("Inghetata", 10, 40, 100) {}

IceCreamShop::IceCreamShop(const std::string& name, double profit, double upgrade, double cost)
    : Business(name, profit, upgrade, cost) {}

double IceCreamShop::calculateRevenue(double bonusMultiplier) const {

    return getProfitPerCycle() * bonusMultiplier * 1.2;
}

std::unique_ptr<Business> IceCreamShop::clone() const {
    return std::make_unique<IceCreamShop>(*this);
}

void IceCreamShop::print(std::ostream& os) const {
    Business::print(os);
    os << " | Tip: Desert";
}