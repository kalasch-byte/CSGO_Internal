#include "../Header Files/Renderer/Renderer.hpp"

#include <stdio.h>
#include <limits>

CRender::CRender(IDirect3DDevice9* device)
{
	m_device = device;

	create();
}

void CRender::create()
{
	m_device->CreateStateBlock(D3DSBT_ALL, &m_state_block);

	D3DXCreateSprite(m_device, &m_sprite);
	D3DXCreateFontA(m_device, 12, 0, FW_NORMAL, 0, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, DEFAULT_PITCH, ("Tahoma"), &m_menu);
	D3DXCreateFontA(m_device, 14, 0, FW_NORMAL, 0, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, ("Segoe UI"), &m_esp);
	D3DXCreateFontA(m_device, 11, 0, FW_NORMAL, 0, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, ("Tahoma"), &m_esp_small);
	D3DXCreateFontA(m_device, 28, 0, FW_NORMAL, 0, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, ("Impact"), &m_other);
}

void CRender::release()
{
	if (m_state_block)
		m_state_block->Release();

	if (m_sprite)
		m_sprite->Release();

	if (m_menu)
		m_menu->Release();

	if (m_esp)
		m_esp->Release();

	if (m_esp_small)
		m_esp_small->Release();

	if (m_other)
		m_other->Release();
}

void CRender::start()
{
	if (!m_state_block || !m_device || !m_sprite)
		return;

	m_verts.clear();
	m_types.clear();

	m_state_block->Capture();

	m_sprite->Begin(D3DXSPRITE_ALPHABLEND);

	m_device->GetFVF(&m_old_fvf);
	m_device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	m_device->SetPixelShader(nullptr);
	m_device->SetTexture(0, nullptr);

	m_device->SetRenderState(D3DRS_ZENABLE, false);
	m_device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xFFFFFFFF);
	m_device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, false);
}

void CRender::finish()
{
	if (!m_state_block || !m_device || !m_sprite)
		return;

	m_device->SetFVF(m_old_fvf);
	m_state_block->Apply();
	m_sprite->End();
	m_device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, true);
}

// big boss doesn't fuck with implementation 
void CRender::draw()
{
	// idk if u can pass 0 to usage
	m_device->CreateVertexBuffer(sizeof(vertex_t) * m_verts.size(), 0, m_old_fvf, D3DPOOL_MANAGED, &m_buffer, 0);
	m_device->CreateIndexBuffer(sizeof(vertex_t) * m_verts.size(), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_indexed_buffer, 0);

	m_device->SetStreamSource(0, m_buffer, 0, sizeof(vertex_t));
	m_device->SetIndices(m_indexed_buffer);

	m_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 5, 0, 6);

	// keep u free from sin 
	for (size_t i = 0; i < m_verts.size(); i++) {}
}

void CRender::line(color_t color, float x1, float y1, float x2, float y2)
{
	vertex_t vertices[2] =
	{
		vertex_t(x1, y1, 1.0f, color),
		vertex_t(x2, y2, 1.0f, color)
	};

	m_device->DrawPrimitiveUP(D3DPT_LINELIST, 1, vertices, sizeof(vertex_t));
}

void CRender::rect(color_t color, float x, float y, float x1, float y1)
{
	vertex_t vertices[5] =
	{
		vertex_t(x, y, 1.0f, color),
		vertex_t(x1, y, 1.0f, color),
		vertex_t(x1, y1, 1.0f, color),
		vertex_t(x, y1, 1.0f, color),
		vertex_t(x, y, 1.0f, color)
	};

	m_device->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, vertices, sizeof(vertex_t));
}

void CRender::filled_rect(color_t color, float x, float y, float x1, float y1)
{
	vertex_t verts[4] =
	{
		vertex_t(x, y, 1.0f, color),
		vertex_t(x1, y, 1.0f, color),
		vertex_t(x, y1, 1.0f, color),
		vertex_t(x1, y1, 1.0f, color),
	};

	m_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, verts, sizeof(vertex_t));
}

void CRender::_filled_rect(color_t color, float x, float y, float w, float h)
{
	vertex_t verts[4] =
	{
		vertex_t(x, y, 1.0f, color),
		vertex_t(x + w, y, 1.0f, color),
		vertex_t(x, y + h, 1.0f, color),
		vertex_t(x + w, y + h, 1.0f, color),
	};

	m_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, verts, sizeof(vertex_t));
}

void CRender::rect_gradient(Gradient_t type, color_t start, color_t end, float x, float y, float w, float h)
{
	vertex_t vertice[4] =
	{
		vertex_t(x, y, 1.0f, start),
		vertex_t(x + w, y, 1.0f, type == Gradient_t::VERTICAL ? start : end),
		vertex_t(x, y + h, 1.0f, type == Gradient_t::VERTICAL ? end : start),
		vertex_t(x + w, y + h, 1.0f, end),
	};

	m_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &vertice, sizeof(vertex_t));
}

float CRender::text_width(Font_t _font, const char* fmt, ...)
{
	char buffer[256];
	va_list args;

	va_start(args, fmt);
	vsprintf(buffer, fmt, args);
	va_end(args);

	ID3DXFont* font;

	switch (_font)
	{
		case Font_t::FONT_MENU:
			font = m_menu;
			break;

		case Font_t::FONT_ESP:
			font = m_esp;
			break;

		case Font_t::FONT_ESP_SMALL:
			font = m_esp_small;
			break;

		case Font_t::FONT_OTHER:
			font = m_other;
			break;

		default:
			font = m_menu;
			break;
	}

	if (!font)
		return 0.f;

	RECT text_size{ 0, 0, 0, 0 };
	font->DrawTextA(0, buffer, -1, &text_size, DT_CALCRECT | DT_NOCLIP, D3DCOLOR_XRGB(0, 0, 0));

	return (float)(text_size.right - text_size.left);
}

void CRender::string(Font_t _font, color_t color, float x, float y, bool centered, const char* fmt, ...)
{
	char buffer[256];
	va_list args;

	va_start(args, fmt);
	vsprintf(buffer, fmt, args);
	va_end(args);

	ID3DXFont* font;

	switch (_font)
	{
		case Font_t::FONT_MENU:
			font = m_menu;
			break;

		case Font_t::FONT_ESP:
			font = m_esp;
			break;

		case Font_t::FONT_ESP_SMALL:
			font = m_esp_small;
			break;

		case Font_t::FONT_OTHER:
			font = m_other;
			break;

		default:
			font = m_menu;
			break;
	}

	if (!font)
		return;

	if (centered)
	{
		RECT rec = { 0, 0, 0, 0 };

		font->DrawTextA(0, buffer, -1, &rec, DT_CALCRECT | DT_NOCLIP, D3DCOLOR_RGBA(color.r(), color.g(), color.b(), color.a()));

		rec = { static_cast<int>(x) - rec.right / 2, static_cast<int>(y), 0, 0 };

		font->DrawTextA(0, buffer, -1, &rec, DT_TOP | DT_LEFT | DT_NOCLIP, D3DCOLOR_RGBA(color.r(), color.g(), color.b(), color.a()));
	}
	else
	{
		RECT rec = { static_cast<int>(x), static_cast<int>(y), 1000, 100 };

		font->DrawTextA(NULL, buffer, -1, &rec, DT_TOP | DT_LEFT | DT_NOCLIP, D3DCOLOR_RGBA(color.r(), color.g(), color.b(), color.a()));
	}
}

void CRender::_string(Font_t _font, color_t color, float x, float y, bool centered, const char* fmt, ...)
{
	char buffer[256];
	va_list args;

	va_start(args, fmt);
	vsprintf(buffer, fmt, args);
	va_end(args);

	ID3DXFont* font;

	switch (_font)
	{
		case Font_t::FONT_MENU:
			font = m_menu;
			break;

		case Font_t::FONT_ESP:
			font = m_esp;
			break;

		case Font_t::FONT_ESP_SMALL:
			font = m_esp_small;
			break;

		case Font_t::FONT_OTHER:
			font = m_other;
			break;

		default:
			font = m_menu;
			break;
	}

	if (!font)
		return;

	if (centered)
	{
		RECT rec = { 0, 0, 0, 0 };

		font->DrawTextA(0, buffer, -1, &rec, DT_CALCRECT | DT_NOCLIP, D3DCOLOR_RGBA(color.r(), color.g(), color.b(), color.a()));

		rec = { static_cast<int>(x) - rec.right / 2, static_cast<int>(y), 0, 0 };

		font->DrawTextA(0, buffer, -1, &rec, DT_TOP | DT_LEFT | DT_NOCLIP, D3DCOLOR_RGBA(color.r(), color.g(), color.b(), color.a()));
	}
	else
	{
		RECT rec = { static_cast<int>(x), static_cast<int>(y), 1000, 100 };

		font->DrawTextA(NULL, buffer, -1, &rec, DT_TOP | DT_LEFT | DT_NOCLIP, D3DCOLOR_RGBA(color.r(), color.g(), color.b(), color.a()));
	}
}

void CRender::wide_string(Font_t _font, color_t color, float x, float y, bool centered, const wchar_t* fmt, ...)
{
	wchar_t buffer[256];
	va_list args;

	va_start(args, fmt);
	vswprintf(buffer, 256, fmt, args);
	va_end(args);

	ID3DXFont* font;

	switch (_font)
	{
		case Font_t::FONT_MENU:
			font = m_menu;
			break;

		case Font_t::FONT_ESP:
			font = m_esp;
			break;

		case Font_t::FONT_ESP_SMALL:
			font = m_esp_small;
			break;

		case Font_t::FONT_OTHER:
			font = m_other;
			break;

		default:
			font = m_menu;
			break;
	}

	if (!font)
		return;

	if (centered)
	{
		RECT rec = { 0, 0, 0, 0 };

		font->DrawTextW(0, buffer, -1, &rec, DT_CALCRECT | DT_NOCLIP, D3DCOLOR_RGBA(color.r(), color.g(), color.b(), color.a()));

		rec = { static_cast<int>(x) - rec.right / 2, static_cast<int>(y), 0, 0 };

		font->DrawTextW(0, buffer, -1, &rec, DT_TOP | DT_LEFT | DT_NOCLIP, D3DCOLOR_RGBA(color.r(), color.g(), color.b(), color.a()));
	}
	else
	{
		RECT rec = { static_cast<int>(x), static_cast<int>(y), 1000, 100 };

		font->DrawTextW(NULL, buffer, -1, &rec, DT_TOP | DT_LEFT | DT_NOCLIP, D3DCOLOR_RGBA(color.r(), color.g(), color.b(), color.a()));
	}
}

void CRender::image(uint32_t x, uint32_t y, uint32_t w, uint32_t h, LPDIRECT3DTEXTURE9 texture, LPD3DXSPRITE sprite, uint8_t* image, const uint32_t size, bool setup)
{
	if (setup)
	{
		D3DXCreateTextureFromFileInMemoryEx(m_device, image, size, w, h, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, 0, &texture);
		D3DXCreateSprite(m_device, &sprite);
	}

	sprite->Begin(D3DXSPRITE_ALPHABLEND);
	sprite->Draw(texture, 0, 0, 0, 0xFFFFFFFF);
	sprite->End();
}