#pragma once

// 注意：不要在这里���含 Windows.h 或标准库头文件
// pch.h 会通过 CMake 的 /FI 选项强制包含到所有编译单元
// 如果需要数学函数，使用 <cmath>（已在 pch.h 中包含）

#include <cmath>

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
		return sqrtf((x * x) + (y * y));
	}

	float x, y;
};