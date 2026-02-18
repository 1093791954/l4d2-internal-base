#include "renderer.h"
#include "globals.h"

#include <cmath>

bool renderer::initialise(IDirect3DDevice9* device)
{
	return create_objects(device);
}

bool renderer::create_objects(IDirect3DDevice9* device)
{
	if (!m_device)
		m_device = device;

	D3DVIEWPORT9 vp{};
	if (FAILED(m_device->GetViewport(&vp)))
		return false;

	g::screen_width = vp.Width;
	g::screen_height = vp.Height;

	if (FAILED(D3DXCreateLine(m_device, &m_line)))
		return false;

	if (FAILED(m_device->CreateStateBlock(D3DSBT_ALL, &m_block)))
		return false;

	return true;
}

void renderer::begin_modificated_states()
{
	if (!m_device)
		return;

	D3DVIEWPORT9 vp{ 0, 0, g::screen_width, g::screen_height, 0.0f, 1.0f };
	m_device->SetViewport(&vp);

	m_block->Capture();

	m_device->SetVertexShader(nullptr);
	m_device->SetPixelShader(nullptr);

	m_device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	m_device->SetRenderState(D3DRS_LIGHTING, false);
	m_device->SetRenderState(D3DRS_FOGENABLE, false);
	m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	m_device->SetRenderState(D3DRS_SCISSORTESTENABLE, true);
	m_device->SetRenderState(D3DRS_ZWRITEENABLE, false);
	m_device->SetRenderState(D3DRS_STENCILENABLE, false);

	m_device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, false);
	m_device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, true);

	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	m_device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, true);

	m_device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	m_device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	m_device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	m_device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	m_device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA);
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);
	m_device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	m_device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
	m_device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);

	m_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
}

void renderer::draw_line(float x0, float y0, float x1, float y1, float thickness, color_t color)
{
	D3DXVECTOR2 lines[2] = {
		D3DXVECTOR2(x0, y0), D3DXVECTOR2(x1, y1)
	};

	m_line->SetWidth(thickness);

	m_line->Begin();
	m_line->Draw(lines, 2, color.get());
	m_line->End();
}

void renderer::draw_filled_rect(const rect_t pos, color_t color)
{
	vertex_t v[4] = {
		{ pos.x, pos.y + pos.h, 0.0f, 1.0f, color.get() },
		{ pos.x, pos.y, 0.0f, 1.0f, color.get() },
		{ pos.x + pos.w, pos.y + pos.h, 0.0f, 1.0f, color.get() },
		{ pos.x + pos.w, pos.y, 0.0f, 1.0f, color.get() }
	};

	m_device->SetTexture(0, nullptr);

	// m_device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, true);
	m_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(vertex_t));
	// m_device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, false);
}

void renderer::draw_rect(const rect_t pos, color_t color)
{
	draw_line(pos.x, pos.y, pos.x + pos.w, pos.y, 1.0f, color);
	draw_line(pos.x, pos.y, pos.x, pos.y + pos.h, 1.0f, color);
	draw_line(pos.x + pos.w, pos.y, pos.x + pos.w, pos.y + pos.h, 1.0f, color);
	draw_line(pos.x, pos.y + pos.h, pos.x + pos.w, pos.y + pos.h, 1.0f, color);
}

void renderer::draw_corner_box(const rect_t pos, float cx, float cy, color_t color)
{
	draw_line(pos.x, pos.y, pos.x + (pos.w / cx), pos.y, 1.0f, color);
	draw_line(pos.x, pos.y, pos.x, pos.y + (pos.h / cy), 1.0f, color);

	draw_line(pos.x + pos.w, pos.y, pos.x + pos.w - (pos.w / cx), pos.y, 1.0f, color);
	draw_line(pos.x + pos.w, pos.y, pos.x + pos.w, pos.y + (pos.h / cy), 1.0f, color);

	draw_line(pos.x, pos.y + pos.h, pos.x + (pos.w / cx), pos.y + pos.h, 1.0f, color);
	draw_line(pos.x, pos.y + pos.h, pos.x, pos.y + pos.h - (pos.h / cy), 1.0f, color);

	draw_line(pos.x + pos.w, pos.y + pos.h, pos.x + pos.w - (pos.w / cx), pos.y + pos.h, 1.0f, color);
	draw_line(pos.x + pos.w, pos.y + pos.h, pos.x + pos.w, pos.y + pos.h - (pos.h / cy), 1.0f, color);
}

void renderer::draw_circle(float x, float y, float radius, int segments, color_t color)
{
	float step = 3.14159265f * 2.0f / segments;
	for (int i = 0; i < segments; i++)
	{
		float theta1 = step * i;
		float theta2 = step * (i + 1);

		float x1 = x + radius * cosf(theta1);
		float y1 = y + radius * sinf(theta1);
		float x2 = x + radius * cosf(theta2);
		float y2 = y + radius * sinf(theta2);

		draw_line(x1, y1, x2, y2, 1.0f, color);
	}
}

void renderer::draw_filled_circle(float x, float y, float radius, int segments, color_t color)
{
	float step = 3.14159265f * 2.0f / segments;

	for (int i = 0; i < segments; i++)
	{
		float theta1 = step * i;
		float theta2 = step * (i + 1);

		float x1 = x + radius * cosf(theta1);
		float y1 = y + radius * sinf(theta1);
		float x2 = x + radius * cosf(theta2);
		float y2 = y + radius * sinf(theta2);

		// 绘制扇形填充三角形
		vertex_t v[3] = {
			{ x, y, 0.0f, 1.0f, color.get() },
			{ x1, y1, 0.0f, 1.0f, color.get() },
			{ x2, y2, 0.0f, 1.0f, color.get() }
		};

		m_device->SetTexture(0, nullptr);
		m_device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, v, sizeof(vertex_t));
	}
}

void renderer::end_modificated_states()
{
	m_block->Apply();
}

void renderer::shutdown()
{
	if (m_line) {
		m_line->Release();
		m_line = nullptr;
	}

	if (m_block) {
		m_block->Release();
		m_block = nullptr;
	}

	if (m_device) {
		m_device->Release();
		m_device = nullptr;
	}
}