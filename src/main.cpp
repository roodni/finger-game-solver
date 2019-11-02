#include "GameSolver.hpp"
#include "GameState.hpp"
#include <iostream>
#include <fstream>
#include <tuple>

void unko(const std::string &str) {
    std::cout << str << std::endl;
}

int main() {
    std::cerr << "[init]" << std::endl;
    GameSolver gameSolver(GameState(0, 1, 1, 1, 1));

    std::ofstream fout;

    std::cerr << "[first-win-graph]" << std::endl;
    fout.open("dot/first.dot");
    if (fout.fail()) return 1;
    gameSolver.getWinGameGraph(0, fout);
    fout.close();

    std::cerr << "[second-win-graph]" << std::endl;
    fout.open("dot/second.dot");
    if (fout.fail()) return 1;
    gameSolver.getWinGameGraph(1, fout);
    fout.close();

    std::cerr << "[all-graph]" << std::endl;
    fout.open("dot/all.dot");
    if (fout.fail()) return 1;
    gameSolver.getAllGameGraph(fout);
    fout.close();

    return 0;
}