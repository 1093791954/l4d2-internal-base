#include "events.h"

#include "interfaces.h"
#include "fnv.h"

void game_event_manager::initialise()
{
	/* Player death */
	g_l4d2.m_game_event_manager->add_listener(this, "player_death");

	/* Player hurt */
	g_l4d2.m_game_event_manager->add_listener(this, "player_hurt");

	/* Player change his team */
	g_l4d2.m_game_event_manager->add_listener(this, "player_team");

	/* When a Player presses +USE on a useable entity. i.e. Weapons, items, doors */
	g_l4d2.m_game_event_manager->add_listener(this, "player_use");

	/* Player say */
	g_l4d2.m_game_event_manager->add_listener(this, "player_say");

	/* Player who fired the teammate */
	g_l4d2.m_game_event_manager->add_listener(this, "friendly_fire");

	/* Rounds */
	g_l4d2.m_game_event_manager->add_listener(this, "round_start");
	g_l4d2.m_game_event_manager->add_listener(this, "round_start_pre_entity");
	g_l4d2.m_game_event_manager->add_listener(this, "round_start_post_nav");
	g_l4d2.m_game_event_manager->add_listener(this, "round_end");

	/* Infected death */
	g_l4d2.m_game_event_manager->add_listener(this, "infected_death");

	/* Infected hurt */
	g_l4d2.m_game_event_manager->add_listener(this, "infected_hurt");

	/* Boss killed (not all) */
	g_l4d2.m_game_event_manager->add_listener(this, "tank_killed");
	g_l4d2.m_game_event_manager->add_listener(this, "witch_killed");
	g_l4d2.m_game_event_manager->add_listener(this, "jockey_killed");
	g_l4d2.m_game_event_manager->add_listener(this, "spitter_killed");
	g_l4d2.m_game_event_manager->add_listener(this, "charger_killed");

	/* Boss spawn (not all) */
	g_l4d2.m_game_event_manager->add_listener(this, "tank_spawn");
	g_l4d2.m_game_event_manager->add_listener(this, "witch_spawn");
}

void game_event_manager::fire_game_event(c_game_event* _event)
{
	switch (fnv::hash(_event->get_name())) {
	case fnv::hash("player_death"): {

		break;
	}
									/* ... */
	}
}

void game_event_manager::shutdown()
{
	g_l4d2.m_game_event_manager->remove_listener(this);
}