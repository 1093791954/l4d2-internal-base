#include "interfaces.h"

#include "globals.h"
#include "signatures.h"

void L4D2::initialise()
{
	m_client_mode = *mem::read<c_client_mode**>(reinterpret_cast<uintptr_t>(g_signatures.get_sig(S_CLIENT_MODE_PTR)));
	m_device = *mem::read<IDirect3DDevice9**>(reinterpret_cast<uintptr_t>(g_signatures.get_sig(S_DEVICE_PTR)));

	m_client = mem::get_interface<c_base_client>(
		g::module_list[clientDLL], CLIENT_INTERFACE_VERSION
	);

	m_entity_list = mem::get_interface<c_client_entity_list>(
		g::module_list[clientDLL], CLIENT_ENTITY_INTERFACE_VERSION
	);

	m_engine = mem::get_interface<c_engine_client>(
		g::module_list[engineDLL], ENGINE_CLIENT_INTERFACE_VERSION
	);

	m_engine_vgui = mem::get_interface<c_engine_vgui>(
		g::module_list[engineDLL], ENGINE_VGUI_VERSION
	);

	m_surface = mem::get_interface<c_vgui_surface>(
		g::module_list[vguiMatSurfaceDLL], VGUI_SURFACE_INTERFACE_VERSION
	);

	m_panel = mem::get_interface<c_vgui_panel>(
		g::module_list[vgui2DLL], VGUI_PANEL_INTERFACE_VERSION
	);

	m_player_info_mgr = mem::get_interface<c_player_info_manager>(
		g::module_list[serverDLL], PLAYER_INFO_MANAGER_INTERFACE_VERSION
	);

	m_game_event_manager = mem::get_interface<c_game_event_manager2>(
		g::module_list[engineDLL], GAME_EVENT_MANAGER_INTERFACE_VERSION
	);

	m_input_system = mem::get_interface<c_input_system>(
		g::module_list[inputSystemDLL], INPUTSYSTEM_INTERFACE_VERSION
	);

	m_tracers = mem::get_interface<c_trace_ray>(
		g::module_list[engineDLL], ENGINE_TRACE_INTERFACE_VERSION
	);

	m_materials = mem::get_interface<c_material_system>(
		g::module_list[materialSystemDLL], MATERIAL_SYSTEM_INTERFACE_VERSION
	);

	m_model_info = mem::get_interface<c_model_info>(
		g::module_list[engineDLL], MODEL_INFO_INTERFACE_VERSION
	);

	m_model_render = mem::get_interface<c_model_render>(
		g::module_list[engineDLL], ENGINE_MODEL_INTERFACE_VERSION
	);
	
	m_debug_overlay = mem::get_interface<c_debug_overlay>(
		g::module_list[engineDLL], DEBUG_OVERLAY_INTERFACE_VERSION
	);
}

void L4D2::init_local(const local_t& local)
{
	m_local = local;
}

c_base_player* L4D2::get_local()
{
	if (!m_local)
		return nullptr;

	return m_local.get();
}