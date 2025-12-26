#include "ui.h"

#include "input.h"
#include "sdk.h"

#include <sstream>
#include <ui_fonts.hpp>
#include <ui_sprites.hpp>

PDIRECT3DTEXTURE9 g_L4D2Logo = nullptr;

void ui::initialise(IDirect3DDevice9* device)
{
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(g_input.get_window());

	ImGui::StyleColorsDark();

	ImGui::GetStyle().WindowMinSize = ImVec2(20.0f, 20.0f);

	ImGui::GetIO().IniFilename = {};
	ImGui::GetIO().LogFilename = {};
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

	ImFontConfig font_cfg{};

	font_cfg.FontDataOwnedByAtlas = false;

	ImGui::GetIO().Fonts->AddFontFromMemoryTTF(droidsans_ttf, sizeof(droidsans_ttf), 14.0f, &font_cfg);

	/* Droid Sans 12px */
	ImGui::fonts::small_font = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(droidsans_ttf, sizeof(droidsans_ttf), 12.0f, &font_cfg);
	
	/* Droid Sans 22px */
	ImGui::fonts::high_font = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(droidsans_ttf, sizeof(droidsans_ttf), 22.0f, &font_cfg);
	
	/* Tab Icons 14px */
	ImGui::fonts::small_icon_font = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(icons_ttf, sizeof(icons_ttf), 14.0f, &font_cfg);
	
	/* Tab Icons 16px */
	ImGui::fonts::icon_font = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(icons_ttf, sizeof(icons_ttf), 16.0f, &font_cfg);

	/* L4D2 icon for watermark */
	D3DXCreateTextureFromFileInMemoryEx(device, &l4d2_icon, sizeof(l4d2_icon), 30, 30,
		D3DUSAGE_DYNAMIC,
		0,
		D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT,
		D3DX_DEFAULT, D3DX_DEFAULT,
		0, NULL, NULL,
		&g_L4D2Logo
	);

	ImGui_ImplDX9_Init(device);
}

void ui::invalidate_device_objects()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
}

void ui::create_device_objects()
{
	ImGui_ImplDX9_CreateDeviceObjects();
}

void ui::render()
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();
	{
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.06f, 0.06f, 0.06f, 1.00f));

		if (m_opened)
			draw_main_frame();

		draw_watermark();

		ImGui::PopStyleColor();
	}
	ImGui::EndFrame();

	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void ui::draw_main_frame()
{
	ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 1.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 0.0f);

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.00f, 1.00f, 1.00f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_TextDisabled, ImVec4(1.00f, 1.00f, 1.00f, 0.78f));
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.06f, 0.06f, 0.06f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.06f, 0.06f, 0.06f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.15f, 0.15f, 0.15f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.15f, 0.15f, 0.15f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.15f, 0.15f, 0.15f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.15f, 0.15f, 0.15f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.15f, 0.15f, 0.15f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.15f, 0.15f, 0.15f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, ImVec4(0.26f, 0.25f, 0.25f, 1.00f));

	if (ImGui::InitFrame("##MainFrame", ImVec2(525.0f, 420.0f)))
	{
		auto esp_content = []() {
			ImGui::SetPreviewBar("ESP", "Display player boxes & etc");

			ImGui::MakeChild("Box", []() {
				ImGui::ColorEdit3("##box_color", "visuals->players->box->col");
				ImGui::SameLine();

				ImGui::Combo("Type", "visuals->players->box->type", "Default\0Corner");
			}, "visuals->players->box");

			ImGui::MakeChild("Health", []() {
				ImGui::ColorEdit3("##health_color", "visuals->players->health->col");
				ImGui::SameLine();

				ImGui::Combo("Type", "visuals->players->health->type", "Default\0Colored Health");
			}, "visuals->players->health");
		};

		auto visuals_content = []() {
			ImGui::SetPreviewBar("Visuals", "Display some entity objects");

			ImGui::MakeChild("Spawn Objects", []() {
				ImGui::ColorEdit3("##spawn_objects_col", "visuals->spawn_objects->col");
				ImGui::SameLine();

				ImGui::Combo("Type", "visuals->spawn_objects->type", "Text\0Icon");
				ImGui::SliderFloat("Distance", "visuals->spawn_objects->distance", 300.0f, 5000.0f);
			}, "visuals->spawn_objects");

			ImGui::MakeChild("Special Infected", []() {
				ImGui::ColorEdit3("##special_infected_col", "visuals->special_infected->col");
			}, "visuals->special_infected");
		};

		auto misc_content = []() {
			ImGui::SetPreviewBar("Misc", "Include various helpful game movement");

			ImGui::MakeChild("Auto Pistol", "misc->autopistol");
			ImGui::MakeChild("Bunnyhop", "misc->bhop");
			ImGui::MakeChild("Auto Strafe", "misc->autostrafe");
		};

		auto settings_content = []() {
			static char config_name[16] = "sample_cfg";

			ImGui::SetPreviewBar("Settings", "Save configurations & Game utilities");

			ImGui::PushItemWidth(381.0f);
			{
				ImGui::SetCursorPosX(50.0f);
				ImGui::InputText("##", config_name, IM_ARRAYSIZE(config_name));
			}
			ImGui::PopItemWidth();

			ImGui::SameLine();

			if (ImGui::Button("Load"))
				config::load(config_name);

			ImGui::SameLine();

			if (ImGui::Button("Save"))
				config::save(config_name);

			ImGui::SetCursorPosX(50.0f);
			if (ImGui::Button("Unload", ImVec2(465.0f, 0.0f)))
				g::done = true;
		};

		tab_content_t tab_list[] = {
			{ "C", esp_content },
			{ "B", visuals_content },
			{ "D", misc_content },
			{ "E", settings_content },
		};

		ImGui::SetupTabs(tab_list, sizeof(tab_list) / sizeof(tab_list[0]), &m_selected_tab);
		ImGui::ShutdownFrame();
	}

	ImGui::PopStyleColor(11);
	ImGui::PopStyleVar(5);
}

void ui::draw_watermark()
{
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.13f, 0.13f, 0.13f, 0.98f));
	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.15f, 0.15f, 0.15f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.15f, 0.15f, 0.15f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.15f, 0.15f, 0.15f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(1.0f, 1.0f, 1.0f, 0.67f));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.15f, 0.15f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.18f, 0.18f, 0.18f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.20f, 0.20f, 0.20f, 1.00f));

	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 4.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 4.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 0.0f);

	static bool v[3] = { true, false, false };
	std::ostringstream ss{};
	ss << "L4D2";

	if (v[0])
		ss << " | " << util::get_current_time();

	if (v[1])
	{
		if (g::hours_in_game > 0)
		{
			if (g::hours_in_game >= 24)
				ss << " | " + std::to_string(g::hours_in_game) + "h Playing";

			else
				ss << " | " + std::to_string(g::hours_in_game) + "h " + std::to_string(g::minutes_in_game) + "m Playing";
		}
		else {
			ss << " | " + std::to_string(g::minutes_in_game) + "m " + std::to_string(g::seconds_in_game) + "s Playing";
		}
	}

	if (v[2])
		ss << " | " << std::to_string(static_cast<int>(1.f / g_l4d2.m_player_info_mgr->get_globals()->absolute_frame_time)) << " fps";


	auto width = ImGui::CalcTextSize(ss.str().c_str()).x;
	ImGui::SetNextWindowSize(ImVec2(m_opened ? width + 70.0f : width + 42.0f, 30.0f));

	ImGuiWindowFlags flags = (ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar |
		                      ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar |
		                      ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoMove);

	if (m_opened)
		flags &= ~ImGuiWindowFlags_NoMove;

	ImGui::SetNextWindowPos(ImVec2(5.0f, 5.0f), ImGuiCond_Once);

	if (ImGui::Begin("##WatermarkFrame", 0, flags))
	{
		ImGui::SetCursorPos(ImVec2(5.0f, 5.0f));
		ImGui::Image(g_L4D2Logo, ImVec2(20.0f, 20.0f));

		ImGui::SetCursorPos(ImVec2(32.0f, 7.0f));
		ImGui::Text(ss.str().c_str());

		if (m_opened)
		{
			ImGui::SameLine();

			ImGui::SetCursorPos(ImVec2(width + 40.0f, 0));
			if (ImGui::Button(ImGui::IsPopupOpen("##WatermarkUtils") ? "-" : "+", ImVec2(30.0f, 30.0f)))
				ImGui::OpenPopup("##WatermarkUtils");

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4.0f, 3.0f));

			if (ImGui::BeginPopup("##WatermarkUtils"))
			{
				ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.15f, 0.15f, 0.15f, 1.00f));
				ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.15f, 0.15f, 0.15f, 1.00f));
				ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.15f, 0.15f, 0.15f, 1.00f));
				ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(1.0f, 1.0f, 1.0f, 0.67f));

				ImGui::Checkbox("Current Time", &v[0]);
				ImGui::Checkbox("Time Playing", &v[1]);
				ImGui::Checkbox("Show FPS", &v[2]);

				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();

				ImGui::EndPopup();
			}

			ImGui::PopStyleVar();
		}

		ImGui::End();
	}

	ImGui::PopStyleVar(5);
	ImGui::PopStyleColor(8);
}

void ui::toggle()
{
	m_opened = !m_opened;
	g_l4d2.m_surface->set_cursor_always_visible(m_opened);

	if (!g_l4d2.m_engine->is_game_paused() && g_l4d2.m_engine->is_in_game())
	{
		if (m_opened) {
			m_old_mouse_pos_x = g_input.get_mouse_pos_x();
			m_old_mouse_pos_y = g_input.get_mouse_pos_y();
		}
		else {
			RECT r{};
			if (GetWindowRect(g_input.get_window(), &r)) {
				POINT p{ m_old_mouse_pos_x, m_old_mouse_pos_y };
				
				ClientToScreen(g_input.get_window(), &p);
				SetCursorPos(p.x, p.y);
			}
		}
	}
}

void ui::shutdown()
{
	if (m_opened) {
		g_l4d2.m_surface->set_cursor_always_visible(false);
		m_opened = false;
	}

	m_old_mouse_pos_x = 0;
	m_old_mouse_pos_y = 0;

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();

	ImGui::DestroyContext();
}