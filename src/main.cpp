#include "GameSolver.hpp"
#include "GameState.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>

void fileOutput(std::string fileName, std::function<void(std::ostream &)> toStream);

int main() {
    int max;
    int num;
    RuleOverflow overflow;
    RuleBunshin bunshin;
    RuleAttackSelf attackSelf;
    GraphLoop graphLoop;
    GraphShowLose graphShowLose;

    std::string prefix;  // ファイル名のプレフィックス
    std::string suffix;  // ファイル名のサフィックス

    // ルール設定を入力
    std::cerr
        << "[ルール設定]\n"
        << "* 指の本数の最大数\n";
    do {
        std::cerr << "> ";
        std::cin >> max;
    } while (max <= 0);
    prefix += std::to_string(max);
    prefix += "_";

    std::cerr
        << "\n"
        << "* 指の本数のオーバーフロー\n"
        << "0: 0\n"
        << "1: mod " << (max + 1) << "\n";
    do {
        std::cerr << "> ";
        std::cin >> num;
        switch (num) {
        case 0:
            prefix += "no-mod";
            overflow = RuleOverflow::death;
            break;
        case 1:
            prefix += "mod";
            overflow = RuleOverflow::mod;
            break;
        default:
            num = -1;
            break;
        }
    } while (num == -1);
    prefix += "_";

    std::cerr
        << "\n"
        << "* 分身と結合\n"
        << "0: 禁止\n"
        << "1: 常に可能\n"
        << "2: 片方が0のときのみ可能\n";
    do {
        std::cerr << "> ";
        std::cin >> num;
        switch (num) {
        case 0:
            prefix += "no-split";
            bunshin = RuleBunshin::forbid;
            break;
        case 1:
            prefix += "split";
            bunshin = RuleBunshin::allow;
            break;
        case 2:
            prefix += "resuscitation-only-split";
            bunshin = RuleBunshin::resuscitateOnly;
            break;
        default:
            num = -1;
            break;
        }
    } while (num == -1);
    prefix += "_";

    std::cerr
        << "\n"
        << "* 自分への攻撃\n"
        << "0: 禁止\n"
        << "1: 可能\n";
    do {
        std::cerr << "> ";
        std::cin >> num;
        switch (num) {
        case 0:
            prefix += "no-self-attack";
            attackSelf = RuleAttackSelf::forbid;
            break;
        case 1:
            prefix += "self-attack";
            attackSelf = RuleAttackSelf::allow;
            break;
        default:
            num = -1;
            break;
        }
    } while (num == -1);


    // 描画設定を入力
    std::cerr
        << "\n"
        << "[描画設定]\n"
        << "* 千日手グラフのループ\n"
        << "0: ループしない\n"
        << "1: ループする (大きな状態遷移図では時間がかかるかもしれません)\n";

    do {
        std::cerr << "> ";
        std::cin >> num;
        switch (num) {
        case 0:
            suffix += "no-loop";
            graphLoop = GraphLoop::forbid;
            break;
        case 1:
            suffix += "loop";
            graphLoop = GraphLoop::allow;
            break;
        default:
            num = -1;
            break;
        }
    } while (num == -1);
    suffix += "_";

    std::cerr
        << "\n"
        << "* 千日手から外れる遷移\n"
        << "0: 描画しない\n"
        << "1: 描画する\n";
    do {
        std::cerr << "> ";
        std::cin >> num;
        switch (num) {
        case 0:
            suffix += "hide-out-trans";
            graphShowLose = GraphShowLose::hide;
            break;
        case 1:
            suffix += "show-out-trans";
            graphShowLose = GraphShowLose::show;
            break;
        default:
            num = -1;
        }
    } while (num == -1);

    std::cerr << "[ゲーム探索]" << std::endl;
    GameSolver gameSolver(GameRule(max, overflow, bunshin, attackSelf));
    std::cerr << "完了" << std::endl;

    std::cerr << "[グラフ描画]" << std::endl;

    std::string directory = "./dot/";

    fileOutput(directory + prefix + "_first.dot", [&](std::ostream &out){gameSolver.makeWinGameGraph(0, out);});
    fileOutput(directory + prefix + "_second.dot", [&](std::ostream &out){gameSolver.makeWinGameGraph(1, out);});
    fileOutput(directory + prefix + "_draw_" + suffix + ".dot", [&](std::ostream &out){gameSolver.makeLoopGameGraph(out, graphLoop, graphShowLose);});

    return 0;
}


void fileOutput(std::string fileName, std::function<void(std::ostream &)> toStream) {
    std::ofstream fout;
    fout.open(fileName);
    if (fout.fail()) {
        std::cerr << "ファイループンに失敗しました: " << fileName << std::endl;
    } else {
        toStream(fout);
        fout.close();
        std::cout << fileName << std::endl;
    }
}