#pragma once

#include <string>

enum _module_list {
	shaderapidx9DLL,
	serverBrowserDLL,
	clientDLL,
	engineDLL,
	vguiMatSurfaceDLL,
	vgui2DLL,
	serverDLL,
	inputSystemDLL,
	materialSystemDLL,
};

namespace g
{
	inline bool initialised{},
		               done{};

	inline int minutes_in_game{},
		       seconds_in_game{};

	inline int screen_width{};
	inline int screen_height{};

	inline std::string module_list[] = {
		"shaderapidx9.dll",
		"serverbrowser.dll",
		"client.dll",
		"engine.dll",
		"vguimatsurface.dll",
		"vgui2.dll",
		"server.dll",
		"inputsystem.dll",
		"materialsystem.dll",
	};
}