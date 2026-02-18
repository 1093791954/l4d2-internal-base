#include "base_entity.h"

#include "mem.h"
#include "netvars.h"
#include "matrix.h"

c_client_renderable* c_base_entity::get_renderable()
{
	return reinterpret_cast<c_client_renderable*>(uintptr_t(this) + 0x4);
}

c_client_networkable* c_base_entity::get_networkable()
{
	return reinterpret_cast<c_client_networkable*>(uintptr_t(this) + 0x8);
}

client_class_t* c_base_entity::get_client_class()
{
	return mem::call_virtual_fn<client_class_t*, 1>(get_networkable());
}

int c_base_entity::get_team()
{
	return mem::read<int>(reinterpret_cast<uintptr_t>(this) +
		netvars::get_netvar_address("DT_BaseEntity::m_iTeamNum"));
}

vec3 c_base_entity::get_origin()
{
	return mem::read<vec3>(reinterpret_cast<uintptr_t>(this) +
		netvars::get_netvar_address("DT_BaseEntity::m_vecOrigin"));
}

vec3 c_base_entity::get_abs_origin()
{
	return mem::call_virtual_fn<vec3&, 10>(this);
}

bool c_base_entity::get_dormant()
{
	return mem::call_virtual_fn<bool, 7>(get_networkable());
}

int c_base_entity::get_move_type()
{
	return mem::read<int>(reinterpret_cast<uintptr_t>(this) + 0x144);
}

bool c_base_entity::setup_bones(matrix3x4_t* bone_matrix, int max_bones, int bone_mask, float curtime)
{
	auto renderable = get_renderable();
	if (!renderable)
		return false;

	return mem::call_virtual_fn<bool, 13>(renderable, bone_matrix, max_bones, bone_mask, curtime);
}

vec3 c_base_entity::get_bone_position(int bone_index)
{
	matrix3x4_t bone_matrix[128];
	if (!setup_bones(bone_matrix, 128, 0x100, 0))
		return vec3(0, 0, 0);

	return vec3(
		bone_matrix[bone_index].m[0][3],
		bone_matrix[bone_index].m[1][3],
		bone_matrix[bone_index].m[2][3]
	);
}