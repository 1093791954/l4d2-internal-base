#pragma once

#define VGUI_PANEL_INTERFACE_VERSION "VGUI_Panel009"

#include "mem.h"

class c_vgui_panel {
public:
	const char* get_panel_name(unsigned int id) {
		return mem::call_virtual_fn<const char*, 36>(this, id);
	}
};