#include "events.h"

#include "interfaces.h"
#include "fnv.h"

void game_event_manager::initialise()
{
	/* Player death */
	g_l4d2.m_game_event_manager->add_listener(this, "player_death");

	// userid(short) - user ID who died
	// entityid(long) - entity ID who died, userid should be used first, to get the dead Player.Otherwise, it is not a player, so use this.
	// attacker(short) - user ID who killed
	// attackername(string) - What type of zombie, so we don't have zombie names
	// attackerentid(long) - if killer not a player, the entindex of who killed.Again, use attacker first
	// weapon(string) - weapon name killer used
	// headshot(bool) - signals a headshot
	// attackerisbot(bool) - is the attacker a bot
	// victimname(string) - What type of zombie, so we don't have zombie names
	// victimisbot(bool) - is the victim a bot
	// abort(bool) - did the victim abort
	// type(long) - damage type
	// victim_x(float) -
	// victim_y(float) -
	// victim_z(float) -

	/* Player hurt */
	g_l4d2.m_game_event_manager->add_listener(this, "player_hurt");
	g_l4d2.m_game_event_manager->add_listener(this, "player_hurt_concise");

	// local(1) - Not networked
	// userid(short) - user ID who was hurt
	// attacker(short) - user id who attacked
	// attackerentid(long) - entity id who attacked, if attacker not a player, and userid therefore invalid
	// health(short) - remaining health points
	// armor(byte) - remaining armor points
	// weapon(string) - weapon name attacker used, if not the world
	// dmg_health(short) - damage done to health
	// dmg_armor(byte) - damage done to armor
	// hitgroup(byte) - hitgroup that was damaged
	// type(long) - damage type

	/* Player change his team */
	g_l4d2.m_game_event_manager->add_listener(this, "player_team");

	// userid(short) - user ID on server
	// team(byte) - team id
	// oldteam(byte) - old team id
	// disconnect(bool) - team change because player disconnects
	// name(string) -
	// isbot(bool) -

	/* When a Player presses +USE on a useable entity. i.e. Weapons, items, doors */
	g_l4d2.m_game_event_manager->add_listener(this, "player_use");

	// userid(short) - userid of user
	// targetid(long) - Entindex of the used entity

	/* Player say */
	g_l4d2.m_game_event_manager->add_listener(this, "player_say");

	// userid(short) -
	// text(string) -

	/* Player afk */
	g_l4d2.m_game_event_manager->add_listener(this, "player_afk");

	// player(short) - user ID of the player

	/* Player who fired the teammate */
	g_l4d2.m_game_event_manager->add_listener(this, "friendly_fire");

	// attacker(short) - player who fired the weapon
	// victim(short) - player who got shot
	// guilty(short) - player who was at fault
	// type(long) - damage type

	/* Rounds */
	g_l4d2.m_game_event_manager->add_listener(this, "round_start");

	// timelimit(long) - round time limit in seconds
	// fraglimit(long) - frag limit in seconds
	// objective(string) - round objective

	g_l4d2.m_game_event_manager->add_listener(this, "round_start_pre_entity");
	g_l4d2.m_game_event_manager->add_listener(this, "round_start_post_nav");
	g_l4d2.m_game_event_manager->add_listener(this, "round_end");

	/* Infected death */
	g_l4d2.m_game_event_manager->add_listener(this, "infected_death");

	// attacker(short) - user ID who killed
	// infected_id(short) - ID of the infected that died
	// gender(short) - gender(type) of the infected
	// weapon_id(short) - ID of the weapon used
	// headshot(bool) - signals a headshot
	// minigun(bool) - signals a minigun kill
	// blast(bool) - signals a death from blast damage
	// submerged(bool) - indicates the infected was submerged

	/* Infected hurt */
	g_l4d2.m_game_event_manager->add_listener(this, "infected_hurt");

	// local(1) - don't network this, its way too spammy
	// attacker(short) - player userid who attacked
	// entityid(long) - entity id of infected
	// hitgroup(byte) - hitgroup that was damaged
	// amount(short) - how much damage was done
	// type(long) - damage type

	/* Tank killed */
	g_l4d2.m_game_event_manager->add_listener(this, "tank_killed");

	// userid(short) - user ID of dead tank
	// attacker(short) - user id of killer
	// solo(bool) - TRUE if a player single - handedly killed the Tank
	// melee_only(bool) - TRUE if the tank was only killed by melee attacks(no blast, burn, or bullet damage)
	// l4d1_only(bool) - TRUE if l4d1 survivors inflicted damage and the l4d2 survivors did not)

	/* Witch killed */
	g_l4d2.m_game_event_manager->add_listener(this, "witch_killed");

	// userid(short) - Player who killed the witch
	// witchid(long) - Entindex of witch that was killed
	// oneshot(bool) - TRUE if the Witch was killed with one shot
	// melee_only(bool) - TRUE if the witch was only killed by melee attacks(no blast, burn, or bullet damage)
	// bride(bool) - bride variant?

	/* Jockey killed */
	g_l4d2.m_game_event_manager->add_listener(this, "jockey_killed");

	// userid(short) - user ID of dead jockey
	// attacker(short) - user id of killer
	// weapon(string) - weapon name

	/* Spitter killed */
	g_l4d2.m_game_event_manager->add_listener(this, "spitter_killed");

	// userid(short) - user ID of dead spitter
	// attacker(short) - user id of killer
	// has_spit(bool) - TRUE if the spitter spit at some point

	/* Charger killed */
	g_l4d2.m_game_event_manager->add_listener(this, "charger_killed");

	// userid(short) - user ID of dead charger
	// attacker(short) - user id of killer
	// melee(bool) - TRUE if a player killed the charger with a melee weapon
	// charging(bool) - TRUE if the charger was charging when it died

	/* Tank spawn */
	g_l4d2.m_game_event_manager->add_listener(this, "tank_spawn");

	// userid(short) - User ID of the tank spawning now
	// tankid(short) - Entindex of tank spawning right now

	/* Witch spawn */
	g_l4d2.m_game_event_manager->add_listener(this, "witch_spawn");

	// witchid(long) - Entindex of witch spawning right now

	/* Hunter headshot */
	g_l4d2.m_game_event_manager->add_listener(this, "hunter_headshot");

	// userid(short) - player who made the headshot
	// hunteruserid(long) - user ID of Hunter
	// slunging(bool) - TRUE if the Hunter was in the act of lunging

	/* Jockey headshot */
	g_l4d2.m_game_event_manager->add_listener(this, "jockey_headshot");

	// userid(short) - user ID of player that landed a headshot on the Jockey
	// jockeyuserid(long) - user ID of Jockey
	// slunging(bool) - TRUE if the Jockey was in the act of lunging

	/* Player who used the pills */
	g_l4d2.m_game_event_manager->add_listener(this, "pills_used");

	// userid(short) - person who had the pills
	// subject(short) - person swallowing the pills

	/* Player who tried to use the pills */
	g_l4d2.m_game_event_manager->add_listener(this, "pills_used_fail");

	// userid(short) - person who tried to use the pills

	/* Player who used the defibrillator */
	g_l4d2.m_game_event_manager->add_listener(this, "defibrillator_used");

	// userid(short) - person who used the defibrillator
	// subject(short) - person it helped

	/* Player who tried to use the defibrillator */
	g_l4d2.m_game_event_manager->add_listener(this, "defibrillator_used_fail");

	// userid(short) - person who tried to use the defibrillator
	// subject(short) - person it failed to help
}

void game_event_manager::fire_game_event(c_game_event* _event)
{
	switch (fnv::hash(_event->get_name())) {
	case fnv::hash_const("player_death"): {

		break;
	}
									/* ... */
	}
}

void game_event_manager::shutdown()
{
	g_l4d2.m_game_event_manager->remove_listener(this);
}