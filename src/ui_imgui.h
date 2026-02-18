#pragma once

#include <string>

#define IMGUI_DEFINE_MATH_OPERATORS

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx9.h>

#include "vars.h"
#include "config.h"

struct tab_content_t {
	char* icon{};
	std::function<void()> fn{};
};

namespace ImGui
{
	namespace fonts
	{
		inline ImFont* small_font{};
		inline ImFont* high_font{};

		inline ImFont* small_icon_font{};
		inline ImFont* icon_font{};
	}

	inline bool Tab(const char* icon, bool selected)
	{
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(icon);
		const ImVec2 label_size = CalcTextSize(icon, NULL, true);

		ImVec2 pos = window->DC.CursorPos;
		ImGuiButtonFlags flags = ImGuiButtonFlags_None;

		if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset)
			pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;

		ImVec2 size_arg = ImVec2(40.0f, 40.0f);
		ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

		const ImRect bb(pos, pos + size);
		ItemSize(size, style.FramePadding.y);
		if (!ItemAdd(bb, id))
			return false;

		bool hovered, held;
		bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

		if (selected)
			PushStyleColor(ImGuiCol_Button, GetColorU32(ImGuiCol_ButtonActive));
		else
			PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

		const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
		RenderNavCursor(bb, id);

		const float line_height = 6.5f;

		window->DrawList->AddLine(
			ImVec2(bb.Max.x - 1.5f, (bb.Min.y + bb.Max.y) * 0.5f - line_height),
			ImVec2(bb.Max.x - 1.5f, (bb.Min.y + bb.Max.y) * 0.5f + line_height),
			col, 2.0f
		);

		PopStyleColor();

		PushFont(fonts::icon_font);

		PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.40f, 0.50f));
		PushStyleColor(ImGuiCol_Text, selected ? GetColorU32(ImGuiCol_Text) : GetColorU32(ImGuiCol_TextDisabled));

		RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, icon, NULL, &label_size, style.ButtonTextAlign, &bb);

		PopStyleColor();
		PopStyleVar();

		PopFont();

		return pressed;
	}

	inline void Toggle(const char* id, const std::string& key)
	{
		ImVec2 p = GetCursorScreenPos();
		ImDrawList* draw = GetWindowDrawList();

		float height = 14.0f;
		float width = height * 1.5f;
		float radius = height * 0.50f;

		InvisibleButton(id, ImVec2(width + 8.f, height));

		static bool v = false;
		auto pv = g_vars.get_as<bool>(key);

		if (pv)
			v = *pv;

		if (IsItemClicked()) {
			v = !v;
			g_vars.set(key, v);
		}

		float t = v ? 1.0f : 0.0f;

		ImGuiContext& g = *GImGui;
		if (g.LastActiveId == g.CurrentWindow->GetID(id))
		{
			float t_anim = ImSaturate(g.LastActiveIdTimer / 0.2f);
			t = v ? (t_anim) : (1.0f - t_anim);
		}

		ImU32 col_bg;
		if (IsItemHovered())
			col_bg = GetColorU32(ImLerp(ImVec4(0.78f, 0.78f, 0.78f, 0.0f), ImVec4(0.f, 120 / 255.f, 215 / 255.f, 1.f), t));
		else
			col_bg = GetColorU32(ImLerp(ImVec4(0.85f, 0.85f, 0.85f, 0.0f), ImVec4(0.f, 120 / 255.f, 215 / 255.f, 1.f), t));

		draw->AddRect(p, ImVec2(p.x + width + 8.f, p.y + height + 3.f),
			IsItemHovered() ? IM_COL32(255, 255, 255, 255) : IM_COL32(153, 153, 153, 255), height * 0.5f);

		draw->AddRectFilled(p, ImVec2(p.x + width + 8.f, p.y + height + 3.f), col_bg, height * 0.5f);

		draw->AddCircleFilled(ImVec2(p.x + radius + t * (width - radius * 1.2f) + 2.f, p.y + radius + 1.5f),
			radius - 2.5f, IM_COL32(255, 255, 255, 255));
	}

	inline void MakeChild(const std::string& label, const std::string& key)
	{
		std::string _id{ "##" + label };

		SetCursorPosX(50.0f);
		BeginChild(_id.c_str(), ImVec2(465.0f, 28.0f));
		{
			SetCursorPos(ImVec2(12.0f, 5.0f));
			Text(label.c_str());

			SameLine();

			SetCursorPosX(430.0f);

			Toggle(std::string{ "##" + _id + "-toggle" }.c_str(), key);
		}
		EndChild();
	}

	inline void MakeChild(const std::string& label, std::function<void()> fn, const std::string& key)
	{
		std::string _id{ "##" + label };

		SetCursorPosX(50.0f);
		BeginChild(_id.c_str(), ImVec2(465.0f, 28.0f));
		{
			SetCursorPos(ImVec2(12.0f, 5.0f));
			Text(label.c_str());

			SameLine();

			SetCursorPosX(400.0f);

			PushFont(fonts::small_icon_font);

			PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

			SetCursorPosY(2);
			if (Button("D", ImVec2(20.0f, 20.0f)))
				OpenPopup(std::string{ "##" + _id + "-popup" }.c_str());

			PopStyleColor();
			PopStyleColor();
			PopStyleColor();

			PopFont();

			PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 15.0f));
			PushStyleVar(ImGuiStyleVar_PopupRounding, 10.0f);

			if (BeginPopup(std::string{ "##" + _id + "-popup" }.c_str())) {
				fn();
				EndPopup();
			}

			PopStyleVar();
			PopStyleVar();

			SameLine();

			SetCursorPosX(430.0f);
			Toggle(std::string{ "##" + _id + "-toggle" }.c_str(), key);
		}
		EndChild();
	}

	inline void SetPreviewBar(const char* title, const char* description)
	{
		SetCursorPosX(50.0f);
		BeginChild("##", ImVec2(465.0f, 50.0f));
		{
			SetCursorPos(ImVec2(12.0f, 5.0f));

			PushFont(fonts::high_font);
			Text(title);
			PopFont();

			SetCursorPos(ImVec2(12.0f, 30.0f));

			PushFont(fonts::small_font);
			TextDisabled(description);
			PopFont();
		}
		EndChild();
	}

	inline void SliderFloat(const char* label, const std::string& key, float v_min, float v_max)
	{
		static auto v = 0.0f;
		auto p = g_vars.get_as<float>(key);

		if (p)
			v = *p;

		if (SliderFloat(label, &v, v_min, v_max, "%.0fm"))
			g_vars.set(key, v);
	}

	inline void Combo(const char* label, const std::string& key, const char* items)
	{
		auto current_value = g_vars.get_as<int>(key).value();

		auto selected_item = current_value;
		if (Combo(label, &selected_item, items, IM_ARRAYSIZE(items)))
			g_vars.set(key, selected_item);
	}

	inline void ColorEdit3(const char* label, const std::string& key)
	{
		float col[3]{};

		col[0] = g_vars.get_as<int>(key + "->r").value() / 255.0f;
		col[1] = g_vars.get_as<int>(key + "->g").value() / 255.0f;
		col[2] = g_vars.get_as<int>(key + "->b").value() / 255.0f;

		if (ColorEdit3(label, col, ImGuiColorEditFlags_NoInputs)) {
			g_vars.set(key + "->r", static_cast<int>(col[0] * 255));
			g_vars.set(key + "->g", static_cast<int>(col[1] * 255));
			g_vars.set(key + "->b", static_cast<int>(col[2] * 255));
		}
	}

	inline void Checkbox(const char* label, const std::string& key)
	{
		bool v = g_vars.get_as<bool>(key).value_or(false);
		if (Checkbox(label, &v)) {
			g_vars.set(key, v);
		}
	}

	inline void SetupTabs(tab_content_t t_list[], int max_tabs, int* selected)
	{
		PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

		for (int i{}; i < max_tabs; i++)
		{
			if (Tab(t_list[i].icon, *selected == i))
				*selected = i;
		}

		PopStyleVar();

		for (int i{}; i < max_tabs; i++)
		{
			if (*selected == i)
			{
				ImGui::SetCursorPosY(10.0f);
				t_list[i].fn();
			}
		}
	}

	inline bool InitFrame(const char* label, ImVec2 size)
	{
		SetNextWindowSize(size);
		auto ret = Begin(label, 0, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);

		auto window_pos = GetWindowPos();
		auto window_draw_list = GetWindowDrawList();
		auto window_size = GetWindowSize();

		window_draw_list->AddRectFilled(
			window_pos + ImVec2(40.0f, 0.0f),
			window_pos + window_size,
			GetColorU32(ImGuiCol_WindowBg),
			15.0f,
			ImDrawFlags_RoundCornersTopRight | ImDrawFlags_RoundCornersBottomRight
		);

		window_draw_list->AddRectFilled(
			window_pos,
			window_pos + window_size,
			IM_COL32(0, 0, 0, 50),
			15.0f,
			ImDrawFlags_RoundCornersTopRight | ImDrawFlags_RoundCornersBottomRight
		);

		return ret;
	}

	inline void ShutdownFrame()
	{
		End();
	}
}