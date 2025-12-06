#pragma once

#include "sdk.h"

struct visuals {
	void render();

private:
	void draw_special_infected(c_base_entity* entity);
	void draw_spawn_objects(c_base_entity* entity);
};

inline visuals g_visuals{};