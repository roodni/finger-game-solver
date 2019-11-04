#pragma once
#include <set>
#include <tuple>
#include <string>

// player
// 0: 先攻
// 1: 後攻

// 必要最小限のゲームの状態を持つ
class GameState {
public:
    GameState(int player, int p1, int p2, int e1, int e2);  // p1,p2がplayerの指本数(順不同)
    int getPlayer() const;
    int getL(int player) const;
    int getR(int player) const;
    std::tuple<int, int, int, int, int> makeTuple() const;    // 状態をコンストラクタと同じ順にタプルにする
private:
    int l[2], r[2]; // 指の本数
    int player;
};

bool operator <(const GameState &l, const GameState &r);

// 自分を攻撃できるルール
enum class RuleAttackSelf {
    allow,  // 自分を攻撃できる
    forbid  // 自分を攻撃できない
};

// オーバーフロー時のルール
enum class RuleOverflow {
    mod,    // modを許可する
    death   // 即死
};

// 分身ルール
enum class RuleBunshin {
    allow,  // 常に許可
    resuscitateOnly,    // 復活のみ可能
    forbid  // 分身禁止
};

// ゲームのルールを持つ
class GameRule {
public:
    GameRule(int max, RuleOverflow overflow, RuleBunshin bunshin, RuleAttackSelf atackSelf);
    int getPlayer(const GameState &state) const;    // その状態を操作可能なプレイヤーを返す
    int getWinner(const GameState &state) const;    // 勝利状態であれば勝者を返し、そうでなければ-1を返す
    void calcTransSet(const GameState &state, std::set<GameState> &set) const;    // 遷移可能な状態の集合を求め、与えられた集合に加える
    std::string getLabel(const GameState &state) const;  // ラベルを吐く
    GameState firstState() const;   // 初期状態を与える
private:
    int max;    // 指の最大本数
    RuleOverflow overflow;
    RuleBunshin bunshin;
    RuleAttackSelf attackSelf;
    int ofAct(int finger) const;  // 指本数オーバーフローを丸める
};