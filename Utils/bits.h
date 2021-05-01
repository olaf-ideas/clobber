#pragma once

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
