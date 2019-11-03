#include "GameState.hpp"
#include <cassert>
#include <algorithm>
#include <tuple>
#include <sstream>

GameState::GameState(int player, int p1, int p2, int e1, int e2) {
    assert(player == 0 || player == 1);
    this->player = player;

    // l <= r となるようにする
    l[player] = std::min(p1, p2);
    r[player] = std::max(p1, p2);
    int enemy = 1 - player;
    l[enemy] = std::min(e1, e2);
    r[enemy] = std::max(e1, e2);
}

int GameState::getPlayer() const {
    return player;
}
int GameState::getL(int player) const {
    assert(player == 0 || player == 1);
    return l[player];
}
int GameState::getR(int player) const {
    assert(player == 0 || player == 1);
    return r[player];
}
std::tuple<int, int, int, int, int> GameState::makeTuple() const {
    int enemy = 1 - player;
    return std::make_tuple(player, l[player], r[player], l[enemy], r[enemy]);
}

bool operator <(const GameState &l, const GameState &r) {
    return l.makeTuple() < r.makeTuple();
}



GameRule::GameRule(int max, RuleOverflow overflow, RuleBunshin bunshin) {
    this->max = max;
    this->overflow = overflow;
    this->bunshin = bunshin;
}

int GameRule::getPlayer(const GameState &state) const {
    return state.getPlayer();
}

int GameRule::getWinner(const GameState &state) const {
    if (state.getL(0) == 0 && state.getR(0) == 0) {
        return 1;
    }
    if (state.getL(1) == 0 && state.getR(1) == 0) {
        return 0;
    }
    return -1;
}

void GameRule::calcTransSet(const GameState &state, std::set<GameState> &set) const {
    int player = state.getPlayer();
    int enemy = 1 - player;
    int pl = state.getL(player), pr = state.getR(player);
    int el = state.getL(enemy), er = state.getR(enemy);
    
    // 自分左で
    if (pl != 0) {
        // 相手左を攻撃
        if (el != 0) {
            set.insert(GameState(enemy, ofAct(el + pl), er, pl, pr));
        }
        // 相手右を攻撃
        if (er != 0) {
            set.insert(GameState(enemy, el, ofAct(er + pl), pl, pr));
        }
        // 自分右を攻撃
        if (pr != 0) {
            set.insert(GameState(enemy, el, er, pl, ofAct(pr + pl)));
        }
    }
    // 自分右で
    if (pr != 0) {
        // 相手左を攻撃
        if (el != 0) {
            set.insert(GameState(enemy, ofAct(el + pr), er, pl, pr));
        }
        // 相手右を攻撃
        if (er != 0) {
            set.insert(GameState(enemy, el, ofAct(er + pr), pl, pr));
        }
        // 自分左を攻撃
        if (pl != 0) {
            set.insert(GameState(enemy, el, er, ofAct(pl + pr), pr));
        }
    }
    // 分身
    if (bunshin == RuleBunshin::allow || (bunshin == RuleBunshin::resuscitateOnly && pl == 0)) {
        int lmin = std::max(pl + pr - max, 0);
        int lmax = std::min(pl + pr, max);
        for (int i = lmin; i <= lmax; i++) {
            if (i == pl) continue;
            int nl = i;
            int nr = pl + pr - i;
            if (nl > nr) break;
            set.insert(GameState(enemy, el, er, nl, nr));
        }
    }
}

std::string GameRule::getLabel(const GameState &state) const {
    std::stringstream out;
    out << (state.getPlayer() == 0 ? "first" : "second") << "\\n"
        << "f{" << state.getL(0) << ", " << state.getR(0) << "}\\n"
        << "s{" << state.getL(1) << ", " << state.getR(1) << "}";
    return out.str();
}

GameState GameRule::firstState() const {
    return GameState(0, 1, 1, 1, 1);
}

int GameRule::ofAct(int finger) const {
    switch(overflow) {
    case RuleOverflow::mod:
        finger %= (max + 1);
        break;
    case RuleOverflow::death:
        if (finger > max) {
            finger = 0;
        }
        break;
    }
    return finger;
}