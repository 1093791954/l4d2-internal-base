#pragma once

// 注意：不要在这里包含 Windows.h 或标准库头文件
// pch.h 会通过 CMake 的 /FI 选项强制包含到所有编译单元

#include "vec2.h"
#include <cmath>

// 简单的 clamp 实现，避免 std::clamp 问题
template<typename T>
inline T clamp_val(T value, T min_val, T max_val) {
	return value < min_val ? min_val : (value > max_val ? max_val : value);
}

class vec3 {
public:
	vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}

	inline void init(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
	}

	inline void clamp() {
		x = clamp_val(x, -89.0f, 89.0f);
		y = clamp_val(remainderf(y, 360.0f), -180.0f, 180.0f);
		z = clamp_val(z, -50.0f, 50.0f);
	}

	float distance_to(const vec3& other) {
		vec3 delta{};

		delta.x = x - other.x;
		delta.y = y - other.y;
		delta.z = z - other.z;

		return delta.length();
	}

	inline void normalize() {
		x = isfinite(x) ? remainderf(x, 360.0f) : 0.0f;
		y = isfinite(y) ? remainderf(y, 360.0f) : 0.0f;
		z = 0.0f;
	}

	inline float length()
	{
		return sqrtf(length_sqr());
	}

	inline float length_2d() {
		return sqrtf(x * x + y * y);
	}

	inline float length_sqr() {
		return { x * x + y * y + z * z };
	}

	inline float length_2d_sqr() {
		return { x * x + y * y };
	}

	inline float dot(const vec3 other) {
		return { x * other.x + y * other.y + z * other.z };
	}

	inline float dot(const float* other) {
		const vec3& a = *this;
		return { a.x * other[0] + a.y * other[1] + a.z * other[2] };
	}

	vec3& operator+=(const vec3& v) {
		x += v.x;
		y += v.y;
		z += v.z;

		return *this;
	}

	vec3& operator-=(const vec3& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return *this;
	}

	vec3& operator*=(float v) {
		x *= v;
		y *= v;
		z *= v;

		return *this;
	}

	vec3 operator+(const vec3& v) {
		return vec3{ x + v.x, y + v.y, z + v.z };
	}

	vec3 operator-(const vec3& v) {
		return vec3{ x - v.x, y - v.y, z - v.z };
	}

	auto operator-(const vec3& other) const -> vec3 {
		vec3 vec{ *this };

		vec.x -= other.x;
		vec.y -= other.y;
		vec.z -= other.z;

		return vec;
	}

	vec3 operator*(float fl) const {
		return vec3{ x * fl, y * fl, z * fl };
	}

	vec3 operator*(const vec3& v) const {
		return vec3{ x * v.x, y * v.y, z * v.z };
	}

	vec3& operator/=(float fl) {
		x /= fl;
		y /= fl;
		z /= fl;

		return *this;
	}

	vec3 operator/(float other) const {
		vec3 vec{};

		vec.x = x / other;
		vec.y = y / other;
		vec.z = z / other;

		return vec;
	}

	vec3 operator+(const vec3& v) const {
		return vec3{ x + v.x, y + v.y, z + v.z };
	}

	float& operator[](int i) {
		return ((float*)this)[i];
	}

	float operator[](int i) const {
		return ((float*)this)[i];
	}

	float x, y, z;
};

class __declspec(align(16)) vec_aligned : public vec3 {
public:
	inline vec_aligned(void) {};

	inline vec_aligned(float x, float y, float z) {
		init(x, y, z);
	}

	explicit vec_aligned(const vec3& othr) {
		init(othr.x, othr.y, othr.z);
	}

	vec_aligned& operator=(const vec3& oth) {
		init(oth.x, oth.y, oth.z);
		return *this;
	}

	float w;
};