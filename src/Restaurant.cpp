#include "../include/Restaurant.h"
#include <iostream>

Restaurant::Restaurant()
    : Business("Restaurant", 100, 400, 1000) {}

Restaurant::Restaurant(const std::string& name, double profit, double upgrade, double cost)
    : Business(name, profit, upgrade, cost) {}

double Restaurant::calculateRevenue(double bonusMultiplier) const {

    return getProfitPerCycle() * (1.0 + (bonusMultiplier - 1.0) * 0.5);
}

std::unique_ptr<Business> Restaurant::clone() const {
    return std::make_unique<Restaurant>(*this);
}

void Restaurant::print(std::ostream& os) const {
    Business::print(os);
    os << " | Tip: Fine Dining";
}