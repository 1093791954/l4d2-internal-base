#pragma once

#define ENGINE_CLIENT_INTERFACE_VERSION "VEngineClient013"

#include "player_info.h"
#include "matrix.h"
#include "achievement_mgr.h"

class c_engine_client {
public:
	/*
	* IEngineClient::IsInGame()
	* Check if player is playing.
	*/
	bool is_in_game();

	/*
	* IEngineClient::IsConnected()
	* Check if player is connected to the map.
	*/
	bool is_connected();

	/*
	* IEngineClient::GetLocalPlayer()
	* Get local player index.
	*/
	int get_local_player();

	/*
	* IEngineClient::GetPlayerForUserID(int)
	* Get player by userid.
	*/
	int get_player_by_userid(int id);

	/*
	* IEngineClient::GetPlayerInfo(int, player_info_t)
	* Get player info structure.
	*/
	bool get_player_info(int id, player_info_t* info);

	/*
	* IEngineClient::WorldToScreenMatrix()
	* Get world to screen matrix.
	*/
	matrix3x4_t& get_matrix();

	/*
	* IEngineClient::IsGamePaused()
	* Check if game paused.
	*/
	bool is_game_paused();

	/*
	* IEngineClient::GetEngineBuildNumber()
	* Get engine build number (game version).
	*/
	std::uint32_t get_engine_build_number();

	/*
	* IEngineClient::ExecuteClientCmd(string)
	* Execute cmd.
	*/
	void execute_client_cmd(const char* cmd);

	/*
	* IEngineClient::GetAchievementMgr()
	* Get achievement manager.
	*/
	c_achievement_mgr* get_achievement_mgr();
};