#include "base_weapon.h"

#include "mem.h"
#include "signatures.h"

int c_base_weapon::get_weapon_id()
{
	return mem::read<int>(reinterpret_cast<uintptr_t>(this) + 0x958);
}

file_weapon_info_t* c_base_weapon::get_weapon_data()
{
	return reinterpret_cast<file_weapon_info_t*(__thiscall*)(c_base_weapon*)>(g_signatures.get_sig(S_WEAPON_DATA_PTR))(this);
}