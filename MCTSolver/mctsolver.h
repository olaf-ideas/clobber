#pragma once

#include <bits/stdc++.h>

#include "../State/state.h"
#include "../Utils/types.h"
#include "../Utils/timer.h"
#include "../MCTSolver/node.h"

class MCTSolver {
private:
	
	Color simulate(State state) {
		while(!state.is_end()) {
			state.move(state.get_random_action());
		}
		return state.get_player();
	}
	
	int solve(Node *node) {
		if(node->state.is_end()) {
			node->val = +infinity;
			return -infinity;
		}
		
		Node *child = node->select();
		
		int result = 0;
		
		if(child->val == +infinity ||
		   child->val == -infinity) {
			
			result = child->val;
			
		} else if(child->vis > 0) {
			
			result = -solve(child);
			
		} else {
			
			child->expand();
			
			result = (simulate(child->state) == node->state.get_player() ? -1 : +1);
			
			child->update(-result);
			
			node ->update(+result);
			
			return result;
			
		}
		
		if(result == +infinity) {
			node->val = -infinity;
			return result;
		}
		
		if(result == -infinity) {
			for(int child_id = 0; child_id < node->child_len; child_id++) {
				if(node->get_child(child_id)->val != result) {
					result = -1;
					node->update(result);
					
					return result;
				}
			}
			
			node->val = +infinity;
			return result;
		}
		
		node->update(result);
		
		return result;
	}
	
public:
	
	Timer timer;
	Node* root;
	
	MCTSolver() {
		Node::node_pool_size = 0;
		Node::node_pool[Node::node_pool_size] = Node(State());
		
		root = &Node::node_pool[Node::node_pool_size++];
		
		root->expand();
	}
	
	void run(int time_limit) {
		do {
			solve(root);
		} while(timer.get_elapsed() < time_limit - 5);
	}
	
	Action best_action() {
		return root->best_action();
	}
	
	void pass_action(Action action) {
		if(root->vis == 0) {
			root->expand();
			root->build();
		}
		root = root->pass_action(action);
		if(root->vis == 0) {
			root->expand();
		}
	}
	
	void print(Node *node, int depth) {
		if(depth == 0)	return;
		assert(node != nullptr);
		std::cerr << node->val << ": ";
		for(int child_id = 0; child_id < node->child_len; child_id++) {
			std::cerr << node->get_child(child_id)->val << ' ';
		}
		std::cerr << '\n';
		
		for(int child_id = 0; child_id < node->child_len; child_id++) {
			print(node->get_child(child_id), depth - 1);
		}
	}
	
};
