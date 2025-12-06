#pragma once

#include "client_class.h"
#include "vec3.h"

enum {
	MOVETYPE_NONE,
	MOVETYPE_ISOMETRIC,
	MOVETYPE_WALK,
	MOVETYPE_STEP,
	MOVETYPE_FLY,
	MOVETYPE_FLYGRAVITY,
	MOVETYPE_VPHYSICS,
	MOVETYPE_PUSH,
	MOVETYPE_NOCLIP,
	MOVETYPE_LADDER,
	MOVETYPE_OBSERVER,
	MOVETYPE_CUSTOM,
	MOVETYPE_LAST = MOVETYPE_CUSTOM,
	MOVETYPE_MAX_BITS = 4,
};

class c_client_renderable;
class c_client_networkable;

class c_base_entity {
public:
	c_client_renderable* get_renderable();
	c_client_networkable* get_networkable();

	/*
	* IClientNetworkable::GetClientClass()
	* Get client class.
	*/
	client_class_t* get_client_class();

	/*
	* DT_BaseEntity::m_iTeamNum
	* Get entity team.
	*/
	int get_team();

	/*
	* DT_BaseEntity::m_vecOrigin
	* Get entity vec origin.
	*/
	vec3 get_origin();

	/*
	* IClientEntity::GetAbsOrigin()
	* Get entity absolute origin.
	*/
	vec3 get_abs_origin();

	/*
	* IClientNetworkable::GetDormant()
	* Get entity absolute origin.
	*/
	bool get_dormant();

	/*
	* IClientEntity::GetMoveType()
	* Get entity move type.
	*/
	int get_move_type();
};