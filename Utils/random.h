#pragma once

#include "types.h"
#include "bits.h"

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
