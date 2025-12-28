#include "include/Application.h"
#include <iostream>
#include <limits>

int main() {
    std::cout << "Pornire aplicatie...\n";
    try {
        Application app;
        std::cout << "Aplicatie initializata. Rulare...\n";
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "O eroare critica a oprit aplicatia: " << e.what() << std::endl;
        std::cout << "Apasa Enter pentru a iesi...\n";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        return 1;
    } catch (...) {
        std::cerr << "O eroare necunoscuta a oprit aplicatia.\n";
        std::cout << "Apasa Enter pentru a iesi...\n";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        return 1;
    }
    std::cout << "Aplicatie terminata cu succes.\n";
    return 0;
}