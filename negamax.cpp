#pragma GCC optimize("Ofast","unroll-loops","omit-frame-pointer","inline") //Optimization flags
#pragma GCC option("arch=native","tune=native","no-zero-upper") //Enable AVX
#pragma GCC target("avx")  //Enable AVX

#include <bits/stdc++.h>

#include <ext/pb_ds/assoc_container.hpp>

using namespace std;
using namespace chrono;

using namespace __gnu_pbds;

const int inf = 1'000'000'000;

typedef unsigned long long BB;
typedef unsigned long long Key;
typedef unsigned long long u64;

enum Color : int {
    WHITE    = 0,
    BLACK    = 1,
    COLOR_NB = 2,
};

typedef int16_t Action;

inline Action make_action(int from, int to) {
    return Action(from | (to << 6));
}

// from (x, y) to the index on the board
inline int xy(int x, int y) {
    return (x << 3) | y;
}

inline Action from_string(string action) {
    int fx = action[0] - 'a', fy = action[1] - '1'; 
    int tx = action[2] - 'a', ty = action[3] - '1';
    fy = 7 - fy; //fy = 7 - fy;
    ty = 7 - ty; //ty = 7 - ty;
    return make_action(xy(fy, fx), xy(ty, tx));
}

inline string to_string(Action action) {
    int fx = (action & 63) & 7, fy = (action & 63) >> 3;
    int tx = (action >> 6) & 7, ty = (action >> 6) >> 3;
    
    fy = 7 - fy; //fy = 7 - fy;
    ty = 7 - ty; //ty = 7 - ty;

    string res;

    res += char(fx + 'a');
    res += char(fy + '1');
    res += char(tx + 'a');
    res += char(ty + '1');

    return res;
}

inline int lsb(const BB &b) {
    // assert(b); 
    return __builtin_ctzll(b);
}

inline int pop_lsb(BB &b) {
    // assert(b);
    const int s = lsb(b);
    b &= b - 1;
    return s;
}

inline int pop_cnt(const BB &b) {
    return __builtin_popcountll(b);
}

inline void debug(const BB &b) {
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            if(b & (1ULL << (i * 8 + j))) {
                cerr << 'x';
            } else {
                cerr << '.';
            }
        }
        cerr << '\n';
    }
    cerr << '\n';
}

BB attacks[64] = {
    0x0000000000000102ULL, 0x0000000000000205ULL, 0x000000000000040aULL, 0x0000000000000814ULL, 0x0000000000001028ULL, 0x0000000000002050ULL, 0x00000000000040a0ULL, 0x0000000000008040ULL,
    0x0000000000010201ULL, 0x0000000000020502ULL, 0x0000000000040a04ULL, 0x0000000000081408ULL, 0x0000000000102810ULL, 0x0000000000205020ULL, 0x000000000040a040ULL, 0x0000000000804080ULL,
    0x0000000001020100ULL, 0x0000000002050200ULL, 0x00000000040a0400ULL, 0x0000000008140800ULL, 0x0000000010281000ULL, 0x0000000020502000ULL, 0x0000000040a04000ULL, 0x0000000080408000ULL,
    0x0000000102010000ULL, 0x0000000205020000ULL, 0x000000040a040000ULL, 0x0000000814080000ULL, 0x0000001028100000ULL, 0x0000002050200000ULL, 0x00000040a0400000ULL, 0x0000008040800000ULL,
    0x0000010201000000ULL, 0x0000020502000000ULL, 0x0000040a04000000ULL, 0x0000081408000000ULL, 0x0000102810000000ULL, 0x0000205020000000ULL, 0x000040a040000000ULL, 0x0000804080000000ULL,
    0x0001020100000000ULL, 0x0002050200000000ULL, 0x00040a0400000000ULL, 0x0008140800000000ULL, 0x0010281000000000ULL, 0x0020502000000000ULL, 0x0040a04000000000ULL, 0x0080408000000000ULL,
    0x0102010000000000ULL, 0x0205020000000000ULL, 0x040a040000000000ULL, 0x0814080000000000ULL, 0x1028100000000000ULL, 0x2050200000000000ULL, 0x40a0400000000000ULL, 0x8040800000000000ULL,
    0x0201000000000000ULL, 0x0502000000000000ULL, 0x0a04000000000000ULL, 0x1408000000000000ULL, 0x2810000000000000ULL, 0x5020000000000000ULL, 0xa040000000000000ULL, 0x4080000000000000ULL,
};

BB diagonal[64] = {
    0x0000000000000200ULL, 0x0000000000000500ULL, 0x0000000000000a00ULL, 0x0000000000001400ULL, 0x0000000000002800ULL, 0x0000000000005000ULL, 0x000000000000a000ULL, 0x0000000000004000ULL, 
    0x0000000000020002ULL, 0x0000000000050005ULL, 0x00000000000a000aULL, 0x0000000000140014ULL, 0x0000000000280028ULL, 0x0000000000500050ULL, 0x0000000000a000a0ULL, 0x0000000000400040ULL, 
    0x0000000002000200ULL, 0x0000000005000500ULL, 0x000000000a000a00ULL, 0x0000000014001400ULL, 0x0000000028002800ULL, 0x0000000050005000ULL, 0x00000000a000a000ULL, 0x0000000040004000ULL, 
    0x0000000200020000ULL, 0x0000000500050000ULL, 0x0000000a000a0000ULL, 0x0000001400140000ULL, 0x0000002800280000ULL, 0x0000005000500000ULL, 0x000000a000a00000ULL, 0x0000004000400000ULL, 
    0x0000020002000000ULL, 0x0000050005000000ULL, 0x00000a000a000000ULL, 0x0000140014000000ULL, 0x0000280028000000ULL, 0x0000500050000000ULL, 0x0000a000a0000000ULL, 0x0000400040000000ULL, 
    0x0002000200000000ULL, 0x0005000500000000ULL, 0x000a000a00000000ULL, 0x0014001400000000ULL, 0x0028002800000000ULL, 0x0050005000000000ULL, 0x00a000a000000000ULL, 0x0040004000000000ULL, 
    0x0200020000000000ULL, 0x0500050000000000ULL, 0x0a000a0000000000ULL, 0x1400140000000000ULL, 0x2800280000000000ULL, 0x5000500000000000ULL, 0xa000a00000000000ULL, 0x4000400000000000ULL, 
    0x0002000000000000ULL, 0x0005000000000000ULL, 0x000a000000000000ULL, 0x0014000000000000ULL, 0x0028000000000000ULL, 0x0050000000000000ULL, 0x00a0000000000000ULL, 0x0040000000000000ULL,
};

Key zobrist[COLOR_NB][64];
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

void init_zobrist() {

    for(int i = 0; i < 64; i++) {
        zobrist[WHITE][i] = uniform_int_distribution<Key>(0ULL, ~0ULL)(rng);
        zobrist[BLACK][i] = uniform_int_distribution<Key>(0ULL, ~0ULL)(rng);
    }
}

Color my_color, op_color;

int uf[64];
int cw[64], cb[64];
bool done[64];

struct State {

    BB pieces[COLOR_NB];

    Key hash;

    State() {
        pieces[WHITE] = 0xaa55aa55aa55aa55ULL;
        pieces[BLACK] = 0x55aa55aa55aa55aaULL;
        
        for(int i = 0; i < 64; i++) {
            if(pieces[WHITE] & (1ULL << i)) {
                hash ^= zobrist[WHITE][i];
            }
            if(pieces[BLACK] & (1ULL << i)) {
                hash ^= zobrist[BLACK][i];
            }
        }
    }

    int get(int u) { return u == uf[u] ? u : uf[u] = get(uf[u]); }

    bool merge(int u, int v) {
        u = get(u), v = get(v);
        if(u == v)  return false;
        cw[v] += cw[u];
        cb[v] += cb[u];
        uf[u] = v;
        return true;
    }

    inline int score(Color col) {
        int result = 0;

        Color opp = Color(col ^ 1);

        BB white = pieces[WHITE];

        while(white) {
            int pos = pop_lsb(white);
            cb[pos] = 0;
            cw[pos] = pop_cnt(attacks[pos]  & pieces[BLACK]) * 5 +
                      pop_cnt(diagonal[pos] & pieces[WHITE]) * 1 +
                      1;
        }

        BB black = pieces[BLACK];

        while(black) {
            int pos = pop_lsb(black);
            cw[pos] = 0;
            cb[pos] = pop_cnt(attacks[pos]  & pieces[WHITE]) * 5 +
                      pop_cnt(diagonal[pos] & pieces[BLACK]) * 1 + 
                      1;
        }

        BB all_pieces = pieces[WHITE] | pieces[BLACK];
        BB copy_all = all_pieces;

        while(all_pieces) {
            int pos = pop_lsb(all_pieces);
            uf[pos] = pos;
            done[pos] = false;
        }

        all_pieces = copy_all;

        while(all_pieces) {
            int pos = pop_lsb(all_pieces);

            BB neigh = attacks[pos] & copy_all;
            while(neigh) {
                int nei = pop_lsb(neigh);
                merge(pos, nei);
            }
        }

        all_pieces = copy_all;

        while(all_pieces) {
            int pos = get(pop_lsb(all_pieces));
            if(cw[pos] > 0 && cb[pos] > 0 && !done[pos]) {
                done[pos] = true;
                if(col == WHITE) {
                    if(cw[pos] > cb[pos])   result += cw[pos] * 1000 / cb[pos];
                    if(cw[pos] < cb[pos])   result -= cb[pos] * 1000 / cw[pos];
                } else {
                    if(cb[pos] > cw[pos])   result += cb[pos] * 1000 / cw[pos];
                    if(cb[pos] < cw[pos])   result -= cw[pos] * 1000 / cb[pos];
                }
            }
        }

        return result;
    }

    inline void debug() const {
        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {
                int sq = i * 8 + j;
                char c = '.';
                if(pieces[WHITE] & (1ULL << sq))    c = 'w';
                if(pieces[BLACK] & (1ULL << sq))    c = 'b';
                cerr << c;
            }
            cerr << '\n';
        }
        cerr << '\n';
    }

    // doesn't check if move is correct
    inline void move(int from, int to, Color col) {
        pieces[col]     &= ~(1ULL << from);
        pieces[col ^ 1] &= ~(1ULL << to);
        pieces[col]     |=   1ULL << to;
  
        hash ^= zobrist[col][from];
        hash ^= zobrist[col ^ 1][to];
        hash ^= zobrist[col][to];      
    }

    inline void move(Action action, Color col) {
        move(action & 63, action >> 6, col);
    }
    
    inline void undo(int from, int to, Color col) {
        pieces[col]     &= ~(1ULL << to);
        pieces[col ^ 1] |=   1ULL << to;
        pieces[col]     |=   1ULL << from;
    
        hash ^= zobrist[col][from];
        hash ^= zobrist[col ^ 1][to];
        hash ^= zobrist[col][to];
    }
    
    inline void undo(Action action, Color col) {
        undo(action & 63, action >> 6, col);
    }

    inline BB get_active(Color col) const {
        return ((pieces[col ^ 1] << 8) |
                (pieces[col ^ 1] >> 8) |
                ((pieces[col ^ 1] & 0x7f7f7f7f7f7f7f7fULL) << 1) |
                ((pieces[col ^ 1] & 0xfefefefefefefefeULL) >> 1))
                 & pieces[col];
    }

    // generates all legal moves
    inline Action* gen_moves(Color col, Action* actions) const {
        BB bb = get_active(col);
        
        while(bb) {
            int from = pop_lsb(bb);
            BB to_mask = attacks[from] & pieces[col ^ 1];
            while(to_mask) {
                int to = pop_lsb(to_mask);
                *actions++ = make_action(from, to);
            }
        }
        return actions;
    }

    inline int count_moves(Color col) const {
        BB bb = get_active(col);

        int moves = 0;
        while(bb) {
            int from = pop_lsb(bb);
            BB to_mask = attacks[from] & pieces[col ^ 1];
            while(to_mask) {
                pop_lsb(to_mask);
                moves++;
            }
        }
        return moves;
    }
};

State state;

double time_limit;

clock_t start;
bool quit_minmax;

gp_hash_table<Key, int> cache[80];
gp_hash_table<Key, Action> cache_action[80];

inline int negamax(int depth, int max_depth, Color col = WHITE, int alpha = -inf, int beta = +inf) {

    if(cache[max_depth].find(state.hash) != cache[max_depth].end()) {
        return cache[max_depth][state.hash];
    }

    if(depth >= max_depth) {
        return state.score(col);
    }

    if(clock() - start > CLOCKS_PER_SEC * time_limit * 0.90) {
        quit_minmax = true;
        return 0;
    }

    Action actions[113];

    Action *end = state.gen_moves(col, actions);

    int count = int(end - actions);

    pair<int, Action> ordered_actions[113];

    for(int i = 0; i < count; i++) {
        ordered_actions[i].second = actions[i];

        if(depth >= max_depth - 2) {
            ordered_actions[i].first = -rng();
            continue;
        }

        auto it = cache[max_depth - 1].find(state.hash);

        if(it != cache[max_depth - 1].end()) {
            ordered_actions[i].first = -it->second;
        } else {
            ordered_actions[i].first = -rng();
        }
    }

    sort(ordered_actions, ordered_actions + count);

    int value = -inf - 1;
    Action best_action = 0;

    for(int act_id = 0; act_id < count; act_id++) {
        Action now = ordered_actions[act_id].second;

        state.move(now, col);

        int new_max_depth = max_depth;

        if(act_id > 40) new_max_depth --;
        if(act_id > 100)    new_max_depth --;

        int current = -negamax(depth + 1, new_max_depth, Color(col ^ 1), -beta, -alpha);
        
        if(quit_minmax) {
            state.undo(now, col);
            return 0;
        }
        
        if(value < -inf || current > value) {
            value = current;
            best_action = now;
            
            if(alpha < value) {
                alpha = value;
            }
        }
        
        state.undo(now, col);
        
        if(alpha >= beta)   break;
    }

    cache_action[max_depth][state.hash] = best_action;
    cache       [max_depth][state.hash] = value;
    return value;
}

int main() {
    init_zobrist();
    
    int boardSize; 
    cin >> boardSize; cin.ignore();
    
    string color;  
    cin >> color; cin.ignore();
    
    my_color = (color == "w" ? WHITE : BLACK);
    op_color = (color == "b" ? WHITE : BLACK);

    time_limit = 1.;
    for(int turn = 0; ; turn += 2) {
        if(turn > 0) {
            cache[turn - 2].clear();
            cache_action[turn - 2].clear();
            cache[turn - 1].clear();
            cache_action[turn - 1].clear();            
        }
               
        for(int i = 0; i < boardSize; i++) {
            string line;
            cin >> line; cin.ignore();
        }
        string lastAction;
        cin >> lastAction; cin.ignore();
        int actionsCount;
        cin >> actionsCount; cin.ignore();
        
        assert(actionsCount > 0);
    
        if(lastAction != "null") {
            // state.debug();
            state.move(from_string(lastAction), op_color);
        }
    
        Action best_action = 0;    

        // state.debug();

        cerr << "score: " << state.score(my_color) << '\n';

        cerr << "# of moves: " << state.count_moves(my_color) << '\n';

        start = clock();
        for(int depth = 2; depth + turn < 70 && !quit_minmax; depth++) {
            cerr << "calculating: " << depth << " = " << negamax(turn, turn + depth, my_color) << '\n';
            cerr << "best move: " << to_string(best_action) << '\n';
            cerr << "max depth: " << depth + turn << '\n';
            if(!quit_minmax) {
                assert(cache_action[turn + depth].find(state.hash) != cache_action[turn + depth].end());
                best_action = cache_action[turn + depth][state.hash];
            }
        }
        
        state.move(best_action, my_color);
         
        cout << to_string(best_action) << endl;

        quit_minmax = false;
        time_limit = 0.1;
    }
}

