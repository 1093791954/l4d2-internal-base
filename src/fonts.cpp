#include "fonts.h"

void fonts::initialise(IDirect3DDevice9* device, const std::vector<font_t> list)
{
	if (m_font_list.empty())
		m_font_list = list;

	for (const auto& font : m_font_list)
	{
		D3DXCreateFontA(
			device,
			font.m_px, 0,
			font.m_weight, 1, 0,
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			font.m_quality,
			FF_DONTCARE,
			font.m_font_name.c_str(),
			&m_fonts[font.m_index]);
	}
}

void fonts::draw_text(const std::string& string, int x, int y, _font_list font, uint8_t flags, color_t color)
{
	RECT r{ x, y, x, y };
	RECT o_r{ x + 1, y + 1, x + 1, y + 1 };

	if (flags & TEXT_CENTER_X) {
		const auto half_width = get_text_width(font, string) / 2;

		r = { x - half_width, y, x - half_width, y };
		o_r = { x - half_width + 1, y + 1, x - half_width + 1, y + 1 };
	}

	if (flags & TEXT_OUTLINE) {
		m_fonts[font]->DrawTextA(NULL, string.c_str(), -1, &o_r, DT_NOCLIP, color_t(0, 0, 0, color.get_a()).get());
	}

	m_fonts[font]->DrawTextA(NULL, string.c_str(), -1, &r, DT_NOCLIP, color.get());
}

int fonts::get_text_width(_font_list font, const std::string& string)
{
	RECT r{};
	m_fonts[font]->DrawTextA(0, string.c_str(), -1, &r, DT_CALCRECT, 0xffffffff);

	return (r.right - r.left);
}

int fonts::get_text_height(_font_list font, const std::string& string)
{
	RECT r{};
	m_fonts[font]->DrawTextA(0, string.c_str(), -1, &r, DT_CALCRECT, 0xffffffff);

	return (r.bottom - r.top);
}

ID3DXFont* fonts::get_font(_font_list index)
{
	return m_fonts[index];
}

void fonts::shutdown()
{
	for (const auto& font : m_font_list)
	{
		if (m_fonts[font.m_index]) {
			m_fonts[font.m_index]->Release();
			m_fonts[font.m_index] = nullptr;
		}
	}
}