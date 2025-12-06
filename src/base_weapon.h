#pragma once

#include "base_entity.h"

struct file_weapon_info_t {
private:
	unsigned char u0[0x4];
public:
	bool m_parsed_script;
	bool m_loaded_hud_elements;
	char m_class_name[80];
	char m_print_name[80];
	char m_view_model[80];
	char m_world_model[80];
	char m_anim_prefix[16];
	short u1;
	int m_slot;
	int m_position;
	int m_maxclip1;
	int m_maxclip2;
	int m_defaultclip1;
	int m_defaultclip2;
	int m_weight;
	int m_rumble;
	bool m_auto_switch_to;
	bool m_auto_switch_from;
	int m_flags;
};

class c_base_weapon : public c_base_entity {
public:
	/*
	* GetWeaponID()
	* Get every weapon by id.
	*/
	int get_weapon_id();

	/*
	* GetWeaponData()
	* Get file_weapon_info_t structure
	*/
	file_weapon_info_t* get_weapon_data();
};