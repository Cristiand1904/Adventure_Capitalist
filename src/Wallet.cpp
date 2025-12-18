#include "../include/Wallet.h"

Wallet::Wallet(double initialMoney) : money(initialMoney) {}

double Wallet::getMoney() const {
    return money;
}

void Wallet::addMoney(double amount) {
    money += amount;
}

void Wallet::spendMoney(double amount) {
    if (money >= amount) {
        money -= amount;
    } else {
        throw InsufficientFundsException(amount, money);
    }
}