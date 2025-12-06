#pragma once

#define VGUI_SURFACE_INTERFACE_VERSION "VGUI_Surface031"

enum _font_flags {
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};

class c_vgui_surface {
public:
	/*
	* ISurface::DrawSetColor(int, int, int, int)
	*
	*/
	void draw_set_color(int r, int g, int b, int a = 255);

	/*
	* ISurface::DrawFilledRect(int, int, int, int)
	*
	*/
	void draw_filled_rect(int x, int y, int w, int h);

	/*
	* ISurface::SetCursorAlwaysVisible(bool)
	*
	*/
	void set_cursor_always_visible(bool v);

	/*
	* ISurface::IsCursorVisible()
	*
	*/
	bool is_cursor_visible();
};