#pragma once

#include "base_entity.h"

class c_base_player : public c_base_entity {
public:
	/*
	* DT_BasePlayer::m_iHealth
	* Get health.
	*/
	int get_health();

	/*
	* DT_BasePlayer::m_bDucked
	* Get ducked state.
	*/
	bool is_ducked();

	/*
	* DT_BasePlayer::m_bDucking
	* Get ducking state.
	*/
	bool is_ducking();

	/*
	* DT_BasePlayer::m_bPoisoned
	* Get poisoned.
	*/
	bool is_poisoned();

	/*
	* DT_BasePlayer::m_fFlags
	* Get flags.
	*/
	int get_flags();

	/*
	* DT_BasePlayer::m_vecVelocity
	* Get velocity.
	*/
	vec3 get_velocity();
};

using c_terror_player = c_base_player;