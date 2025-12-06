#pragma once

#define PLAYER_INFO_MANAGER_INTERFACE_VERSION "PlayerInfoManager002"

#include "global_vars_base.h"

class c_player_info_manager {
public:
	/*
	* IPlayerInfoManager::GetGlobalVars()
	* Get global vars base.
	*/
	c_global_vars_base* get_globals();
};