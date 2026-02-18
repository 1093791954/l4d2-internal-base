#include "ui.h"

#include "input.h"
#include "sdk.h"

#include <sstream>
#include <ui_fonts.hpp>
#include <ui_sprites.hpp>
#include <shlobj.h>

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

	/* 合并中文字体（微软雅黑）到默认字体 */
	{
		char font_path[MAX_PATH]{};
		if (GetWindowsDirectoryA(font_path, MAX_PATH))
		{
			strcat_s(font_path, "\\Fonts\\msyh.ttc");

			ImFontConfig chinese_cfg{};
			chinese_cfg.MergeMode = true;
			chinese_cfg.FontDataOwnedByAtlas = false;

			ImGui::GetIO().Fonts->AddFontFromFileTTF(font_path, 14.0f, &chinese_cfg, ImGui::GetIO().Fonts->GetGlyphRangesChineseSimplifiedCommon());
		}
	}

	/* Droid Sans 12px */
	ImGui::fonts::small_font = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(droidsans_ttf, sizeof(droidsans_ttf), 12.0f, &font_cfg);

	/* 合并中文字体到 small_font */
	{
		char font_path[MAX_PATH]{};
		if (GetWindowsDirectoryA(font_path, MAX_PATH))
		{
			strcat_s(font_path, "\\Fonts\\msyh.ttc");

			ImFontConfig chinese_cfg{};
			chinese_cfg.MergeMode = true;
			chinese_cfg.FontDataOwnedByAtlas = false;

			ImGui::GetIO().Fonts->AddFontFromFileTTF(font_path, 12.0f, &chinese_cfg, ImGui::GetIO().Fonts->GetGlyphRangesChineseSimplifiedCommon());
		}
	}

	/* Droid Sans 22px */
	ImGui::fonts::high_font = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(droidsans_ttf, sizeof(droidsans_ttf), 22.0f, &font_cfg);

	/* 合并中文字体到 high_font */
	{
		char font_path[MAX_PATH]{};
		if (GetWindowsDirectoryA(font_path, MAX_PATH))
		{
			strcat_s(font_path, "\\Fonts\\msyh.ttc");

			ImFontConfig chinese_cfg{};
			chinese_cfg.MergeMode = true;
			chinese_cfg.FontDataOwnedByAtlas = false;

			ImGui::GetIO().Fonts->AddFontFromFileTTF(font_path, 22.0f, &chinese_cfg, ImGui::GetIO().Fonts->GetGlyphRangesChineseSimplifiedCommon());
		}
	}

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
			ImGui::SetPreviewBar("透视", "显示玩家方框等");

			ImGui::MakeChild("方框", []() {
				ImGui::ColorEdit3("##box_color", "visuals->players->box->col");
				ImGui::SameLine();

				ImGui::Combo("类型", "visuals->players->box->type", "默认\0角落");
			}, "visuals->players->box");

			ImGui::MakeChild("生命值", []() {
				ImGui::ColorEdit3("##health_color", "visuals->players->health->col");
				ImGui::SameLine();

				ImGui::Combo("类型", "visuals->players->health->type", "默认\0彩色生命");
			}, "visuals->players->health");
		};

		auto visuals_content = []() {
			ImGui::SetPreviewBar("视觉", "显示一些实体对象");

			ImGui::MakeChild("生成物品", []() {
				ImGui::ColorEdit3("##spawn_objects_col", "visuals->spawn_objects->col");
				ImGui::SameLine();

				ImGui::Combo("类型", "visuals->spawn_objects->type", "文字\0图标");
				ImGui::SliderFloat("距离", "visuals->spawn_objects->distance", 300.0f, 5000.0f);
			}, "visuals->spawn_objects");

			ImGui::MakeChild("特感", []() {
				ImGui::ColorEdit3("##special_infected_col", "visuals->special_infected->col");
				ImGui::SameLine();

				ImGui::Combo("方框", "visuals->special_infected->box->type", "无\0默认\0角落");

				if (ImGui::CollapsingHeader("骨骼绘制")) {
					ImGui::Checkbox("启用骨骼", "visuals->special_infected->skeleton");
					ImGui::Checkbox("显示骨骼序号", "visuals->special_infected->skeleton->show_index");
					ImGui::ColorEdit3("骨骼颜色", "visuals->special_infected->skeleton->col");
				}
			}, "visuals->special_infected");
		};

		auto aim_content = []() {
			ImGui::SetPreviewBar("自瞄", "按住左Shift键瞄准特感骨骼点");

			ImGui::MakeChild("特感自瞄", []() {
				ImGui::Combo("瞄准部位", "aim->bone", "头部\0胸部\0腰部");
				ImGui::SliderFloat("视野范围", "aim->fov", 5.0f, 90.0f);
				ImGui::SliderFloat("平滑度", "aim->smooth", 0.0f, 100.0f);
				ImGui::SliderFloat("最大距离", "aim->max_distance", 500.0f, 5000.0f);
			}, "aim->enabled");
		};

		auto misc_content = []() {
			ImGui::SetPreviewBar("杂项", "包含各种实用的游戏移动功能");

			ImGui::MakeChild("自动手枪", "misc->autopistol");
			ImGui::MakeChild("连跳", "misc->bhop");
			ImGui::MakeChild("自动转向", "misc->autostrafe");
		};

		auto settings_content = []() {
			static char config_name[16] = "sample_cfg";

			ImGui::SetPreviewBar("设置", "保存配置和游戏工具");

			ImGui::PushItemWidth(381.0f);
			{
				ImGui::SetCursorPosX(50.0f);
				ImGui::InputText("##", config_name, IM_ARRAYSIZE(config_name));
			}
			ImGui::PopItemWidth();

			ImGui::SameLine();

			if (ImGui::Button("加载"))
				config::load(config_name);

			ImGui::SameLine();

			if (ImGui::Button("保存"))
				config::save(config_name);

			ImGui::SetCursorPosX(50.0f);
			if (ImGui::Button("卸载", ImVec2(465.0f, 0.0f)))
				g::done = true;
		};

		tab_content_t tab_list[] = {
			{ "C", esp_content },
			{ "B", visuals_content },
			{ "A", aim_content },
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
				ss << " | " + std::to_string(g::hours_in_game) + "小时游戏中";

			else
				ss << " | " + std::to_string(g::hours_in_game) + "小时" + std::to_string(g::minutes_in_game) + "分游戏中";
		}
		else {
			ss << " | " + std::to_string(g::minutes_in_game) + "分" + std::to_string(g::seconds_in_game) + "秒游戏中";
		}
	}

	if (v[2])
		ss << " | " << std::to_string(static_cast<int>(1.f / g_l4d2.m_player_info_mgr->get_globals()->absolute_frame_time)) << "帧";


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

				ImGui::Checkbox("当前时间", &v[0]);
				ImGui::Checkbox("游戏时长", &v[1]);
				ImGui::Checkbox("显示帧率", &v[2]);

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