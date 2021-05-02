#pragma GCC optimize("Ofast","unroll-loops","omit-frame-pointer","inline")
#pragma GCC option("arch=native","tune=native","no-zero-upper")
#pragma GCC target("avx2","popcnt","rdrnd","bmi2")

#include <bits/stdc++.h>

#include "MCTS/mcts.h"

MCTS mcts;

int main() {

/*
    {
        int white = 0, black = 0;
        for(int rep = 0; rep < 1000; rep++) {
            std::cerr << "rep: " << rep << '\n';
            MCTS now;

            for(int turn = 0; !now.root->is_terminal(); turn++) {
                if(turn < 2)    now.run(1000);
                else            now.run(100);
            
                Action action = now.best_action();

                std::cout << to_string(action) << ' ' << std::fixed << std::setprecision(4) << now.win_rate() * 100 << "%" << '\n';
                
                now.pass_action(action);
        
                now.root->state.debug();
            }
            
            if(now.root->state.get_player() == WHITE)   std::cerr << "WHITE lost\n", white++;
            else                                        std::cerr << "BLACK lost\n", black++;
        }
    
        std::cerr << "white : " << white << " / " << black << " : black" << '\n';
    }
*/

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
      
        mcts.run(time_limit);

        Action best_action = mcts.best_action();
 
        std::cout << to_string(best_action) << '\n';
 
        std::cerr << "pool%: " << Node::pool_size / float(POOL_SIZE) * 100 << '\n';

        mcts.pass_action(best_action);
        
        time_limit = 100;

#ifdef LOCAL
        return 0;
#endif
    }
}
