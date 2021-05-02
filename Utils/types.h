#pragma once

#include <string>

#include <x86intrin.h>

typedef unsigned long long BB;
typedef unsigned long long Key;
typedef unsigned long long u64;

typedef int8_t Color;

const Color WHITE = 0;
const Color BLACK = 1;
const Color COLOR_NB = 2;

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
