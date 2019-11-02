#include "GameSolver.hpp"
#include "GraphVisualizer.hpp"
#include <deque>
#include <array>
#include <cassert>

namespace {
    char const* const color[2] = {"red", "blue"};
    char const* const fillcolor[2] = {"pink", "skyblue"};
    char const* const dupe = "gray";
}

GameSolver::GameSolver(const GameState &firstState) : firstState(firstState) {
    isWinTransCalced[0] = false;
    isWinTransCalced[1] = false;

    // ゲームグラフの全探索
    std::deque<GameState> bfsq{firstState};
    while (!bfsq.empty()) {
        GameState state = bfsq.front();
        bfsq.pop_front();

        // 勝利状態を勝利確定状態の集合に入れる
        int winner = state.getWinner();
        if (winner == 0 || winner == 1) {
            winStates[winner].insert(state);
        }

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
    std::map<GameState, int> ids;   // 状態とグラフのノードidを対応づける

    ids[firstState] = gv.createNode(firstState.getLabel());

    std::deque<GameState> sq{firstState};
    while (!sq.empty()) {
        // 深さ優先探索
        GameState state = sq.back();
        sq.pop_back();

        Trans &trans = transs[state]; // 状態遷移の取得
        int stateId = ids[state];   // ノードidの取得

        // 各遷移を見る
        // std::fprintf(stderr, "%d %d %d\n", trans.loop.size(), trans.win[0].size(), trans.win[1].size());
        std::array<std::set<GameState> *, 3> transSets{&trans.loop, &trans.win[0], &trans.win[1]};
        for (std::set<GameState> *transSet : transSets) {
            for (const GameState &next : *transSet) {
                // 遷移先には新規ノード作成
                int nextId = gv.createNode(next.getLabel());
                int winner = next.getWinner();

                if (ids.find(next) == ids.end()) {
                    // 状態が初回であればノードidを登録し探索
                    ids[next] = nextId;
                    sq.push_back(next);
                } else {
                    // 状態が既出かつ
                    if (winner != 0 && winner != 1) {
                        // 勝利状態でなければ色を薄くする
                        gv.setNodeOption(nextId, "color", dupe);
                    }
                }
                if (winner == 0 || winner == 1) {
                    // 勝利状態であれば色をつける
                    gv.setNodeOption(nextId, "style", "filled");
                    gv.setNodeOption(nextId, "fillcolor", fillcolor[winner]);
                }

                gv.setTrans(stateId, nextId);
            }
        }
    }

    gv.toDotLang(out);
}

void GameSolver::calcWinTrans(int player) {
    assert(player == 0 || player == 1);
    if (isWinTransCalced[player]) {
        return;
    }

    int enemy = 1 - player;

    std::set<GameState> &pWinStates = winStates[player];    // playerの勝利確定状態の集合
    std::deque<GameState> sq(pWinStates.begin(), pWinStates.end());

    while (!sq.empty()) {
        // 幅優先探索
        GameState state = sq.front();
        sq.pop_front();

        for (const GameState &prev : transs[state].from) {
            transs[prev].loop.erase(state);
            transs[prev].win[player].insert(state);
            if (pWinStates.find(prev) == pWinStates.end()) {
                // 遷移元がまだ勝利確定状態でないとき
                if (prev.getPlayer() == player || (transs[prev].loop.empty() && transs[prev].win[enemy].empty())) {
                    // prevからstateに自分の手番で移行できる、または相手が移行せざるをえないとき
                    // 勝利確定状態に加え、探索する
                    pWinStates.insert(prev);
                    sq.push_back(prev);
                }
            }
        }

    }

    isWinTransCalced[player] = true;
}

void GameSolver::getWinGameGraph(int player, std::ostream &out) {
    assert(player == 0 || player == 1);
    calcWinTrans(player);

    GraphVisualizer gv;
    std::map<GameState, int> ids;

    for (const GameState &state : winStates[player]) {
        if (ids.find(state) == ids.end()) ids[state] = gv.createNode(state.getLabel());
        int stateId = ids[state];

        if (state.getWinner() == player) {
            // 勝利状態であれば色を付ける
            gv.setNodeOption(stateId, "style", "filled");
            gv.setNodeOption(stateId, "fillcolor", fillcolor[player]);
        }

        for (const GameState &next : transs[state].win[player]) {
            if (ids.find(next) == ids.end()) ids[next] = gv.createNode(next.getLabel());
            int nextId = ids[next];
            gv.setTrans(stateId, nextId);
        }
    }

    gv.toDotLang(out);
}