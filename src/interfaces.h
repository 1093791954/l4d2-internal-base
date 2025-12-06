#pragma once

#include "base_client.h"
#include "client_mode.h"
#include "client_entity_list.h"
#include "engine_client.h"
#include "engine_vgui.h"
#include "vgui_surface.h"
#include "vgui_panel.h"
#include "player_info_manager.h"
#include "game_event_listener2.h"
#include "input_system.h"
#include "trace_ray.h"
#include "material_system.h"
#include "model_info.h"
#include "model_render.h"
#include "debug_overlay.h"

#include <d3dx9.h>

struct interfaces {
	c_base_client* m_client{};
	c_client_entity_list* m_entity_list{};
	c_engine_client* m_engine{};
	c_engine_vgui* m_engine_vgui{};
	c_vgui_surface* m_surface{};
	c_vgui_panel* m_panel{};
	c_player_info_manager* m_player_info_mgr{};
	c_game_event_manager2* m_game_event_manager{};
	c_input_system* m_input_system{};
	c_trace_ray* m_tracers{};
	c_material_system* m_materials{};
	c_model_info* m_model_info{};
	c_model_render* m_model_render{};
	c_debug_overlay* m_debug_overlay{};
	c_client_mode* m_client_mode{};
	IDirect3DDevice9* m_device{};
};

struct local_t {
	local_t() = default;
	local_t(c_base_player* ptr) : m_ptr(ptr) {};

	c_base_player* get() const {
		return m_ptr;
	}

	bool operator!() const {
		return (m_ptr == nullptr);
	}

private:
	c_base_player* m_ptr{};
};

struct L4D2 : public interfaces {
	void initialise();

	void init_local(const local_t& local);
	c_base_player* get_local();

private:
	local_t m_local{};
};

inline L4D2 g_l4d2{};