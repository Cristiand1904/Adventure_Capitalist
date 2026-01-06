#ifdef USE_HEADLESS_APP
#include "include/ApplicationHeadless.h"
#else
#include "include/Application.h"
#endif

#include <iostream>

int main() {
    std::cout << "Pornire aplicatie...\n";

    Application app;
    std::cout << "Aplicatie initializata. Rulare...\n";
    app.run();

    std::cout << "Aplicatie terminata cu succes.\n";
    return 0;
}