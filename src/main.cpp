#include "Game.h"

#include <chrono>
#include <cstdint>
#include <iostream>

int main() {
    const auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    Game game(static_cast<std::uint32_t>(now));

    std::cout << "\nКоманды: w/a/s/d — идти, x — магия, h — кекс, n — новый забег, q — выход.\n";

    char command = 0;
    while (game.isRunning() && std::cin >> command) {
        switch (command) {
            case 'w': game.move(0, 1); break;
            case 's': game.move(0, -1); break;
            case 'a': game.move(-1, 0); break;
            case 'd': game.move(1, 0); break;
            case 'x': game.useMagic(); break;
            case 'h': game.eatCupcake(); break;
            case 'n': game.newRun(); break;
            case 'q': return 0;
            default: std::cout << "Неизвестная команда.\n"; break;
        }
    }
    return 0;
}
