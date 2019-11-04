#pragma once
#include "GameState.hpp"
#include <map>
#include <set>
#include <ostream>


enum class GraphLoop {
    forbid, // ループを作らない
    allow   // ループを許可する
};


namespace {
    // 各状態からの遷移を分類して格納するデータ構造
    struct Trans {
        std::set<GameState> from;   // その状態に至る状態
        std::set<GameState> loop;   // 千日手となる遷移
        std::set<GameState> win[2]; // 勝利確定遷移
    };
}

class GameSolver {
public:
    GameSolver(GameRule rule);
    void makeWinGameGraph(int player, std::ostream &out);    // playerの勝利するゲームグラフを描画する
    void makeLoopGameGraph(std::ostream &out, GraphLoop loopMode = GraphLoop::forbid);   // 千日手となるゲームグラフを描画する
private:
    GameRule rule;
    void calcWinTrans(int player);   // playerの勝利する状態遷移を計算する
    bool isWinTransCalced[2];   // 勝利確定遷移が計算されているか
    std::map<GameState, Trans> transs;
    std::set<GameState> winStates[2];   // 勝利確定状態の集合
};