#include "GameSolver.hpp"
#include "GraphVisualizer.hpp"
#include <deque>
#include <array>

namespace {
    char const* const color[2] = {"red", "blue"};
}

void GameSolver::init(const GameState &firstState) {
    // ゲームグラフの全探索
    std::deque<GameState> bfsq{firstState};
    while (!bfsq.empty()) {
        GameState state = bfsq.front();
        bfsq.pop_front();
        loopStates.insert(state);   // 暫定千日手状態とする
        state.addTransSet(transs[state].loop); // 暫定千日手遷移に全ての遷移を追加
        for (const GameState &next : transs[state].loop) {
            transs[next].from.insert(state);
            if (loopStates.find(next) == loopStates.end()) {
                // 初回なら探索する
                bfsq.push_back(next);
            }
        }
    }
}

void GameSolver::getAllGameGraph(std::ostream &out) {
    GraphVisualizer gv;
    std::map<GameState, int> ids;

    for (const auto &p : transs) {
        const GameState &state = p.first;
        const Trans &trans = p.second;

        // 対応するノードがなければ作成
        if (ids.find(state) == ids.end()) ids[state] = gv.createNode(state.getLabel());
        int stateId = ids[state];

        int winner = state.getWinner();
        if (winner == 0 || winner == 1) {
            gv.setNodeOption(stateId, "color", color[winner]);
        }

        std::array<const std::set<GameState> *, 3> transSets{&trans.loop, &trans.win[0], &trans.win[1]};
        for (const std::set<GameState> *transSet : transSets) {
            for (const GameState &next : *transSet) {
                // 遷移先には新規ノードを作成する
                int nextId = gv.createNode(next.getLabel());
                if (ids.find(next) == ids.end()) {
                    // 状態が初回であればノードidを登録
                    ids[next] = nextId;
                } else {
                    // 状態が既出なら薄める
                    gv.setNodeOption(nextId, "color", "gray");
                }
                gv.setTrans(stateId, nextId);
            }
        }
    }

    gv.toDotLang(out);
}