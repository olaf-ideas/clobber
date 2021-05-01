#pragma once

#include <bits/extc++.h>

using namespace __gnu_pbds;

#include "../State/state.h"


gp_hash_table<Key, State> cache_state;
gp_hash_table<Key, int> cache_visited;
gp_hash_table<Key, Color> cache_loser;

inline Color get_loser(State &state) {
        
    const Key hash = state.get_hash();

    cache_visited[hash]++;
    cache_state[hash] = state;
    
    auto it = cache_loser.find(state.get_hash());
    if(it != cache_loser.end()) {
        cache_visited[state.get_hash()]++;
        return it->second;
    }

    Action actions[112];
    Action* end = state.get_actions(actions);
    for(Action *now = actions; now != end; now++) {
        state.move(*now);

        Color loser = get_loser(state);

        state.undo(*now);

        if(loser != state.get_player()) {
            return cache_loser[state.get_hash()] = loser;
        }
    }

    return cache_loser[state.get_hash()] = state.get_player();
}

class Search {
private:
    State state;
    gp_hash_table<Key, int> cache[70];
    gp_hash_table<Key, Action> cache_action[70];

    int time_limit; // in milliseconds

    int turn;
    bool quit_search;

    clock_t start;

    static constexpr int inf = 1'000'000'000;

    inline int negamax(int depth, const int &max_depth, int alpha = -inf, int beta = +inf) {

        auto it = cache[max_depth].find(state.get_hash());
        if(it != cache[max_depth].end()) {
            return it->second;
        }

        if(depth == max_depth) {
            return state.score();
        }

        if((clock() - start) * 950 > time_limit * CLOCKS_PER_SEC) {
            quit_search = true;
            return 0;
        }

        Action actions[113];

        Action *end = state.get_actions(actions);

        int value = -inf;

        Action best_action = -1;

        for(Action *now = actions; now != end; now++) {
            state.move(*now);

            int current = -negamax(depth + 1, max_depth, -beta, -alpha);

            if(quit_search) {
                state.undo(*now);
                return 0;
            }

            if(current > value) {
                value = current;
                best_action = *now;
                if(alpha < value) {
                    alpha = value;
                }
            }

            state.undo(*now); 
        
            if(alpha >= beta)   break;
        }
        
        cache_action[max_depth][state.get_hash()] = best_action;
        cache       [max_depth][state.get_hash()] = value;
        return value;
    }

public:

    Search(State _state = State()) : state(_state), turn(0) {

    }

    inline void set_time_limit(int new_time_limit) {
        time_limit = new_time_limit;
    }

    inline void pass_action(Action action) {
        state.move(action);

        turn++;
    }

    inline Action get_best_action() {
        start = clock();        

        quit_search = false;

        Action best_action = -1;
        for(int depth = 2; turn + depth < 70 && !quit_search; depth++) {
            negamax(turn, turn + depth);
            if(!quit_search) {
                best_action = cache_action[turn + depth][state.get_hash()];
            }
        }

        pass_action(best_action);
    }

    inline u64 perft(int depth) {
        if(depth == 0)  return 1;

        Action actions[112];

        Action *end = state.get_actions(actions);

        if(depth == 1)  return u64(end - actions);

        if(actions == end)  return 1;
        
        u64 nodes = 1;
    
        for(Action *now = actions; now != end; now++) {
            state.move(*now);

            nodes += perft(depth - 1);

            state.undo(*now);
        }

        return nodes;
    }

    inline float perft2(int random_depth, int branch_factor) {
        if(random_depth == 0) {
            u64 res = perft(64);
            return res;
        }

        Action actions[112];

        Action *end = state.get_actions(actions);

        if(actions == end)  return 1;

        std::random_shuffle(actions, end);

        float nodes = 0;

        for(int act_id = 0; act_id < std::min(int(end - actions), branch_factor); act_id++) {
            state.move(actions[act_id]);
            nodes += perft2(random_depth - 1, branch_factor);
            state.undo(actions[act_id]);
        }

        return nodes / std::min(int(end - actions), branch_factor);
    } 
};
