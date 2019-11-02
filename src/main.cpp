#include "GameSolver.hpp"
#include "GameState.hpp"
#include <iostream>
#include <tuple>

void unko(const std::string &str) {
    std::cout << str << std::endl;
}

int main() {
    GameSolver gameSolver;
    std::cerr << "[init]" << std::endl;
    gameSolver.init(GameState(0, 1, 1, 1, 1));
    std::cerr << "[graph]" << std::endl;
    gameSolver.getAllGameGraph(std::cout);

    return 0;
}