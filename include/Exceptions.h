#pragma once
#include <exception>
#include <string>

class GameException : public std::exception {
protected:
    std::string message;

public:
    explicit GameException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class InsufficientFundsException : public GameException {
public:
    InsufficientFundsException(double required, double available)
        : GameException("Fonduri insuficiente! Necesar: " + std::to_string(required) +
                        "$, Disponibil: " + std::to_string(available) + "$") {}
};

class InvalidBusinessIndexException : public GameException {
public:
    explicit InvalidBusinessIndexException(int index)
        : GameException("Index business invalid: " + std::to_string(index)) {}
};

class BusinessAlreadyOwnedException : public GameException {
public:
    explicit BusinessAlreadyOwnedException(const std::string& name)
        : GameException("Business-ul '" + name + "' este deja detinut!") {}
};

class BusinessNotOwnedException : public GameException {
public:
    explicit BusinessNotOwnedException(const std::string& name)
        : GameException("Trebuie sa detii business-ul '" + name + "' pentru a efectua aceasta actiune!") {}
};