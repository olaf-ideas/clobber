#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <omp.h>
#include <queue>
#include <thread>
#include <csignal>

#include "Utils/timer.h"
#include "Utils/types.h"
#include "State/state.h"

using namespace std;
using namespace chrono;

class DataTrader {
	
private:
	
	int pid;
	
	int inp_pp;
	int out_pp;
	int err_pp;
	
	int time_limit;
	
public:
	
	Timer timer;
	
	~DataTrader() {
		close(inp_pp);
		close(out_pp);
		close(err_pp);
		kill(pid, SIGTERM);
		int status;
		waitpid(pid, &status, 0);
		if(!WIFEXITED(status)) {
			kill(pid, SIGKILL);
		}
	}
	
	DataTrader(const char *name) {
		#define READ  0
		#define WRITE 1
		
		int stdinp_pp[2];
		int stdout_pp[2];
		int stderr_pp[2];
		
		pipe(stdinp_pp);
		pipe(stdout_pp);
		pipe(stderr_pp);
		
		pid = fork();
		
		if(pid == 0) {
			dup2(stdinp_pp[READ],  STDIN_FILENO);
			dup2(stdout_pp[WRITE], STDOUT_FILENO);
			dup2(stderr_pp[WRITE], STDERR_FILENO);
			
			close(stdinp_pp[READ]), close(stdinp_pp[WRITE]);
			close(stdout_pp[READ]), close(stdout_pp[WRITE]);
			close(stderr_pp[READ]), close(stderr_pp[WRITE]);
			
			execl(name, name, (char*) NULL);
		}
		
		close(stdinp_pp[READ]);
		close(stdout_pp[WRITE]);
		close(stderr_pp[WRITE]);
		
		inp_pp = stdinp_pp[WRITE];
		out_pp = stdout_pp[READ];
		err_pp = stderr_pp[READ];
		
		this_thread::sleep_for(milliseconds(10));
		
		#undef READ
		#undef WRITE
	}
	
	friend DataTrader& operator<< (DataTrader &trader, const string &input) {
		if(write(trader.inp_pp, &input[0], input.size()) != (int) input.size()) {
			throw(5);
		}
		return trader;
	}
	
	DataTrader& operator() (int new_time_limit) {
		time_limit = new_time_limit;
		timer.start();
		return *this;
	}
	
	friend DataTrader& operator>> (DataTrader &trader, string &response) {
		pollfd outpool{trader.out_pp, POLLIN};
		response.clear();
		while(trader.timer.get_elapsed() <= trader.time_limit) {
			if(poll(&outpool, 1, trader.time_limit - trader.timer.get_elapsed())) {
				int bytes;
				if(ioctl(trader.out_pp, FIONREAD, &bytes) < 0)	throw(4);
				string out; out.resize(bytes);
				
				if(read(trader.out_pp, &out[0], bytes) < 0)	throw(4);
				response += out;
			}
			
			if(response.size() && response.back() == '\n') {
				response.pop_back();
				return trader;
			}
		}
		
		throw(1); // timeout
		
		return trader;
	}
};

/* Usage: 
	DataTrader trader("program_name");
	trader << input << "\n";
	trader(time_limit_in_milliseconds) >> output;
	
	eg.
	
	DataTrader trader("main");
	trader << "test" << "\n";
	string action;
	trader(1000) >> action;
*/

bool valid_action(string &action) {
	if(action.size() < 4)	return false;
	
	while(action.size() > 4)	action.pop_back();
	
	if(action[0] >= 'a' && action[0] <= 'h' &&
	   action[1] >= '1' && action[1] <= '8' &&
	   action[2] >= 'a' && action[2] <= 'h' &&
	   action[3] >= '1' && action[3] <= '8') {
		return true;
	}
	
	return false;
}

int main(int argc, char**argv) {
	
	if(argc != 3) {
		printf("Usage: %s <bot_name> <bot_name>\n", argv[0]);
		return -1;
	}
	
	signal(SIGPIPE, SIG_IGN);
	
	State state;
	//							WHITE				BLACK			
	DataTrader bots[2] = {DataTrader(argv[1]), DataTrader(argv[2])};
	
	string last_action = "null";
	
	for(int turn = 0; !state.is_end(); turn++) {
		std::cerr << "turn: " << turn << '\n';
		state.debug();
		std::cerr << '\n';
		
		string board = state.get_board();
		
		stringstream ss;
		
		if(turn == 0) {
			ss << "8\n";
			ss << "w\n";
		}
		
		if(turn == 1) {
			ss << "8\n";
			ss << "b\n";
		}
		
		ss << board << '\n';
		ss << last_action << '\n';
		ss << state.action_count() << '\n';
		
		//std::cerr << ss.str() << '\n';
		
		bots[turn & 1] << ss.str();
		
		string action;
		
		bots[turn & 1](turn < 2 ? 1100 : 110) >> action;
		
		//std::cerr << "action: " << '"' << action << '"' << '\n';
		
		if(!valid_action(action) || !state.check(from_string(action))) {
			assert(false);
			break;
		}
		
		last_action = action;
		
		state.move(from_string(action));
	}
	
	cout << (state.get_player() == WHITE ? 0 : 1) << '\n';
}
