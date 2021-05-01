#ifndef LOCAL
#pragma GCC optimize("Ofast","unroll-loops","omit-frame-pointer","inline")
#pragma GCC option("arch=native","tune=native","no-zero-upper")
#pragma GCC target("avx")
#endif

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

struct State {
	
	BB pieces[COLOR_NB];
	
	Key hash = 0ULL;
	
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
	
	// doesn't check if any move exist
	inline Action random_move(Color col) const {
		BB bb = get_active(col);
		
		// assert(bb); // valid move must exist
		
		int from = random_set_bit(bb);
		
		return make_action(from, random_set_bit(attacks[from] & pieces[col ^ 1]));
	}
	
	inline bool is_end(Color col) const {
		return pop_cnt(get_active(col)) == 0;
	}
};

namespace mcts {
	
	class Node {
	private:
		State state;
		Color color;
		Node* parent;
		vector<pair<Action, Node*>> children;
		vector<Action> untried;
		
		int vis_cnt;
		int win_cnt;
		
		float value;
		
	public:
		
		Node(State _state, Color _color, Node *_parent) : state(_state), color(_color), parent(_parent),
														  vis_cnt(0), win_cnt(0) {
			static Action actions[112];
			Action* end = state.gen_moves(_color, actions);
			
			shuffle(actions, end, rng);
			
			for(Action *now = actions; now != end; now++) {
				untried.push_back(*now);
			}
			
			value = numeric_limits<float>::max();
		}
		
		inline bool is_fully_expanded() const {
			return untried.empty();
		}
		
		inline bool is_terminal() const {
			return children.empty() && untried.empty();
		}
		
		inline float get_value() const {
			return value;
		}
		
		inline int get_vis_cnt() const {
			return vis_cnt;
		}
		
		inline State get_state() const {
			return state;
		}
		
		inline void set_parent(Node *new_parent) {
			parent = new_parent;
		}
		
		inline void update_value() {
			value = get_ucb();
		}
		
		Node* select() {
			Node* leaf_node = this;
			
			while(!leaf_node->is_terminal()) {
				if(leaf_node->is_fully_expanded()) {
					leaf_node = leaf_node->select_best_child();
				} else {
					return leaf_node->expand();
				}
			}
			
			return leaf_node;
		}
		
		Node* expand() {
			Action action = untried.back();
			untried.pop_back();
			
			state.move(action, color);
			
			Node* child_node = new Node(state, Color(color ^ 1), this);
			
			children.push_back(make_pair(action, child_node));
			
			state.undo(action, color);
			
			return child_node;
		}
		
		Color simulate() {
			State now = state;
			Color col = color;
			
			//int sim_len = 0;
			
			while(!now.is_end(color)) {
				now.move(now.random_move(col), col);
				col = Color(col ^ 1);
				//sim_len++;
			}
			
			//cerr << sim_len << '\n';
			
			return col;
		}
		
		Node* select_best_child() {
			
			float best_value = -numeric_limits<float>::max();
			Node* best_node = nullptr;
			
			for(pair<Action, Node*> &child : children) {
				child.second->update_value();
				if(child.second->get_value() > best_value) {
					best_node = child.second;
					best_value = child.second->get_value();
				}
			}
			
			assert(!children.empty());
			assert(best_node != nullptr);
			return best_node;
		}
		
		inline void backpropagate(Color loser) {
			vis_cnt++;
			if(color != loser) {
				win_cnt++;
			}
			
			if(parent) {
				parent->backpropagate(loser);
				value = get_ucb();
			}
		}
		
		inline float win_parent_rate() const {
			if(vis_cnt == 0)	return 0.5f;
			return float(vis_cnt - win_cnt) / vis_cnt;
		}
		
		inline float win_rate() const {
			if(vis_cnt == 0)	return 0.5f;
			return float(win_cnt) / vis_cnt;
		}
		
		// winrate of parent + c * sqrt(log(# of parent games) / # of my games)
		inline float get_ucb() const {
			if(vis_cnt == 0)	return numeric_limits<float>::max();
			return win_parent_rate() + sqrt(2) * sqrt(log(parent->vis_cnt) / (vis_cnt + 0.0001f));
		}
		
		Action get_best_action() const {
			int best_vis_cnt = -1;
			Action best_action = -1;
			for(const pair<Action, Node*> &child : children) {
				if(child.second->get_vis_cnt() > best_vis_cnt) {
					best_vis_cnt = child.second->get_vis_cnt();
					best_action = child.first;
				}
			}
			
			assert(best_action != -1);
			return best_action;
		}
	
		Node* get_child(Action action) const {
			for(const pair<Action, Node*> &child : children) {
				if(child.first == action) {
					return child.second;
				}
			}
			assert(false);
			return nullptr;
		}
	};
	
	Node* root = new Node(State(), WHITE, nullptr);
	
	float get_win_rate() {
		return root->win_rate() * 100.f;
	}
	
	Action get_best_action(float time_limit) {
		
		clock_t start = clock();
		
		int iterations = 0;
		
		do {
			
			iterations++;
			
			Node* node = root->select();
			
			// node->get_state().debug();
			
			Color loser = node->simulate();
			
			node->backpropagate(loser);
			
			#ifdef LOCAL
			if(iterations % 10001 == 0) {
				cerr << "winrate: " << get_win_rate() << '\n';
				cerr << "best move: " << to_string(root->get_best_action()) << '\n';
			}
			#endif
			
		} while(clock() - start < CLOCKS_PER_SEC * time_limit * 0.98);
		
		cerr << "iterations: " << iterations << '\n';
		
		return root->get_best_action();
	}
	
	void move(Action action) {
		
		while(!root->is_fully_expanded()) {
			root->expand();
		}
		
		root = root->get_child(action);
		
		root->set_parent(nullptr);
	}
	
} // using namespace mcts;

int main() {
	init_zobrist();

	int boardSize; 
	cin >> boardSize; cin.ignore();

	string color;  
	cin >> color; cin.ignore();

	my_color = (color == "w" ? WHITE : BLACK);
	op_color = (color == "b" ? WHITE : BLACK);

	float time_limit = 100.f;

	for(int turn = 0; ; turn += 2) {
		
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
			mcts::move(from_string(lastAction));
		}
		
		Action best_action = mcts::get_best_action(time_limit);
		
		cerr << "win rate: " << mcts::get_win_rate() << '%' << '\n';
		cout << to_string(best_action) << ' ' << setprecision(6) << fixed << mcts::get_win_rate() << '%' << '\n';
		
		time_limit = 0.1f;
		
		mcts::move(best_action);
		
	}
}
