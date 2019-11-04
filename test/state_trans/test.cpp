#include "../../src/GameState.hpp"
#include <iostream>

// case.txtの形式
/*
max overflow bunshin attackself  <- ルール
p l0 r0 l1 r1   <- 状態
p_ l0_1 r0_1 l1_1 r1_1  <- 遷移先の状態を列挙
p_ l0_2 r0_2 l1_2 r1_2
...
-1   <- 遷移先の終端
...
*/

void outState(const GameState &state) {
    std::clog
        << state.getPlayer()
        << state.getL(0)
        << state.getR(0)
        << state.getL(1)
        << state.getR(1) << std::endl;
}

bool check(int max) {
    RuleOverflow overflow;
    RuleBunshin bunshin;
    RuleAttackSelf attackSelf;

    // ルールの読み込み
    std::string str;
    std::cin >> str;
    if (str == "mod") {
        overflow = RuleOverflow::mod;
    } else if (str == "death") {
        overflow = RuleOverflow::death;
    } else {
        std::clog << "invalid modRule" << std::endl;
        return false;
    }
    std::cin >> str;
    if (str == "allow") {
        bunshin = RuleBunshin::allow;
    } else if (str == "ronly") {
        bunshin = RuleBunshin::resuscitateOnly;
    } else if (str == "forbid") {
        bunshin = RuleBunshin::forbid;
    } else {
        std::clog << "invalid bunshinRule" << std::endl;
        return false;
    }
    std::cin >> str;
    if (str == "allow") {
        attackSelf = RuleAttackSelf::allow;
    } else if (str == "forbid") {
        attackSelf = RuleAttackSelf::forbid;
    } else {
        std::clog << "invalid attackSelfRule" << std::endl;
        return false;
    }

    // 元状態の読み込み
    int p, l[2], r[2];
    std::cin >> p >> l[0] >> r[0] >> l[1] >> r[1];
    assert(p == 0 || p == 1);

    GameState state(p, l[p], r[p], l[1 - p], r[1 - p]);

    std::clog << "State: ";
    outState(state);

    // 遷移先の列挙
    GameRule rule(max, overflow, bunshin, attackSelf);
    std::set<GameState> nexts;
    rule.calcTransSet(state, nexts);
    std::clog << "[Trans]" << std::endl;
    for (const GameState &next : nexts) {
        outState(next);
    }

    // テストケースと遷移先が合致するか検査
    // std::clog << "[Tests]" << std::endl;
    int transCount = 0;
    std::set<GameState> tested;
    int np, nl[2], nr[2];
    while (std::cin >> np, np >= 0) {
        assert(np == 0 || np == 1);
        transCount++;
        std::cin >> nl[0] >> nr[0] >> nl[1] >> nr[1];
        GameState next(np, nl[np], nr[np], nl[1 - np], nr[1 - np]);
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
    int i = 1;
    int max;
    while (std::cin >> max) {
        std::clog << "[" << i << "]" << std::endl;
        if (check(max)) {
            std::clog << "OK" << std::endl;
        } else {
            std::clog << "NG" << std::endl;
            std::cout << "NG" << std::endl;
            return 0;
        }
        i++;
    }

    std::cout << "OK" << std::endl;
    return 0;
}