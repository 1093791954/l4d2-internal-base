#pragma once

#include <string>

#define IMGUI_DEFINE_MATH_OPERATORS

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx9.h>

#include "vars.h"
#include "config.h"

namespace ImGui
{
	inline void SetTabIndex(const char* label, int* index, int selected)
	{
		if (Button(label, ImVec2(78, 20)))
			*index = selected;
	}

	inline void SetConfigPanel(char* name, int len)
	{
		for (int i{}; i < 12; i++)
			Spacing();

		PushItemWidth(78);
		InputText("##", name, len);
		PopItemWidth();

		if (Button("load"))
			config::load(name);

		SameLine();

		if (Button("save"))
			config::save(name);
	}

	inline void TabContent(int* index, int selected, std::function<void()> ctt)
	{
		if (*index != selected)
			return;

		SetCursorPos(ImVec2(92, 8));

		BeginChild("##", ImVec2(-1, -1));
		{
			SetCursorPos(ImVec2(4, 4));

			if (BeginTabBar("##"))
			{
				ctt();

				EndTabBar();
			}
		}
		EndChild();
	}

	inline bool Button(const char* label, const std::string& key, const ImVec2& size_arg = ImVec2(20, 20))
	{
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(key.c_str());
		const ImVec2 label_size = CalcTextSize(label, NULL, true);

		ImVec2 pos = window->DC.CursorPos;
		ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

		const ImRect bb(pos, pos + size);
		ItemSize(size, style.FramePadding.y);
		if (!ItemAdd(bb, id))
			return false;

		bool hovered, held;
		bool pressed = ButtonBehavior(bb, id, &hovered, &held, ImGuiButtonFlags_None);

		const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);

		RenderNavCursor(bb, id);
		RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);
		RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);

		return pressed;
	}

	inline void Checkbox(const char* label, const std::string& key)
	{
		static bool v{ false };
		auto p = g_vars.get_as<bool>(key);

		if (p)
			v = *p;

		if (Checkbox(label, &v))
			g_vars.set(key, v);
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

	inline void SliderFloat(const char* label, const std::string& key, float v_min, float v_max)
	{
		static auto v{ 0.0f };

		auto p = g_vars.get_as<float>(key);

		if (p)
			v = *p;

		if (SliderFloat(label, &v, v_min, v_max, "%.0fm"))
			g_vars.set(key, v);
	}

	inline void Combo(const char* label, const std::string& key, const char* items)
	{
		auto current_value{ g_vars.get_as<int>(key).value() };

		auto selected_item = current_value;
		if (Combo(label, &selected_item, items, IM_ARRAYSIZE(items)))
			g_vars.set(key, selected_item);
	}
}