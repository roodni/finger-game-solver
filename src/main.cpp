#include "GameSolver.hpp"
#include "GameState.hpp"
#include <iostream>
#include <tuple>

void unko(const std::string &str) {
    std::cout << str << std::endl;
}

int main() {
    GameSolver gameSolver;
    gameSolver.init(GameState(0, 1, 1, 1, 1));

    gameSolver.getAllGameGraph(std::cout);

    return 0;
}