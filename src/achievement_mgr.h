#pragma once

#include "achievement.h"

#define ACHIEVEMENTMGR_INTERFACE_VERSION "ACHIEVEMENTMGR_INTERFACE_VERSION001"

class c_achievement_mgr {
public:
	/*
	* IAchievementMgr::GetAchievementByIndex(int)
	* Get achievement by index.
	*/
	achievement_t* get_achievement_by_index(int index, int player_slot = 0);

	/*
	* IAchievementMgr::GetAchievementByID(int)
	* Get achievement by id.
	*/
	achievement_t* get_achievement_by_id(int id, int player_slot = 0);

	/*
	* IAchievementMgr::GetAchievementCount()
	* Get achievement count.
	*/
	int get_achievement_count(bool unk = false);

	/*
	* IAchievementMgr::GetAchievementTotal()
	* Get achievement total.
	*/
	int get_achievement_total();

	/*
	* IAchievementMgr::AwardAchievement(int)
	* Unlock achievement by id.
	*/
	void award_achievement(int id, int player_slot = 0);
};