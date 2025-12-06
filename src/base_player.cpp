#include "base_player.h"

#include "mem.h"
#include "netvars.h"
#include "interfaces.h"

int c_base_player::get_health()
{
	return mem::read<int>(reinterpret_cast<uintptr_t>(this) +
		netvars::get_netvar_address("DT_BasePlayer::m_iHealth"));
}

bool c_base_player::is_ducked()
{
	return mem::read<bool>(reinterpret_cast<uintptr_t>(this) +
		netvars::get_netvar_address("DT_BasePlayer::m_bDucked"));
}

bool c_base_player::is_ducking()
{
	return mem::read<bool>(reinterpret_cast<uintptr_t>(this) +
		netvars::get_netvar_address("DT_BasePlayer::m_bDucking"));
}

bool c_base_player::is_poisoned()
{
	return mem::read<bool>(reinterpret_cast<uintptr_t>(this) +
		netvars::get_netvar_address("DT_BasePlayer::m_bPoisoned"));
}

int c_base_player::get_flags()
{
	return mem::read<int>(reinterpret_cast<uintptr_t>(this) +
		netvars::get_netvar_address("DT_BasePlayer::m_fFlags"));
}

vec3 c_base_player::get_velocity()
{
	return mem::read<vec3>(reinterpret_cast<uintptr_t>(this) +
		netvars::get_netvar_address("DT_BasePlayer::m_vecVelocity[0]"));
}