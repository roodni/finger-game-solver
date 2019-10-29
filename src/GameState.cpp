#include "GameState.hpp"
#include <cassert>
#include <algorithm>
#include <tuple>

// MODルール
const int mod = 5;

GameState::GameState(int player, int p1, int p2, int e1, int e2) {
    // l <= r となるようにする
    l[player] = std::min(p1, p2);
    r[player] = std::max(p1, p2);
    assert(0 <= l[player] && r[player] < mod);

    int enemy = 1 - player;
    l[enemy] = std::min(e1, e2);
    r[enemy] = std::max(e1, e2);
    assert(0 <= l[enemy] && r[enemy] < mod);

    assert(player == 0 || player == 1);
    this->player = player;
}

int GameState::getPlayer() const{
    return player;
}

int GameState::getWinner() const{
    if (l[0] == 0 && r[0] == 0) {
        return 1;
    }
    if (l[1] == 0 && r[1] == 0) {
        return 0;
    }
    return -1;
}

void GameState::addTransSet(std::set<GameState> &set) const {
    int enemy = 1 - player;
    int pl = l[player], pr = r[player];
    int el = l[enemy], er = r[enemy];
    
    // 自分左で
    if (pl != 0) {
        // 相手左を攻撃
        if (el != 0) {
            set.insert(GameState(enemy, (el + pl) % mod, er, pl, pr));
        }
        // 相手右を攻撃
        if (er != 0) {
            set.insert(GameState(enemy, el, (er + pl) % mod, pl, pr));
        }
        // 自分右を攻撃
        if (pr != 0) {
            set.insert(GameState(enemy, el, er, pl, (pr + pl) % mod));
        }
    }
    // 自分右で
    if (pr != 0) {
        // 相手左を攻撃
        if (el != 0) {
            set.insert(GameState(enemy, (el + pr) % mod, er, pl, pr));
        }
        // 相手右を攻撃
        if (er != 0) {
            set.insert(GameState(enemy, el, (er + pr) % mod, pl, pr));
        }
        // 自分左を攻撃
        if (pl != 0) {
            set.insert(GameState(enemy, el, er, (pl + pr) % mod, pr));
        }
    }
    // 分身
    int lmin = std::max(pl + pr - (mod - 1), 0);
    int lmax = std::min(pl + pr, mod - 1);
    for (int i = lmin; i <= lmax; i++) {
        if (i == pl) continue;
        int nl = i;
        int nr = pl + pr - i;
        if (nl > nr) break;
        set.insert(GameState(enemy, el, er, nl, nr));
    }
}

std::tuple<int, int, int, int, int> GameState::makeTuple() const {
    int enemy = 1 - player;
    return std::make_tuple(player, l[player], r[player], l[enemy], r[enemy]);
}

bool operator <(const GameState &l, const GameState &r) {
    return l.makeTuple() < r.makeTuple();
}