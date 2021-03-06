#pragma once

#include <bits/stdc++.h>

#include "../State/state.h"
#include "../Utils/types.h"
#include "../Utils/timer.h"
#include "../MCTS/node.h"

class MCTS {

public:

    Timer timer;

    Node *root;
   
    MCTS() {
        Node::pool_size = 0;
        Node::pool[Node::pool_size] = Node(State());

        root = &Node::pool[Node::pool_size++];
        root->expand();
    }
    
    inline Color simulate(Node* node) {
        State state = node->state;
        while(!state.is_end()) {
            state.move(state.get_random_action());
        }
        return state.get_player();
    }
    
    inline Color select(Node *node) {
        if(node->is_terminal()) {
            Color loser = node->state.get_player();
            node->update(loser);
            return loser;
        }

        if(node->is_expanded()) {
            Color loser = select(node->select());
            node->update(loser);
            return loser;
        }

        node->expand();
        Color loser = simulate(node);
        node->update(loser);
        return loser;
    }
    
    inline void run(int time_limit) {
        
        int iterations = 0;
        do {
            select(root);
            iterations++;
        } while(timer.get_elapsed() < time_limit - 5);
        
        // std::cerr << "iterations: " << iterations << '\n';
    }

    inline Action best_action() {
        return root->best_action(); 
    }

    inline void pass_action(Action action) {
        if(root->vis == 0)  root->expand();
        root = root->get_child(action);
    }

    inline float win_rate() {
        return root->win / float(root->vis);
    }
};
