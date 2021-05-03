#pragma GCC optimize("Ofast","unroll-loops","omit-frame-pointer","inline")
#pragma GCC option("arch=native","tune=native","no-zero-upper")
#pragma GCC target("avx2","popcnt","rdrnd","bmi2")

#include <bits/stdc++.h>

#include "MCTSolver/mctsolver.h"

MCTSolver mcts;

int main() {
	
	//for(int turn = 0; !mcts.root->state.is_end(); turn++) {
		//mcts.timer.start();
		//mcts.run(turn < 2 ? 1000 : 100);
		//std::cerr << to_string(mcts.best_action()) << ' ' << (mcts.root->state.get_player() == WHITE ? "white" : "black") << '\n';
		//mcts.pass_action(mcts.best_action());
		
		//if(turn == 27)	mcts.print(mcts.root, 2);
		
		//std::cerr << "turn: " << turn << '\n';
		//std::cerr << "node pool: " << Node::node_pool_size / float(NODE_POOL) << '\n';
		//std::cerr << "action pool: " << action_pool_size / float(ACTION_POOL) << '\n';
		 //mcts.root->state.debug();
		//std::cerr << std::fixed << std::setprecision(6) << "value: " << mcts.root->val << '\n';
		//std::cerr << '\n';
	//}
	
	//return 0;
	
	int board_size;
	std::cin >> board_size;
	
	std::string color;
	std::cin >> color;
	
	int time_limit = 1000;
	for(int turn = 0; ; turn += 2) {
		
		for(int i = 0; i < board_size; i++) {
			std::string line;
			std::cin >> line;
			if(i == 0)  mcts.timer.start();
		}
		
		std::string last_action;
		std::cin >> last_action;
		
		int action_count;
		std::cin >> action_count;
		 
		if(last_action != "null") {
			mcts.pass_action(from_string(last_action));
		}
		
		mcts.timer.start();
		mcts.run(time_limit);
		
		Action best_action = mcts.best_action();
		
		mcts.pass_action(best_action);
		
		std::cout << to_string(best_action) << ' ' << (mcts.root->val == +infinity ? "EZ WIN" : "") << (mcts.root->val == -infinity ? "GG WP" : "") << '\n';
		std::cerr << "node pool: " << Node::node_pool_size / float(NODE_POOL) << '\n';
		std::cerr << "action pool: " << action_pool_size / float(ACTION_POOL) << '\n';
		std::cerr << "nodes: " << Node::node_pool_size << '\n';
		std::cerr << "value  : " << std::fixed << std::setprecision(6) << mcts.root->val << '\n';
		std::cerr << '\n';
		
		time_limit = 100;
		
#ifdef LOCAL
	return 0;
#endif
	}
}
