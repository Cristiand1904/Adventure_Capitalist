#include "include/Game.h"
#include <iostream>

int main() {
    try {
        Game game("Capitalist", 50.0);
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "O eroare critica a oprit jocul: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}