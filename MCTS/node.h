#pragma once

#include <bits/stdc++.h>

#include "../State/state.h"

constexpr int POOL_SIZE = 25'000'000;

constexpr float infinity = std::numeric_limits<float>::max();

constexpr float EXP_RATE = 1.5f;

Action actions[113];

class Node {

public:

    static Node pool[POOL_SIZE];
    static int  pool_size;

    // 29 bytes

    State state;
    
    int   vis;
    int   win;
    int   child_beg;
    char  child_len;

    Node() { }

    Node(State _state) : state(_state), vis(0), win(0) { }

    inline bool is_terminal() const { return state.is_end(); }
    inline bool is_expanded() const { return vis != 0; }

    inline void expand() {

        Action *end = state.get_actions(actions);
            
        child_beg = pool_size;
        child_len = char(end - actions);

        for(Action *now = actions; now != end; now++) {
            state.move(*now);
            pool[pool_size++] = Node(state);
            if(pool_size == POOL_SIZE)  pool_size = 0;
            state.undo(*now);
        }
    }

    inline float get_ucb(float sqrt_log_vis) {
        if(vis == 0)    return infinity;
        float inv_sqrt = rsqrt_fast(vis);
        return (((vis - win) * inv_sqrt) + sqrt_log_vis) * inv_sqrt;
    }

    inline Node* select() {
        float best_ucb = -infinity;
        Node* best_node = nullptr;

        float sqrt_log_vis = EXP_RATE * fastsqrtf(fastlogf(vis));
        for(int child_id = child_len - 1; child_id >= 0; child_id--) {
            int id = child_beg + child_id;
            if(id > POOL_SIZE)  id -= POOL_SIZE;
            Node *child = &pool[id];
        
            float child_ucb = child->get_ucb(sqrt_log_vis);
            if(child_ucb > best_ucb) {
                best_ucb = child_ucb;
                best_node = child;
            }
            if(best_ucb == infinity)    break;
        }
        assert(best_node != nullptr);
        return best_node;
    }

    inline Action best_action() {
        int best_vis = -1;
        Action action = -1;
    
        state.get_actions(actions);

        for(int child_id = 0; child_id < child_len; child_id++) {
            int id = child_beg + child_id;
            if(id > POOL_SIZE)  id -= POOL_SIZE;
            Node *child = &pool[id];

            if(child->vis > best_vis) {
                best_vis = child->vis;
                action = actions[child_id];        
            }
        }

        assert(action != -1);

        return action;
    }

    inline void update(Color loser) {
        vis++;
        if(state.get_player() != loser) {
            win++;
        }
    }

    inline Node* get_child(Action action) {
        state.get_actions(actions);

        for(int child_id = 0; child_id < child_len; child_id++) {
            if(actions[child_id] == action) {
                return &pool[child_beg + child_id];
            }
        }
        assert(false);
        return nullptr;
    }

};

Node Node::pool[POOL_SIZE];
int  Node::pool_size = 0;
