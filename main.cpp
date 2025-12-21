#include "include/Application.h"
#include <iostream>

int main() {
    try {
        Application app;
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "O eroare critica a oprit aplicatia: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}