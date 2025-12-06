#include "base_entity.h"

#include "mem.h"
#include "netvars.h"

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