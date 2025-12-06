#pragma once

#include <cmath>
#include <algorithm>

class vec2 {
public:
	vec2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

	inline vec2 operator*(const float n) const {
		return vec2{ x * n, y * n };
	}

	inline vec2 operator+(const vec2& v) const {
		return vec2{ x + v.x, y + v.y };
	}

	inline vec2 operator-(const vec2& v) const {
		return vec2{ x - v.x, y - v.y };
	}

	inline void operator+=(const vec2& v) {
		x += v.x;
		y += v.y;
	}

	inline void operator-=(const vec2& v) {
		x -= v.x;
		y -= v.y;
	}

	bool operator==(const vec2& v) const {
		return (v.x == x && v.y == y);
	}

	bool operator!=(const vec2& v) const {
		return (v.x != x || v.y != y);
	}

	inline float length() {
		return std::sqrt((x * x) + (y * y));
	}

	float x, y;
};