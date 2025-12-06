#include "achievement_mgr.h"

#include "mem.h"

achievement_t* c_achievement_mgr::get_achievement_by_index(int index, int player_slot)
{
	return mem::call_virtual_fn<achievement_t*, 0>(this, index, player_slot);
}

achievement_t* c_achievement_mgr::get_achievement_by_id(int id, int player_slot)
{
	return mem::call_virtual_fn<achievement_t*, 3>(this, id, player_slot);
}

int c_achievement_mgr::get_achievement_count(bool unk)
{
	return mem::call_virtual_fn<int, 4>(this, unk);
}

int c_achievement_mgr::get_achievement_total()
{
	return mem::call_virtual_fn<int, 5>(this);
}

/*
 * All achievements you unlock will not be saved to your Steam profile.
 * To save them in Steam, you need to use the SteamUserStats interface.
 *
 * Before calling award_achievement(), call SteamUserStats->RequestCurrentStats();
 * after calling award_achievement(), call SteamUserStats->StoreStats()
 * and then SteamUserStats->RequestCurrentStats() again.
 *
 * SteamUserStats->RequestCurrentStats();
 * for (int i = 0; i < achievement_mgr->get_achievement_count(); i++)
		achievement_mgr->award_achievement(achievement_mgr->get_achievement_by_index(i)->get_achievement_id());
 * SteamUserStats->StoreStats();
 * SteamUserStats->RequestCurrentStats();
 *
 * This way, you'll unlock all achievements not only in the game but also in your Steam profile.
*/
void c_achievement_mgr::award_achievement(int id, int player_slot)
{
	return mem::call_virtual_fn<void, 6>(this, id, player_slot);
}