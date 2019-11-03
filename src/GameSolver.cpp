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

GameSolver::GameSolver(GameRule rule) : rule(rule) {
    isWinTransCalced[0] = false;
    isWinTransCalced[1] = false;

    // ゲームグラフの全探索
    std::deque<GameState> bfsq{rule.firstState()};
    while (!bfsq.empty()) {
        GameState state = bfsq.front();
        bfsq.pop_front();

        // 勝利状態を勝利確定状態の集合に入れる
        int winner = rule.getWinner(state);
        if (winner == 0 || winner == 1) {
            winStates[winner].insert(state);
        }

        rule.calcTransSet(state, transs[state].loop); // 暫定千日手遷移に全ての遷移を追加
        for (const GameState &next : transs[state].loop) {
            if (transs.find(next) == transs.end()) {
                // 初回なら探索する
                bfsq.push_back(next);
            }
            transs[next].from.insert(state);
        }
    }
}

void GameSolver::makeAllGameGraph(std::ostream &out) {
    GraphVisualizer gv;
    std::map<GameState, int> ids;   // 状態とグラフのノードidを対応づける

    GameState firstState = rule.firstState();
    ids[firstState] = gv.createNode(rule.getLabel(firstState));

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
                int nextId = gv.createNode(rule.getLabel(next));
                int winner = rule.getWinner(next);

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

void GameSolver::makeWinGameGraph(int player, std::ostream &out) {
    assert(player == 0 || player == 1);
    calcWinTrans(player);

    GraphVisualizer gv;
    std::map<GameState, int> ids;

    for (const GameState &state : winStates[player]) {
        if (ids.find(state) == ids.end()) ids[state] = gv.createNode(rule.getLabel(state));
        int stateId = ids[state];

        if (rule.getWinner(state) == player) {
            // 勝利状態であれば色を付ける
            gv.setNodeOption(stateId, "style", "filled");
            gv.setNodeOption(stateId, "fillcolor", fillcolor[player]);
        }

        for (const GameState &next : transs[state].win[player]) {
            if (ids.find(next) == ids.end()) ids[next] = gv.createNode(rule.getLabel(next));
            int nextId = ids[next];
            gv.setTrans(stateId, nextId);
        }
    }

    gv.toDotLang(out);
}

void GameSolver::makeLoopGameGraph(std::ostream &out, GraphLoopMode loopMode) {
    calcWinTrans(0);
    calcWinTrans(1);

    GraphVisualizer gv;
    std::map<GameState, int> ids;   // allowLoopのとき用いる
    std::set<GameState> isVisited;  // noLoopのとき用いる
    GameState firstState = rule.firstState();
    int firstId = gv.createNode(rule.getLabel(firstState));
    if (loopMode == allowLoop) ids[firstState] = firstId;

    // (状態, ノードID)をdequeに入れて探索する
    std::deque<std::pair<GameState, int>> sq{std::make_pair(firstState, firstId)};
    while (!sq.empty()) {
        auto &pair = sq.front();
        GameState state = pair.first;
        int stateId = pair.second;
        sq.pop_front();

        // 勝利確定状態か調べる
        int winner = -1;
        if (winStates[0].find(state) != winStates[0].end()) {
            winner = 0;
        } else if (winStates[1].find(state) != winStates[1].end()) {
            winner = 1;
        }
        if (winner != -1) {
            // 勝利確定状態のとき
            // 色をつけ、それ以上探索しない
            gv.setNodeOption(stateId, "style", "filled");
            gv.setNodeOption(stateId, "fillcolor", fillcolor[winner]);
        } else if (loopMode == noLoop) {
            // 千日手状態で、noLoopモードのとき
            if (isVisited.find(state) != isVisited.end()) {
                // 既出の状態のとき
                // 色を変えてそれ以上探索しない
                gv.setNodeOption(stateId, "color", dupe);
            } else {
                // 新しい状態のとき
                for (const GameState &next : transs[state].loop) {
                    // 千日手遷移について必ず新規ノードを作成し探索する
                    int nextId = gv.createNode(rule.getLabel(next));
                    gv.setTrans(stateId, nextId);
                    sq.push_back(std::make_pair(next, nextId));
                }
                isVisited.insert(state);
            }
        } else if (loopMode == allowLoop) {
            // 千日手状態で、allowLoopモードのとき
            for (const GameState &next : transs[state].loop) {
                int nextId;
                if (ids.find(next) == ids.end()) {
                    // 状態が初回ならノードを作成し、探索する
                    nextId = gv.createNode(rule.getLabel(next));
                    ids[next] = nextId;
                    sq.push_back(std::make_pair(next, nextId));
                } else {
                    // 状態が既出ならそれ以上探索しない
                    nextId = ids[next];
                }
                gv.setTrans(stateId, nextId);
            }
        }
    }

    gv.toDotLang(out);
}