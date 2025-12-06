#pragma once

#include "mem.h"

enum _achievement_flags {
	ACH_LISTEN_KILL_EVENTS = 0x0001,
	ACH_LISTEN_MAP_EVENTS = 0x0002,
	ACH_LISTEN_COMPONENT_EVENTS = 0x0004,
	ACH_HAS_COMPONENTS = 0x0020,
	ACH_SAVE_WITH_GAME = 0x0040,
	ACH_SAVE_GLOBAL = 0x0080,
	ACH_FILTER_ATTACKER_IS_PLAYER = 0x0100,
	ACH_FILTER_VICTIM_IS_PLAYER_ENEMY = 0x0200,
	ACH_FILTER_FULL_ROUND_ONLY = 0x0400
};

struct achievement_t {
	/*
	* IAchievement::GetAchievementID()
	* Get achievement id.
	*/
	int get_achievement_id() {
		return mem::call_virtual_fn<int, 0>(this);
	}

	/*
	* IAchievement::GetName()
	* Get achievement name.
	*/
	const char* get_name() {
		return mem::call_virtual_fn<const char*, 1>(this);
	}

	/*
	* IAchievement::GetFlags()
	* Get achievement flags.
	*/
	int get_flags() {
		return mem::call_virtual_fn<int, 2>(this);
	}

	/*
	* IAchievement::GetGoal()
	* Get achievement goal.
	*/
	int get_goal() {
		return mem::call_virtual_fn<int, 3>(this);
	}

	/*
	* IAchievement::GetCount()
	* Get achievement count.
	*/
	int get_count() {
		return mem::call_virtual_fn<int, 4>(this);
	}

	/*
	* IAchievement::IsAchieved()
	* Check if current achievement is unlocked.
	*/
	bool is_achieved() {
		return mem::call_virtual_fn<bool, 5>(this);
	}

	/*
	* IAchievement::GetPointValue()
	* Get achievement point value.
	*/
	int get_point_value() {
		return mem::call_virtual_fn<int, 6>(this);
	}

	/*
	* IAchievement::ShouldSaveWithGame()
	*
	*/
	bool should_save_with_game() {
		return mem::call_virtual_fn<bool, 7>(this);
	}

	/*
	* IAchievement::ShouldHideUntilAchieved()
	*
	*/
	bool should_hide_until_achieved() {
		return mem::call_virtual_fn<bool, 8>(this);
	}
};