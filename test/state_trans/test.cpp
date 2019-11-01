#include "../../src/GameState.hpp"
#include <iostream>

// case.txtの形式
/*
n    <- テストケースの個数
p p1 p2 e1 e2    <- 状態
p_1 p1_1 np2_1 ne1_1 ne2_1   <- 遷移先の状態を列挙
p_2 p1_2 np2_2 ne1_2 ne2_2
...
-1   <- 終端
...
*/

void outTuple(std::tuple<int, int, int, int, int> tp) {
    std::clog
        << std::get<0>(tp)
        << std::get<1>(tp)
        << std::get<2>(tp)
        << std::get<3>(tp)
        << std::get<4>(tp) << std::endl;
}

bool check() {
    int p, p1, p2, e1, e2;
    std::set<GameState> nexts;

    std::cin >> p >> p1 >> p2 >> e1 >> e2;
    GameState state(p, p1, p2, e1, e2);
    std::clog << "State: ";
    outTuple(state.makeTuple());

    // 遷移先の列挙
    state.addTransSet(nexts);
    std::clog << "[Trans]" << std::endl;
    for (auto it = nexts.begin(); it != nexts.end(); it++) {
        outTuple(it->makeTuple());
    }

    // テストケースと遷移先が合致するか検査
    // std::clog << "[Tests]" << std::endl;
    int transCount = 0;
    std::set<GameState> tested;
    int np, np1, np2, ne1, ne2;
    while (std::cin >> np, np >= 0) {
        transCount++;
        std::cin >> np1 >> np2 >> ne1 >> ne2;
        GameState next(np, np1, np2, ne1, ne2);
        // outTuple(next.makeTuple());
        // テストケースの重複を検査
        if (tested.find(next) != tested.end()) {
            std::clog << "Testcase duplicate" << std::endl;
            return false;
        }
        // 遷移先の不足を検査
        tested.insert(next);
        if (nexts.find(next) == nexts.end()) {
            std::clog << "State transition lack" << std::endl;
            return false;
        }
    }
    if (nexts.size() > transCount) {
        std::clog << "Illegal state transition" << std::endl;
        return false;
    }
    return true;
}

int main(void) {
    int n;
    std::cin >> n;
    int ok = 0;

    for (int i = 1; i <= n; i++) {
        std::clog << "[" << i << "]" << std::endl;
        if (check()) {
            std::clog << "OK" << std::endl;
            ok++;
        } else {
            std::clog << "NG" << std::endl;
            std::cout << "NG" << std::endl;
            return 0;
        }
    }

    std::cout << "OK" << std::endl;
    return 0;
}