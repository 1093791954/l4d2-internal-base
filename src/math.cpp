#include "math.h"

#include "globals.h"
#include "sdk.h"

static bool screen_transform(const vec3& in, vec3& out)
{
	auto& matrix = g_l4d2.m_engine->get_matrix();

	out.x = matrix.m[0][0] * in.x + matrix.m[0][1] * in.y + matrix.m[0][2] * in.z + matrix.m[0][3];
	out.y = matrix.m[1][0] * in.x + matrix.m[1][1] * in.y + matrix.m[1][2] * in.z + matrix.m[1][3];
	out.z = 0.0f;

	float w = matrix.m[3][0] * in.x + matrix.m[3][1] * in.y + matrix.m[3][2] * in.z + matrix.m[3][3];

	if (w < 0.001f)
	{
		out.x *= 100000;
		out.y *= 100000;

		return false;
	}

	out.x /= w;
	out.y /= w;

	return true;
}

bool math::w2s(const vec3& world, vec3& screen)
{
	if (!screen_transform(world, screen))
		return false;

	screen.x = (g::screen_width / 2.0f) + (screen.x * g::screen_width) / 2.0f;
	screen.y = (g::screen_height / 2.0f) - (screen.y * g::screen_height) / 2.0f;

	return true;
}

bool math::get_player_bbox(c_base_player* player, box& in)
{
	vec3 top, down = player->get_origin(), s[2]{};
	top = down + vec3(0, 0, 72);

	if (!w2s(top, s[1]) || !w2s(down, s[0]))
		return false;

	vec3 delta{ s[1] - s[0] };

	in.h = std::fabsf(delta.y);
	in.w = in.h / 2.0f;

	in.x = s[1].x - (in.w / 2);
	in.y = s[1].y;

	return true;
}

void math::correct_movement(user_cmd_t* cmd, vec3 old_angles, float old_forwardmove, float old_sidemove)
{
	float delta_view;
	float f1;
	float f2;

	if (old_angles.y < 0.0f)
		f1 = 360.0f + old_angles.y;
	else
		f1 = old_angles.y;

	if (cmd->viewangles.y < 0.0f)
		f2 = 360.0f + cmd->viewangles.y;
	else
		f2 = cmd->viewangles.y;

	if (f2 < f1)
		delta_view = abs(f2 - f1);
	else
		delta_view = 360.0f - abs(f1 - f2);

	delta_view = 360.0f - delta_view;

	cmd->forwardmove = std::cos(DEG2RAD(delta_view)) * old_forwardmove + std::cos(DEG2RAD(delta_view + 90.f)) * old_sidemove;
	cmd->sidemove = std::sin(DEG2RAD(delta_view)) * old_forwardmove + std::sin(DEG2RAD(delta_view + 90.f)) * old_sidemove;
}