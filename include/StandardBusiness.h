#pragma once
#include "Business.h"

enum class BusinessType {
    LEMONADE,
    ICE_CREAM,
    RESTAURANT
};

class StandardBusiness : public Business {
private:
    BusinessType type;

protected:
    void print(std::ostream& os) const override;

public:
    StandardBusiness(std::string name, BusinessType type, double profit, double upgrade, double cost, double time, double mngCost = 0);

    std::unique_ptr<Business> clone() const override;
    double calculateRevenue(double bonusMultiplier) const override;
};