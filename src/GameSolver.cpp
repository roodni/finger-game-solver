#include "GameSolver.hpp"
#include "GraphVisualizer.hpp"
#include <deque>
#include <array>
#include <iostream>

namespace {
    char const* const color[2] = {"red", "blue"};
    char const* const fillcolor[2] = {"pink", "skyblue"};
    char const* const dupe = "gray";
}

GameSolver::GameSolver(const GameState &firstState) : firstState(firstState) {
    // ゲームグラフの全探索
    std::deque<GameState> bfsq{firstState};
    while (!bfsq.empty()) {
        GameState state = bfsq.front();
        bfsq.pop_front();
        state.addTransSet(transs[state].loop); // 暫定千日手遷移に全ての遷移を追加

        for (const GameState &next : transs[state].loop) {
            if (transs.find(next) == transs.end()) {
                // 初回なら探索する
                bfsq.push_back(next);
            }
            transs[next].from.insert(state);
        }
    }
}

void GameSolver::getAllGameGraph(std::ostream &out) {
    GraphVisualizer gv;
    std::map<GameState, int> ids;

    ids[firstState] = gv.createNode(firstState.getLabel());

    std::deque<GameState> sq{firstState};
    while (!sq.empty()) {
        // 深さ優先探索
        GameState state = sq.back();
        sq.pop_back();

        const Trans &trans = transs[state]; // 状態遷移の取得
        int stateId = ids[state];   // ノードidの取得

        // 各遷移を見る
        std::array<const std::set<GameState> *, 3> transSets{&trans.loop, &trans.win[0], &trans.win[1]};
        // std::fprintf(stderr, "%d %d %d\n", trans.loop.size(), trans.win[0].size(), trans.win[1].size());
        for (const std::set<GameState> *transSet : transSets) {
            for (const GameState &next : *transSet) {
                // 遷移先には新規ノード作成
                int nextId = gv.createNode(next.getLabel());
                int winner = next.getWinner();

                if (ids.find(next) == ids.end()) {
                    // 状態が初回であればノードidを登録し探索
                    ids[next] = nextId;
                    sq.push_back(next);
                } else {
                    // 状態が既出かつ、勝利状態でなければ色を薄くする
                    if (winner != 0 && winner != 1) {
                        gv.setNodeOption(nextId, "color", dupe);
                    }
                }
                // 勝利状態であれば色をつける
                if (winner == 0 || winner == 1) {
                    gv.setNodeOption(nextId, "style", "filled");
                    gv.setNodeOption(nextId, "fillcolor", fillcolor[winner]);
                }

                gv.setTrans(stateId, nextId);
            }
        }
    }

    gv.toDotLang(out);
}