#include "player_info_manager.h"

#include "mem.h"

c_global_vars_base* c_player_info_manager::get_globals()
{
	return mem::call_virtual_fn<c_global_vars_base*, 1>(this);
}