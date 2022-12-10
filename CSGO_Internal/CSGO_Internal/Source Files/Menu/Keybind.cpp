#include "../../Header Files/Menu.hpp"

void UI::CKeybind::handle(CWindowData* data, const char* text, int* key)
{
	g_input.InputGetCursorPos(m_cursor_x, m_cursor_y);

	m_text = text;
	m_key = key;

	m_start = vec2(data->m_x + 20.f, data->m_y);
	m_box_start = vec2(m_start.x, m_start.y + 12.f);

	data->m_y += 40.f;

	m_bounds = vec2(150, 20);
	m_end = m_box_start + m_bounds;

	m_is_inside = m_cursor_x > m_box_start.x && m_cursor_x < m_end.x&& m_cursor_y > m_box_start.y && m_cursor_y < m_end.y;

	if (data->m_left_click && !data->m_ignore && m_is_inside)
	{
		m_is_waiting = true;
		data->m_ignore = true;
	}

	if (m_is_waiting)
	{
		*m_key = KEYS_NONE;

		VirtualKeys_t start_key = data->m_ignore ? KEYS_MOUSE1 : KEYS_NONE;

		if (g_input.IsAnyKeyPressed() == KEYS_ESCAPE)
		{
			m_is_waiting = false;
			*m_key = KEYS_NONE;
		}
		else if (g_input.IsAnyKeyPressed() > start_key)
		{
			*m_key = g_input.IsAnyKeyPressed();
			m_is_waiting = false;
			data->m_ignore = true;
		}
	}

	draw(data);
}

void UI::CKeybind::draw(CWindowData* data)
{
	if (m_text)
	{
		UIcolor_t text_color(206, 206, 206, data->m_alpha);
		UIcolor_t text_shadow(0, 0, 0, data->m_alpha);

		UI::AddString(Font_t::FONT_MENU, text_shadow, m_start.x + 1.f, m_start.y + 1.f - 3.f, false, m_text); // ???
		UI::AddString(Font_t::FONT_MENU, text_color, m_start.x, m_start.y - 3.f, false, m_text);
	}

	const bool hovering = m_cursor_x > m_start.x && m_cursor_x < m_end.x&& m_cursor_y > m_start.y && m_cursor_y < m_end.y;
	const bool clicking = m_is_inside && data->m_left_click;

	// idle -> 36, 36, 36
	// hover -> 51, 51, 51
	// click -> 72, 72, 72

	const UIcolor_t blob_color = clicking ? UIcolor_t(72, 72, 72, data->m_alpha) : hovering ? UIcolor_t(51, 51, 51, data->m_alpha) : UIcolor_t(36, 36, 36, data->m_alpha);

	UI::AddRectangle(UIcolor_t(0, 0, 0, data->m_alpha), m_box_start.x - 1.f, m_box_start.y - 1.f, m_end.x + 1.f, m_end.y + 1.f);
	UI::AddFilledRectangle(blob_color, m_box_start.x, m_box_start.y, m_end.x, m_end.y);

	// note - to do; hold, toggle, always.

	if (m_is_waiting)
		UI::AddString(Font_t::FONT_MENU, data->m_color, m_box_start.x + 10.0f, m_box_start.y + 3.f, false, ("press key"));
	else if (*m_key)
		UI::AddString(Font_t::FONT_MENU, UIcolor_t(153, 153, 153, data->m_alpha), m_box_start.x + 10.f, m_box_start.y + 3.f, false, g_input.GetKeyName(*reinterpret_cast<VirtualKeys_t*>(m_key)));
}