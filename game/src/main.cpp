#include "src/Game/Game.hpp"

int main(int, char**) {
    Game game;
    if (game.init() == 1) return 1;
    return game.start();
}