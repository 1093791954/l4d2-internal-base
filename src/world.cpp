#include "world.h"

#include "vars.h"
#include "game_math.h"
#include "fonts.h"
#include "renderer.h"

static std::string weapon_name_list[] = {
	"unk1",
	"pistol",
	"uzi",
	"pump shotgun",
	"auto shotgun",
	"m16a1",
	"hunting rifle",
	"mac10",
	"chrome shotgun",
	"scar",
	"military sniper",
	"spas",
	"firstaid",
	"molotov",
	"pipebomb",
	"pills",
	"gascan",
	"propane tank",
	"oxygen tank",
	"melee",
	"chainsaw",
	"grenade launcher",
	"unk3",
	"adrenaline",
	"defibrillator",
	"vomitjar",
	"ak47",
	"unk4",
	"unk5",
	"firework crate",
	"incendiary ammo",
	"explosive ammo",
	"deagle",
	"mp5",
	"sg552",
	"awp",
	"scout",
	"m60",
	"unk6",
};

static std::string weapon_icon_list[] = {
	"",
	"C",
	"W",
	"M",
	"b",
	"Z",
	"O",
	"U",
	"b",
	"f",
	"X",
	"a",
	"v",
	"l",
	"p",
	"r",
	"o",
	"",
	"",
	"",
	"K",
	"T",
	"",
	"u",
	"s",
	"m",
	"Y",
	"",
	"",
	"",
	"R",
	"S",
	"D",
	"V",
	"c",
	"e",
	"d",
	"g",
	"",
};

void visuals::render()
{
	if (!g_l4d2.m_engine->is_connected())
		return;

	if (!g_l4d2.get_local())
		return;

	for (int i{}; i < g_l4d2.m_entity_list->get_highest_entity_index(); i++)
	{
		auto entity = g_l4d2.m_entity_list->get<c_base_entity*>(i);

		if (!entity || i == g_l4d2.m_engine->get_local_player())
			continue;

		if (g_vars.get_as<bool>("visuals->special_infected").value())
			draw_special_infected(entity);

		if (g_vars.get_as<bool>("visuals->spawn_objects").value())
			draw_spawn_objects(entity);
	}
}

void visuals::draw_special_infected(c_base_entity* entity)
{
	auto class_list = entity->get_client_class();

	if (!class_list)
		return;

	for (const auto& infected : std::vector<std::pair<std::string, _class_id>>{
		{ "Hunter",  Hunter },
		{ "Tank",    Tank },
		{ "Jockey",  Jockey },
		{ "Witch",   Witch },
		{ "Boomer",  Boomer },
		{ "Smoker",  Smoker },
		{ "Spitter", Spitter },
		{ "Charger", Charger },
		})
	{
		if (class_list->m_class_id == infected.second)
		{
			// 绘制名称
			vec3 pos{};
			if (math::w2s(entity->get_origin(), pos))
			{
				g_fonts.draw_text(infected.first, pos.x, pos.y, Tahoma12px,
					TEXT_OUTLINE, color_t{ "visuals->special_infected->col" });
			}

			// 绘制方框
			auto box_type = g_vars.get_as<int>("visuals->special_infected->box->type").value();
			if (box_type > 0)
			{
				vec3 top = entity->get_origin() + vec3(0, 0, 72);
				vec3 down = entity->get_origin();
				vec3 s[2]{};

				if (math::w2s(top, s[1]) && math::w2s(down, s[0]))
				{
					vec3 delta{ s[1] - s[0] };

					box bbox{};
					bbox.h = static_cast<int>(std::fabsf(delta.y));
					bbox.w = static_cast<int>(bbox.h / 2.0f);
					bbox.x = static_cast<int>(s[1].x - (bbox.w / 2.0f));
					bbox.y = static_cast<int>(s[1].y);

					auto outline_col = color_t{ 3, 3, 3, 255 };
					auto col = color_t{ "visuals->special_infected->col" };

					if (box_type == 1) {
						// 默认方框
						g_renderer.draw_line(bbox.x - 1, bbox.y - 1, bbox.x + bbox.w + 1, bbox.y - 1, 1.0f, outline_col);
						g_renderer.draw_line(bbox.x - 1, bbox.y + bbox.h + 1, bbox.x + bbox.w + 1, bbox.y + bbox.h + 1, 1.0f, outline_col);
						g_renderer.draw_line(bbox.x - 1, bbox.y - 1, bbox.x - 1, bbox.y + bbox.h + 1, 1.0f, outline_col);
						g_renderer.draw_line(bbox.x + bbox.w + 1, bbox.y - 1, bbox.x + bbox.w + 1, bbox.y + bbox.h + 1, 1.0f, outline_col);

						g_renderer.draw_line(bbox.x, bbox.y, bbox.x + bbox.w, bbox.y, 1.0f, col);
						g_renderer.draw_line(bbox.x, bbox.y + bbox.h, bbox.x + bbox.w, bbox.y + bbox.h, 1.0f, col);
						g_renderer.draw_line(bbox.x, bbox.y, bbox.x, bbox.y + bbox.h, 1.0f, col);
						g_renderer.draw_line(bbox.x + bbox.w, bbox.y, bbox.x + bbox.w, bbox.y + bbox.h, 1.0f, col);
					}
					else if (box_type == 2) {
						// 角落方框
						g_renderer.draw_corner_box({ bbox.x - 1, bbox.y - 1, bbox.w + 2, bbox.h + 2 }, 3, 5, outline_col);
						g_renderer.draw_corner_box({ bbox.x, bbox.y, bbox.w, bbox.h }, 3, 5, col);
					}
				}
			}
		}
	}
}

void visuals::draw_spawn_objects(c_base_entity* entity)
{
	auto class_list = entity->get_client_class();

	if (!class_list)
		return;

	if (class_list->m_class_id != CWeaponSpawn)
		return;

	vec3 pos{};
	vec3 origin = entity->get_origin();
	if (!math::w2s(origin, pos))
		return;

	if (origin.distance_to(g_l4d2.get_local()->get_origin()) >
		g_vars.get_as<float>("visuals->spawn_objects->distance").value())
		return;

	auto weapon = reinterpret_cast<c_base_weapon*>(entity);

	if (!weapon)
		return;

	auto col = color_t{ "visuals->spawn_objects->col" };

	switch (g_vars.get_as<int>("visuals->spawn_objects->type").value()) {
	case 0: {
		g_fonts.draw_text(weapon_name_list[weapon->get_weapon_id()],
			pos.x, pos.y, Tahoma12px, TEXT_OUTLINE | TEXT_CENTER_X, col);
		break;
	}
	case 1: {
		g_fonts.draw_text(weapon_icon_list[weapon->get_weapon_id()],
			pos.x, pos.y, Left4Dead_Icons21px, TEXT_OUTLINE | TEXT_CENTER_X, col);
		break;
	}
	}
}