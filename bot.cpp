// *** Start of: /home/olaf/clobber/main.cpp *** 
#pragma GCC optimize("Ofast","unroll-loops","omit-frame-pointer","inline")
#pragma GCC option("arch=native","tune=native","no-zero-upper")
#pragma GCC target("avx2","popcnt","rdrnd","bmi2")

#include <bits/stdc++.h>

 // *** Start of: /home/olaf/clobber/MCTSolver/mctsolver.h *** 
 
 #include <bits/stdc++.h>
 
  // *** Start of: /home/olaf/clobber/State/state.h *** 
  
   // *** Start of: /home/olaf/clobber/Utils/types.h *** 
   
   #include <string>
   
   #include <x86intrin.h>
   
   typedef unsigned long long BB;
   typedef unsigned long long Key;
   typedef unsigned long long u64;
   
   typedef int8_t Color;
   
   constexpr Color WHITE = 0;
   constexpr Color BLACK = 1;
   constexpr Color COLOR_NB = 2;
   
   constexpr int infinity = 1'000'000'000;
   
   typedef int16_t Action;
   
   inline float fastlogf(const float& x) { union { float f; uint32_t i; } vx = { x }; float y = vx.i; y *= 8.2629582881927490e-8f; return(y - 87.989971088f); }
   inline float fastsqrtf(const float& x) { union { int i; float x; } u; u.x = x; u.i = (1 << 29) + (u.i >> 1) - (1 << 22); return(u.x); }
   inline float rsqrt_fast(float x) { return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ss(x))); }
   
   inline Action make_action(const int &from, const int &to) {
       return Action(from | (to << 6));
   }
   
   inline int xy(int x, int y) {
       return (x << 3) | y;
   }
   
   inline Action from_string(std::string action) {
   	int fx = action[0] - 'a', fy = action[1] - '1'; 
   	int tx = action[2] - 'a', ty = action[3] - '1';
   	fy = 7 - fy;
   	ty = 7 - ty;
   	return make_action(xy(fy, fx), xy(ty, tx));
   }
   
   inline std::string to_string(Action action) {
   	int fx = (action & 63) & 7, fy = (action & 63) >> 3;
   	int tx = (action >> 6) & 7, ty = (action >> 6) >> 3;
   
   	fy = 7 - fy;
   	ty = 7 - ty;
       
   	std::string res;
   
   	res += char(fx + 'a');
   	res += char(fy + '1');
   	res += char(tx + 'a');
   	res += char(ty + '1');
   
   	return res;
   }
   
   // *** End of: /home/olaf/clobber/Utils/types.h *** 
   // *** Start of: /home/olaf/clobber/Utils/bits.h *** 
   
   inline int lsb(const unsigned long long &b) {
   	return __builtin_ctzll(b);
   }
   
   inline int lsb(const unsigned int &b) {
   	return __builtin_ctz(b);
   }
   
   inline int pop_lsb(unsigned long long &b) {
   	const int s = lsb(b);
   	b &= b - 1;
   	return s;
   }
   
   inline int pop_lsb(unsigned int &b) {
   	const int s = lsb(b);
   	b &= b - 1;
   	return s;
   }
   
   inline int pop_cnt(const unsigned long long &b) {
   	return __builtin_popcountll(b);
   }
   
   inline int pop_cnt(const unsigned int &b) {
   	return __builtin_popcount(b);
   }
   
   inline void debug_bitboard(const unsigned long long &b) {
   	for(int i = 0; i < 8; i++) {
   		for(int j = 0; j < 8; j++) {
   			std::cerr << ((b & (1ULL << (i * 8 + j))) > 0);
   		}
   		std::cerr << '\n';
   	}
   }
   
   // *** End of: /home/olaf/clobber/Utils/bits.h *** 
   // *** Start of: /home/olaf/clobber/Utils/random.h *** 
   
   
   static unsigned int g_seed = 2137;
   
   inline void fast_srand(int seed) {
   	g_seed = seed;
   }
   
   inline int fast_rand() {
   	unsigned int z = (g_seed += 0x39A3689AU);
   	z = (z ^ (z >> 14)) * 0x108E8AB8;
   	z = (z ^ (z >> 13)) * 0x2CD01FD6;
   	return int((z ^ (z >> 15)) >> 1);
   }
   
   inline int fast_rand(int a, int b) {
       return a + fast_rand() % (b - a + 1);
   }
   
   inline int random_set_bit(BB b) {
       int bit = fast_rand() % pop_cnt(b);
       while(bit--) {
           b &= b - 1;
       }
       return lsb(b);
   }
   
   // *** End of: /home/olaf/clobber/Utils/random.h *** 
  
  constexpr BB attacks[64] = {
  	0x0000000000000102ULL, 0x0000000000000205ULL, 0x000000000000040aULL, 0x0000000000000814ULL, 0x0000000000001028ULL, 0x0000000000002050ULL, 0x00000000000040a0ULL, 0x0000000000008040ULL,
  	0x0000000000010201ULL, 0x0000000000020502ULL, 0x0000000000040a04ULL, 0x0000000000081408ULL, 0x0000000000102810ULL, 0x0000000000205020ULL, 0x000000000040a040ULL, 0x0000000000804080ULL,
  	0x0000000001020100ULL, 0x0000000002050200ULL, 0x00000000040a0400ULL, 0x0000000008140800ULL, 0x0000000010281000ULL, 0x0000000020502000ULL, 0x0000000040a04000ULL, 0x0000000080408000ULL,
  	0x0000000102010000ULL, 0x0000000205020000ULL, 0x000000040a040000ULL, 0x0000000814080000ULL, 0x0000001028100000ULL, 0x0000002050200000ULL, 0x00000040a0400000ULL, 0x0000008040800000ULL,
  	0x0000010201000000ULL, 0x0000020502000000ULL, 0x0000040a04000000ULL, 0x0000081408000000ULL, 0x0000102810000000ULL, 0x0000205020000000ULL, 0x000040a040000000ULL, 0x0000804080000000ULL,
  	0x0001020100000000ULL, 0x0002050200000000ULL, 0x00040a0400000000ULL, 0x0008140800000000ULL, 0x0010281000000000ULL, 0x0020502000000000ULL, 0x0040a04000000000ULL, 0x0080408000000000ULL,
  	0x0102010000000000ULL, 0x0205020000000000ULL, 0x040a040000000000ULL, 0x0814080000000000ULL, 0x1028100000000000ULL, 0x2050200000000000ULL, 0x40a0400000000000ULL, 0x8040800000000000ULL,
  	0x0201000000000000ULL, 0x0502000000000000ULL, 0x0a04000000000000ULL, 0x1408000000000000ULL, 0x2810000000000000ULL, 0x5020000000000000ULL, 0xa040000000000000ULL, 0x4080000000000000ULL,
  };
  
  constexpr Key zobrist[COLOR_NB][64] = {
      {
          0xedfc338580f87eedULL, 0x230bd0318dda3dddULL, 0x41f936d52926448dULL, 0x10c2d113d49d3d77ULL, 0x865fff9fa61f9626ULL, 0x2cfc875358f845efULL, 0x6ffc31215e42c504ULL, 0x80f60e6ea8ae90c0ULL, 
          0xfd74a61730e66c66ULL, 0xdfc6529d408dbe17ULL, 0x288f3b12738b05caULL, 0x186df39b591f12c0ULL, 0xe498104f68234ab5ULL, 0x0a6a88e451b215e8ULL, 0xfb5efa71f13e99c6ULL, 0xd2ff1f4d01a0f91cULL, 
          0x12629cd4394a970aULL, 0xfaa25da3b6dddd80ULL, 0x803f57e24c43d1adULL, 0xa675f3768c9803b3ULL, 0x76b0152a521c60caULL, 0x98cc9ceb9c6c30f5ULL, 0xe60789726773a156ULL, 0x0229b503b3ca36c7ULL, 
          0x9c69d0a3ddfb4ce5ULL, 0x86deecc84c3a1912ULL, 0x43ea387628356b6eULL, 0x13eca1fa1c4e49cbULL, 0x53b16415e20c05c7ULL, 0xd06bcb222a037fd2ULL, 0x7fbc30b9030c66c5ULL, 0x01a31641252eac0aULL, 
          0xeec05c20ea726ee7ULL, 0x5408d862787529ceULL, 0xe181ea6b38f7a774ULL, 0x0acdb4f88e3b1d73ULL, 0x201b8bc2bd5d4125ULL, 0xbe5719ae8cd6890fULL, 0xf50dde5085d046e5ULL, 0x1d14adef65bc5ee9ULL, 
          0x092fd68e11c58d45ULL, 0x68fe3eb9b11cb202ULL, 0x8941178b4250766bULL, 0xd0ffaa327247f1abULL, 0xf11d1cd3663fcadcULL, 0x541e076d57f5b0d3ULL, 0x300431472fccfad1ULL, 0x6bb8387d066eb3f9ULL, 
          0x341d21243fbc1ec2ULL, 0x4befe097ed8b2600ULL, 0x53c079b68d756868ULL, 0xf3c3ae4f8822ba40ULL, 0x2da341ee2a2db5c5ULL, 0xe7fe1d1899786c2eULL, 0x4772147a9bf7a481ULL, 0x5578963ec87cb132ULL, 
          0xbd75f518465d1bf5ULL, 0xc876f3a8b79520d0ULL, 0xdf112a0aefddd835ULL, 0x6a2cf2b00233c83eULL, 0x89605d22b0072536ULL, 0x47ea26f6a500bbefULL, 0xe9862bbaf2feb9daULL, 0x5937dd7d0b2cba98ULL, 
      }, 
      {
          0xe5dc8d8997192c49ULL, 0x54431f7ca2040dc5ULL, 0xeec8c87039e1b26aULL, 0x00bd0a8c0dad4e07ULL, 0x64bd5370639d979cULL, 0xa6d86723a184bbfeULL, 0xdd655929bb7ccd4cULL, 0xf0b5ad4a415d73b3ULL, 
          0xdf9a45c29c13a1bcULL, 0xb82e5be2d9a65f98ULL, 0x2493fe005afa716cULL, 0x160747b0fda14b5eULL, 0x0f52f869302d6679ULL, 0x71a3ba9f5cfa7764ULL, 0x23246b366dc31a36ULL, 0xe72321e2e0ba2feeULL, 
          0x2ddaf38756bb428bULL, 0x8c96f32c4fbc2c0aULL, 0x4e559f3a6d2db820ULL, 0x901ddeaa8f12e2b4ULL, 0xfd96e5ffc18ffb88ULL, 0x55b5cb18ff41cbccULL, 0x6288ea844742dfe8ULL, 0x6ed62f154853c1e6ULL, 
          0x967e1a946355826fULL, 0x6ee4bc240e64b1e0ULL, 0x93a5aad23c617480ULL, 0x4414ec507876fee9ULL, 0xff83ed87214dce6eULL, 0xe3f78d99002bca48ULL, 0xffcbcaf7cbbb81f6ULL, 0x1b256eac079acfa4ULL, 
          0x3534833b496f15e2ULL, 0x4fdca0beb360abebULL, 0x747b4e372f380830ULL, 0x50009a44a2d8bd6cULL, 0xf6c197ddb8b65ffcULL, 0x2eb72ded1a5812d4ULL, 0xfe437d4402622cc5ULL, 0xf22ed5fb875b9460ULL, 
          0xb3144b5c91ac69daULL, 0xeff53dc89c5b6dddULL, 0xe3e9b829038b9d74ULL, 0x24a722e48c530fbbULL, 0x8426194f5ade638aULL, 0x7e16ebc9b48f8d36ULL, 0xb8bd5052e1a63353ULL, 0x741c0076e3f446f3ULL, 
          0x9fb9f014d662af30ULL, 0x4fbe97b01400380aULL, 0x3336ec180b0302ddULL, 0x6f6b0a80f9b1c0acULL, 0xb93f86a5f6418955ULL, 0x95e01f0931c291f3ULL, 0xf59665b7411432ecULL, 0xa29080c21daa4b7bULL, 
          0xf6dcb93f9f8f15b9ULL, 0x9e670153f067b5b9ULL, 0xf8321144c6efb7e3ULL, 0x5315bc36d00304cfULL, 0xbf809aa5d6598f22ULL, 0xc1e84fc7cad4ca19ULL, 0x68f7a04a28052fd9ULL, 0xc4b559df3477f757ULL, 
      },
  };
  
  class State {
  private:
  
      BB pieces[COLOR_NB];
  
  /*
      Color us;
      Color op;
  */
  
      inline Color us_color() const { return pop_cnt(pieces[WHITE] | pieces[BLACK]) & 1; }
      inline Color op_color() const { return us_color() ^ 1; }
  
      #define us us_color()
      #define op op_color()
  
  //    Key hash;
  
  public:
  
      inline int score() const {
          
      }
  
      State() {
          pieces[WHITE] = 0xaa55aa55aa55aa55ULL;
          pieces[BLACK] = 0x55aa55aa55aa55aaULL;
      }
  
      inline void move(const int &from, const int &to) {
          Color a = us, b = op;
          pieces[a] &= ~(1ULL << from);
          pieces[a] |=   1ULL << to;
          pieces[b] &= ~(1ULL << to);
      }
  
      inline void move(Action action) { move(action & 63, action >> 6); }
  
      inline void undo(const int &from, const int &to) {
          Color a = op, b = us;
          pieces[a] |=   1ULL << from;
          pieces[a] &= ~(1ULL << to); 
          pieces[b] |=   1ULL << to;
      }
  
      inline void undo(const Action &action) { undo(action & 63, action >> 6); }
  
      inline bool is_end() const { return pop_cnt(get_active()) == 0; }
  
      inline Color get_player() const { return us; }
  
      inline Color get_opponent() const { return op; }
  
      inline BB get_active() const {
          const Color xd = op;
          return ((pieces[xd] << 8) |
                  (pieces[xd] >> 8) |
                 ((pieces[xd] & 0x7f7f7f7f7f7f7f7fULL) << 1) |
                 ((pieces[xd] & 0xfefefefefefefefeULL) >> 1))
                 & pieces[xd ^ 1];
      }
  
      inline Action* get_actions(Action *list) const {
          BB bb = get_active();
          while(bb) {
              const int from = pop_lsb(bb);
              BB to_mask = attacks[from] & pieces[op];
              while(to_mask) {
                  *list++ = make_action(from, pop_lsb(to_mask));
              }
          }
          return list;
      }
  
      inline Action get_random_action() const {
          const int from = random_set_bit(get_active());
          return make_action(from, random_set_bit(attacks[from] & pieces[op]));
      }
  
  	inline void debug() const {
  		for(int i = 0; i < 8; i++) {
  			std::cerr << "  +---+---+---+---+---+---+---+---+\n";
  			std::cerr << 8 - i << " +";
  			for(int j = 0; j < 8; j++) {
  				BB k = 1ULL << (i * 8 + j);
  				std::cerr << ' ';
  			    if(pieces[WHITE] & k) {
  				    std::cerr << 'w';
                  } else if(pieces[BLACK] & k) {
                      std::cerr << 'b';
  				} else {
  					std::cerr << ' ';
  				}
  				std::cerr << ' ';
  				if(j != 7)	std::cerr << "|";
  			}
  			std::cerr << "+\n";
  		}
  		std::cerr << "  +---+---+---+---+---+---+---+---+\n";
  		std::cerr << "    a   b   c   d   e   f   g   h  \n";
  	}
  
      #undef us
      #undef op
  };
  
  // *** End of: /home/olaf/clobber/State/state.h *** 
  // *** Start of: /home/olaf/clobber/Utils/timer.h *** 
  #include <chrono>
  
  class Timer {
  public:
  
      void start() {
          start_time = std::chrono::steady_clock::now();
      }
  
      inline auto get_elapsed() {
          using namespace std::chrono;
          milliseconds elapsed = duration_cast<milliseconds>(steady_clock::now() - start_time);
          return elapsed.count();
      }
  
  private:
      std::chrono::steady_clock::time_point start_time;
  };
  
  // *** End of: /home/olaf/clobber/Utils/timer.h *** 
  // *** Start of: /home/olaf/clobber/MCTSolver/node.h *** 
  
  #include <bits/stdc++.h>
  
  
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
  
  // *** End of: /home/olaf/clobber/MCTSolver/node.h *** 
 
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
 
 // *** End of: /home/olaf/clobber/MCTSolver/mctsolver.h *** 

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

// *** End of: /home/olaf/clobber/main.cpp *** 
