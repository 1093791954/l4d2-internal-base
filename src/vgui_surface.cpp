#include "vgui_surface.h"

#include "mem.h"

void c_vgui_surface::draw_set_color(int r, int g, int b, int a)
{
	return mem::call_virtual_fn<void, 11>(this, r, g, b, a);
}

void c_vgui_surface::draw_filled_rect(int x, int y, int w, int h)
{
	return mem::call_virtual_fn<void, 12>(this, x, y, w, h);
}

void c_vgui_surface::set_cursor_always_visible(bool v)
{
	return mem::call_virtual_fn<void, 49>(this, v);
}

bool c_vgui_surface::is_cursor_visible()
{
	return mem::call_virtual_fn<bool, 50>(this);
}