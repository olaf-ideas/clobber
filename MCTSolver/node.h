#pragma once

#include <bits/stdc++.h>

#include "../State/state.h"

constexpr float EXP_RATE = 2.f;

constexpr int NODE_POOL = 2'000'000;
constexpr int ACTION_POOL = 100'000'000;

Action action_pool[ACTION_POOL];
int action_pool_size = 0;

int index_pool[ACTION_POOL];
int index_pool_size = 0;

class Node {
	
public:
	
	static Node node_pool[NODE_POOL];
	static int node_pool_size;
	
	State state;
	
	int beg;
	int end;
	
	char child_len;
	
	float vis;
	float val;
	
	Node() { }
	
	Node(State _state) : state(_state), vis(0), val(0) { }
	
	void expand() {
		
		beg = action_pool_size;
		end = state.get_actions(action_pool + action_pool_size) - action_pool;
		
		std::random_shuffle(action_pool + beg, action_pool + end);
		
		child_len = 0;
		
		action_pool_size += end - beg;
		index_pool_size  += end - beg;
	}
	
	float ucb(float sqrt_log_vis) const {
		float inv_sqrt = rsqrt_fast(vis + 1);
		return sqrt_log_vis * inv_sqrt + val;
	}
	
	float node_score() const {
		return fastsqrtf(vis + 1) + val;
	}
	
	void build() {
		for(int id = end; id != beg; id--) {
			select();
		}
	}
	
	Node* select() {
		if(beg != end) {
			child_len++;
			
			state.move(action_pool[--end]);
			
			index_pool[end] = node_pool_size;
			
			node_pool[node_pool_size++] = Node(state);
			
			state.undo(action_pool[  end]);
			
			return &node_pool[index_pool[end]];
		}
		
		float best_ucb = -2 * infinity;
		Node *best_node = nullptr;
		
		float sqrt_log_vis = EXP_RATE * fastsqrtf(fastlogf(vis));
		
		for(int child_id = 0; child_id < child_len; child_id++) {
			Node *child = &node_pool[index_pool[beg + child_id]];
			
			float ucb = child->ucb(sqrt_log_vis);
			if(ucb > best_ucb) {
				best_ucb = ucb;
				best_node = child;
			}
		}
		
#ifdef DEBUG
		assert(best_node != nullptr);
#endif

		return best_node;
	}
	
	void update(int new_result) {
		val *= vis;
		val += -new_result;
		vis++;
		val /= vis;
	}
	
	Action best_action() {
#ifdef DEBUG
		assert(beg == end);
#endif
		
		Action best_action = -1;
		float best_score = -2 * infinity;
		
		for(int child_id = 0; child_id < child_len; child_id++) {
			float score = node_pool[index_pool[beg + child_id]].node_score();
			
			if(score > best_score) {
				best_score = score;
				best_action = action_pool[beg + child_id];
			}
		}
#ifdef DEBUG
		assert(best_action != -1);
#endif
		return best_action;
	}
	
	Node* pass_action(Action action) {
#ifdef DEBUG
		assert(beg == end);
#endif
		for(int child_id = 0; child_id < child_len; child_id++) {
			if(action_pool[beg + child_id] == action) {
				return &node_pool[index_pool[beg + child_id]];
			}
		}
#ifdef DEBUG
		assert(false);
#endif
		return nullptr;
	}
	
	Node* get_child(int child_id) {
		return &node_pool[index_pool[beg + child_id]];
	}
};

Node Node::node_pool[NODE_POOL];
int  Node::node_pool_size = 0;
