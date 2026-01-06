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
    Business::display(os); // Apelam display din baza care apeleaza print-ul de baza (care e protected/private)
    // Dar Business::print e protected/private.
    // Business::display e public si apeleaza print.
    // Aici suprascriem print.
    // Putem apela Business::print daca e protected.
    // Dar in Business.h e declarat virtual void print(std::ostream& os) const;
    // Deci putem apela Business::print(os);

    // Nota: In Business.h am facut print virtual protected.
    // Deci:
    // Business::print(os);
    // os << " [Type: " << (int)type << "]";
}