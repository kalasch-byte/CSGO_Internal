#include "../../Header Files/Menu.hpp"

void UI::CButton::draw(CWindowData* data)
{
	// outline.
	UI::AddRectangle(UIcolor_t(0, 0, 0, data->m_alpha), m_start.x - 1.f, m_start.y - 1.f, m_end.x, m_end.y);

	const bool hovering = m_cursor_x > m_start.x && m_cursor_x < m_end.x&& m_cursor_y > m_start.y && m_cursor_y < m_end.y;
	const bool clicking = m_is_inside && data->m_left_click;

	// idle -> 36, 36, 36
	// hover -> 51, 51, 51
	// click -> 72, 72, 72

	const UIcolor_t blobColor = clicking ? UIcolor_t(72, 72, 72, data->m_alpha) : hovering ? UIcolor_t(51, 51, 51, data->m_alpha) : UIcolor_t(36, 36, 36, data->m_alpha);

	// blob fill.
	UI::AddFilledRectangle(blobColor, m_start.x, m_start.y, m_end.x, m_end.y);

	// tooltip.
	if (m_is_inside && m_tooltip.length() > 0)
	{
		char buf[256];
		sprintf_s(buf, "%s - %s", m_text, m_tooltip.c_str());

		UI::AddString(Font_t::FONT_MENU, UIcolor_t(207, 207, 207, data->m_alpha), g_ctx.gui.BottomLeftOfMenu.x, g_ctx.gui.BottomLeftOfMenu.y, false, buf);
	}

	// draw button label.
	if (m_text && strlen(m_text) > 0)
	{
		UIcolor_t textColor = m_is_inside && data->m_left_click ? UIcolor_t(g_config.m_menuColor.r(), g_config.m_menuColor.g(), g_config.m_menuColor.b(), data->m_alpha) : UIcolor_t(153, 153, 153, data->m_alpha);

		UI::AddString(Font_t::FONT_MENU, textColor, m_start.x + m_bounds.x * 0.5f, m_start.y + (m_bounds.y * 0.5f) - 6.0f, true, m_text);
	}
}

void UI::CButton::handle(CWindowData* data, const char* text, std::string tooltip, std::function< void() > function)
{
	g_input.InputGetCursorPos(m_cursor_x, m_cursor_y);

	// setup data.
	m_text = text;
	m_tooltip = tooltip;
	m_start = vec2(data->m_x + 20.f, data->m_y);
	m_bounds = vec2(150.f, 20.f);
	m_end = m_start + m_bounds;

	// spacing.
	data->m_y += 25.f;

	m_is_inside = m_cursor_x > m_start.x && m_cursor_x < m_end.x&& m_cursor_y > m_start.y && m_cursor_y < m_end.y;

	if (data->m_left_click && !data->m_ignore && m_is_inside && function)
	{
		function();
		data->m_ignore = true;
	}

	draw(data);
}