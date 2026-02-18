#include "vars.h"

void vars::initialise()
{
	/* note @xastrix:
         before using g_vars.get_as<Type>("variable") anywhere,
	                  create this key here - set("variable", 0) */

	set("visuals->players->box", false);
	set("visuals->players->box->type", 0);
	set("visuals->players->box->col->r", 242);
	set("visuals->players->box->col->g", 121);
	set("visuals->players->box->col->b", 121);

	set("visuals->players->health", false);
	set("visuals->players->health->type", 1);
	set("visuals->players->health->col->r", 242);
	set("visuals->players->health->col->g", 121);
	set("visuals->players->health->col->b", 121);

	set("visuals->spawn_objects", false);
	set("visuals->spawn_objects->type", 1);
	set("visuals->spawn_objects->distance", 700.0f);
	set("visuals->spawn_objects->col->r", 50);
	set("visuals->spawn_objects->col->g", 210);
	set("visuals->spawn_objects->col->b", 0);

	set("visuals->special_infected", false);
	set("visuals->special_infected->box->type", 0);
	set("visuals->special_infected->col->r", 210);
	set("visuals->special_infected->col->g", 50);
	set("visuals->special_infected->col->b", 0);

	set("misc->autopistol", false);
	set("misc->bhop", false);
	set("misc->autostrafe", false);
}

void vars::set(const std::string& key, const varType& value)
{
	for (auto& v : m_map)
	{
		if (v.first == key)
		{
			if (v.second != value)
				v.second = std::move(value);

			return;
		}
	}

	m_map.emplace_back(std::move(key), std::move(value));
}

vars_t vars::get_map()
{
	return m_map;
}

void vars::shutdown()
{
	m_map.clear();
}