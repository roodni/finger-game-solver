#include "GameSolver.hpp"
#include "GameState.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <tuple>

void unko(const std::string &str) {
    std::cout << str << std::endl;
}

int main() {
    int max;
    int num;
    RuleOverflow overflow;
    RuleBunshin bunshin;
    RuleAttackSelf attackSelf;
    GraphLoop graphLoop;
    GraphShowLose graphShowLose;

    // 設定を入力
    std::cout
        << "[ルール設定]\n"
        << "* 指の本数の最大数\n"
        << "> ";
    std::cin >> max;
    std::cout
        << "\n"
        << "オーバーフロー時の動作:\n"
        << "0: mod " << (max + 1) << "\n"
        << "1: ゼロ\n"
        << "> ";
    std::cin >> num;
    overflow = static_cast<RuleOverflow>(num);
    std::cout
        << "\n"
        << "* 分身と結合\n"
        << "0: 常に可能\n"
        << "1: 片方が0のときのみ可能\n"
        << "2: 禁止\n"
        << "> ";
    std::cin >> num;
    bunshin = static_cast<RuleBunshin>(num);
    std::cout
        << "\n"
        << "* 自分への攻撃\n"
        << "0: 可能\n"
        << "1: 禁止\n"
        << "> ";
    std::cin >> num;
    attackSelf = static_cast<RuleAttackSelf>(num);

    std::cout
        << "\n"
        << "[描画設定]\n"
        << "* 千日手グラフのループ\n"
        << "0: ループしない\n"
        << "1: ループする(大きな状態遷移図では時間がかかるかもしれません)\n"
        << "> ";
    std::cin >> num;
    graphLoop = static_cast<GraphLoop>(num);
    std::cout
        << "\n"
        << "* 千日手から外れる遷移\n"
        << "0: 描画する\n"
        << "1: 描画しない\n"
        << "> ";
    std::cin >> num;
    graphShowLose = static_cast<GraphShowLose>(num);

    // ファイル名のプレフィックス
    std::stringstream pss;
    pss << max << "_";
    switch (overflow) {
    case RuleOverflow::mod:
        pss << "mod";
        break;
    case RuleOverflow::death:
        pss << "no-mod";
        break;
    }
    pss << "_";
    switch (bunshin) {
    case RuleBunshin::allow:
        pss << "split";
        break;
    case RuleBunshin::resuscitateOnly:
        pss << "resuscitation-only-split";
        break;
    case RuleBunshin::forbid:
        pss << "no-split";
        break;
    }
    pss << "_";
    switch (attackSelf) {
    case RuleAttackSelf::allow:
        pss << "self-attack";
        break;
    case RuleAttackSelf::forbid:
        pss << "no-self-attack";
        break;
    }
    std::string prefix = pss.str();

    std::stringstream sss;
    switch (graphLoop) {
    case GraphLoop::allow:
        sss << "loop";
        break;
    case GraphLoop::forbid:
        sss << "no-loop";
        break;
    }
    sss << "_";
    switch (graphShowLose) {
    case GraphShowLose::show:
        sss << "show-out-trans";
        break;
    case GraphShowLose::hide:
        sss << "hide-out-trans";
        break;
    }
    std::string suffix = sss.str();


    std::cout << "[初期化]" << std::endl;
    GameSolver gameSolver(GameRule(max, overflow, bunshin, attackSelf));
    std::cout << "終了" << std::endl;

    std::ofstream fout;

    std::cout << "[first-win-graph]" << std::endl;
    fout.open("dot/" + prefix + "_first.dot");
    if (fout.fail()) {
        std::cout << "ファイルオープンに失敗しました" << std::endl;
        return 1;
    }
    gameSolver.makeWinGameGraph(0, fout);
    fout.close();

    std::cout << "[second-win-graph]" << std::endl;
    fout.open("dot/" + prefix + "_second.dot");
    if (fout.fail()) {
        std::cout << "ファイルオープンに失敗しました" << std::endl;
        return 1;
    }
    gameSolver.makeWinGameGraph(1, fout);
    fout.close();

    std::cout << "[loop-graph]" << std::endl;
    fout.open("dot/" + prefix + "_loop_" + suffix + ".dot");
    if (fout.fail()) {
        std::cout << "ファイルオープンに失敗しました" << std::endl;
        return 1;
    }
    gameSolver.makeLoopGameGraph(fout, graphLoop, graphShowLose);
    fout.close();

    return 0;
}