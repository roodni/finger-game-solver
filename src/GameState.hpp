#pragma once
#include <set>
#include <tuple>
#include <string>

// player
// 0: 先攻
// 1: 後攻

// ゲームの状態とルールを持つ
class GameState {
public:
    GameState(int player, int p1, int p2, int e1, int e2);  // p1,p2がplayerの指本数(順不同)
    int getPlayer() const;    // その状態を操作可能なプレイヤーを返す
    int getWinner() const;    // 勝利状態であれば勝者を返し、そうでなければ-1を返す
    void addTransSet(std::set<GameState> &set) const; // 遷移可能な状態の集合を求め、与えられた集合に加える
    std::tuple<int, int, int, int, int> makeTuple() const;    // 状態をコンストラクタと同じ順にタプルにする
    std::string getLabel() const;  // ラベルを吐く
private:
    int l[2], r[2]; // 指の本数
    int player;
};

bool operator <(const GameState &l, const GameState &r);