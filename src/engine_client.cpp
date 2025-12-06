#include "engine_client.h"

#include "mem.h"

bool c_engine_client::is_in_game()
{
	return mem::call_virtual_fn<bool, 26>(this);
}

bool c_engine_client::is_connected()
{
	return mem::call_virtual_fn<bool, 27>(this);
}

int c_engine_client::get_local_player()
{
	return mem::call_virtual_fn<int, 12>(this);
}

int c_engine_client::get_player_by_userid(int id)
{
	return mem::call_virtual_fn<int, 9>(this, id);
}

bool c_engine_client::get_player_info(int id, player_info_t* info)
{
	return mem::call_virtual_fn<bool, 8>(this, id, info);
}

matrix3x4_t& c_engine_client::get_matrix()
{
	return mem::call_virtual_fn<matrix3x4_t&, 37>(this);
}

bool c_engine_client::is_game_paused()
{
	return mem::call_virtual_fn<bool, 87>(this);
}

std::uint32_t c_engine_client::get_engine_build_number()
{
	return mem::call_virtual_fn<std::uint32_t, 99>(this);
}

void c_engine_client::execute_client_cmd(const char* cmd)
{
	return mem::call_virtual_fn<void, 103>(this, cmd);
}

c_achievement_mgr* c_engine_client::get_achievement_mgr()
{
	return mem::call_virtual_fn<c_achievement_mgr*, 114>(this);
}