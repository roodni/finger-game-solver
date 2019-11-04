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
    GameSolver gameSolver(GameRule(4, RuleOverflow::mod, RuleBunshin::allow, RuleAtackSelf::allow));

    std::ofstream fout;

    std::cerr << "[first-win-graph]" << std::endl;
    fout.open("dot/first.dot");
    if (fout.fail()) return 1;
    gameSolver.makeWinGameGraph(0, fout);
    fout.close();

    std::cerr << "[second-win-graph]" << std::endl;
    fout.open("dot/second.dot");
    if (fout.fail()) return 1;
    gameSolver.makeWinGameGraph(1, fout);
    fout.close();

    std::cerr << "[loop-graph]" << std::endl;
    fout.open("dot/loop.dot");
    if (fout.fail()) return 1;
    gameSolver.makeLoopGameGraph(fout, GraphLoop::forbid, GraphShowLose::show);
    fout.close();

    return 0;
}