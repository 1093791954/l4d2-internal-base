#pragma once

#include "matrix.h"
#include "user_cmd.h"

class c_base_player;

struct box {
	int x, y, w, h;
	box() = default;
	box(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {};
};

namespace math
{
	constexpr double M_PI = 3.14159265358979323846;
	constexpr float  M_PI_F = static_cast<float>(M_PI);

	template <typename T>
	constexpr float DEG2RAD(T x) {
		return static_cast<float>(x) * (M_PI_F / 180.f);
	}

	template <typename T>
	constexpr float RAD2DEG(T x) {
		return static_cast<float>(x) * (180.f / M_PI_F);
	}

	bool w2s(const vec3& in, vec3& out);
	bool get_player_bbox(c_base_player* player, box& in);
	void correct_movement(user_cmd_t* cmd, vec3 old_angles, float old_forwardmove, float old_sidemove);
}