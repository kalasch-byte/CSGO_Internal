#include "../../Header Files/Menu.hpp"

bool UI::CWindow::handle(std::vector<std::string>& tabs, UIcolor_t color)
{
	if (g_input.IsKeyPressed(KEYS_INSERT) || g_input.IsKeyPressed(KEYS_DELETE))
	{
		if (!m_was_open)
			m_should_open = !m_should_open;

		m_was_open = true;
	}
	else
		m_was_open = false;

	// smoothly transition the menu in or out.
	LERP(m_data.m_alpha, m_should_open, ((float)g_config.m_iFadeSpeed / 40), 0, 255);

	if (g_config.m_bRainbowAccent)
		g_config.m_menuColor = (m_data.m_color = UI::UIcolor_t::GetRainbow(m_data.m_alpha)).ToRendererColor();
	else
		m_data.m_color = color;

	m_data.m_color.A = m_data.m_alpha;
	m_is_open = m_data.m_alpha > 0;

	// set our menu menu variable to the current state.
	// note - make into a pointer?
	g_ctx.bMenuOpened = m_should_open;

	// dim the screen background when the menu is opened.
	// important: must be done before the m_is_open check.
	LERP(m_data.m_test, m_should_open, ((float)g_config.m_iFadeSpeed / 40 + 0.10f), 0, 85);
	UI::AddFilledRectangle(UIcolor_t(0, 0, 0, m_data.m_test), 0, 0, g_ctx.iScreenWidth, g_ctx.iScreenHeight);

	if (!m_is_open)
		return false;

	g_input.InputGetCursorPos(m_cursor_x, m_cursor_y);

	m_is_inside = m_cursor_x > m_start.x && m_cursor_x < m_start.x + m_bounds.x && m_cursor_y > m_start.y && m_cursor_y < m_start.y + m_bounds.y;
	m_is_inside_edge = m_is_inside && ((m_cursor_x > m_start.x + m_bounds.x - m_edge_size || m_cursor_x < m_start.x + m_edge_size) || (m_cursor_y > m_start.y + m_bounds.y - m_edge_size || m_cursor_y < m_start.y + m_edge_size));

	// update data
	m_data.m_first_click = !m_data.m_left_click && g_input.IsKeyPressed(KEYS_MOUSE1);
	m_data.m_left_click = g_input.IsKeyPressed(KEYS_MOUSE1);
	m_data.m_right_click = g_input.IsKeyPressed(KEYS_MOUSE2);
	m_data.m_animation_progress = std::clamp<float>(m_data.m_animation_progress + 0.05f, 0.f, 1.f);

	if (!m_data.m_left_click)
		m_data.m_ignore = false;

	if (m_dragging && !m_data.m_left_click)
		m_dragging = false;

	if (m_data.m_first_click && m_is_inside_edge)
	{
		m_dragging = true;
		m_drag_x = m_cursor_x - m_start.x;
		m_drag_y = m_cursor_y - m_start.y;
	}

	if (m_dragging)
	{
		m_start.x = std::clamp(static_cast<float>(m_cursor_x - m_drag_x), 0.0f, static_cast<float>(g_ctx.iScreenWidth) - m_bounds.x);
		m_start.y = std::clamp(static_cast<float>(m_cursor_y - m_drag_y), 0.0f, static_cast<float>(g_ctx.iScreenHeight) - m_bounds.y);
	}

	m_end = m_start + m_bounds;

	// acquire the bottom left side of the gui.
	g_ctx.gui.BottomLeftOfMenu = { m_start.x + 27, m_start.y + m_window_size.y + 25 };

	// draw black menu outline.
	UI::AddRectangle(UIcolor_t(1, 1, 1, m_data.m_alpha), m_start.x, m_start.y, m_end.x - 1, m_end.y - 1);

	// draw inner grey outline.
	UI::AddRectangle(UIcolor_t(57, 57, 57, m_data.m_alpha), m_start.x + 1.f, m_start.y + 1.f, m_end.x - 2.f, m_end.y - 2.f);

	// draw the window border.
	UI::AddRectangle(UIcolor_t(35, 35, 35, std::clamp(m_data.m_alpha, 0, 155)), m_start.x + 2.f, m_start.y + 2.f, m_end.x - 3.f, m_end.y - 3.f);
	UI::AddRectangle(UIcolor_t(35, 35, 35, std::clamp(m_data.m_alpha, 0, 155)), m_start.x + 3.f, m_start.y + 3.f, m_end.x - 4.f, m_end.y - 4.f);
	UI::AddRectangle(UIcolor_t(35, 35, 35, std::clamp(m_data.m_alpha, 0, 155)), m_start.x + 4.f, m_start.y + 4.f, m_end.x - 5.f, m_end.y - 5.f);

	// draw inner grey outline 2.
	UI::AddRectangle(UIcolor_t(57, 57, 57, m_data.m_alpha), m_start.x + 5.f, m_start.y + 5.f, m_end.x - 6.f, m_end.y - 6.f);

	// draw transparent background.
	UI::AddFilledRectangle(UIcolor_t(21, 21, 21, std::clamp(m_data.m_alpha, 0, 220)), m_start.x + 6.f, m_start.y + 6.f, m_end.x - 6.f, m_end.y - 6.f);

	// draw main window.
	DrawBox(m_start.x + 20.f, m_start.y + 40.f, m_window_size.x, m_window_size.y);

	// tabs.
	const UIcolor_t text_color(180, 184, 190, m_data.m_alpha);
	const UIcolor_t text_color_shadow(0, 0, 0, m_data.m_alpha);

	const UIcolor_t tab_color(36, 36, 36, m_data.m_alpha);
	const UIcolor_t tab_color_shadow(0, 0, 0, m_data.m_alpha / 2);
	const UIcolor_t tab_color_selected = m_data.m_color;
	UIcolor_t		tab_color_hovered = tab_color_selected;
	tab_color_hovered.A /= 2;

	// spacing per tab.
	float tab_spacing = 3.f;

	// total used by spacing.
	float spacing = (tabs.size()) * tab_spacing;

	// tab width.
	float tab_width = (m_window_size.x - spacing) / tabs.size();

	for (size_t i = 0; i < tabs.size(); i++)
	{
		float tab_x = m_start.x + 20.f + (tab_width + tab_spacing) * i;
		float tab_y = m_start.y + 30.f;

		float text_x = tab_x + (tab_width * 0.5f);
		float text_y = tab_y - 14.f;

		UI::AddString(Font_t::FONT_MENU, text_color_shadow, text_x + 1.f, text_y + 1.f, true, tabs.at(i).c_str());
		UI::AddString(Font_t::FONT_MENU, text_color, text_x, text_y, true, tabs.at(i).c_str());

		UI::AddFilledRectangle(m_current_tab == i ? tab_color_selected : tab_color, tab_x, tab_y, tab_x + tab_width, tab_y + 4.f);
		UI::AddFilledRectangle(tab_color_shadow, tab_x, tab_y + 2.f, tab_x + tab_width, tab_y + 4.f);

		if (IsMouseInRegion(tab_x, tab_y - 16.f, tab_width, 20.f) && m_current_tab != i)
		{
			//select it if we are inside it and pressing
			if (m_data.m_first_click)
			{
				m_current_tab = i;
				m_data.m_animation_progress = 0.f;
			}
			else
			{
				UI::AddFilledRectangle(tab_color_hovered, tab_x, tab_y, tab_x + tab_width, tab_y + 4.f);
				UI::AddFilledRectangle(tab_color_shadow, tab_x, tab_y + 2.f, tab_x + tab_width, tab_y + 4.f);
			}
		}
	}

	return true;
}