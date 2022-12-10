#include "../../Header Files/Menu.hpp"

void UI::CDropdown::handle(CWindowData* data, const char* text, const std::vector<std::string>& items, int* setting)
{
	g_input.InputGetCursorPos(m_cursor_x, m_cursor_y);

	m_text = strlen(text) > 0 ? text : nullptr;
	m_items = items;
	m_setting = setting;

	m_start = vec2(data->m_x + 20.f, data->m_y);
	m_box_start = vec2(m_start.x, m_start.y);

	data->m_y += 25.f;

	if (m_text)
	{
		m_box_start.y += 15.f;
		data->m_y += 15.f;
	}

	m_bounds = vec2(150, 20);

	m_end = m_box_start + m_bounds;
	m_box_end = m_end;

	if (m_is_open)
	{
		float h = items.size() * m_item_height;
		m_box_end.y += h + 3.f;
	}

	m_is_inside = m_cursor_x > m_box_start.x && m_cursor_x < m_end.x&& m_cursor_y > m_box_start.y && m_cursor_y < m_end.y;
	m_is_inside_items = m_cursor_x > m_box_start.x && m_cursor_x < m_box_end.x&& m_cursor_y > m_box_start.y && m_cursor_y < m_box_end.y;

	if (data->m_left_click && !data->m_ignore)
	{
		if (m_is_inside)
		{
			m_is_open = !m_is_open;
			data->m_ignore = true;
		}
		else if (m_is_open && m_is_inside_items)
		{
			float offset = m_cursor_y - m_end.y;
			*m_setting = (int)(offset / m_item_height);
			data->m_ignore = true;

			m_is_open = false;
		}
		else
		{
			m_is_open = false;
		}
	}

	// clamp setting to avoid crashing.
	*m_setting = std::clamp<int>(*m_setting, 0, std::max<int>(0, items.size() - 1));

	draw(data);
}

void UI::CDropdown::draw(CWindowData* data)
{
	// preview label.
	if (m_text && strlen(m_text) > 0)
	{
		const UIcolor_t text_color(206, 206, 206, data->m_alpha);
		const UIcolor_t text_shadow(0, 0, 0, data->m_alpha);

		UI::AddString(Font_t::FONT_MENU, text_shadow, m_start.x + 1.f, m_start.y + 1.f - 3.f, false, m_text);
		UI::AddString(Font_t::FONT_MENU, text_color, m_start.x, m_start.y - 3.f, false, m_text);
	}

	// blob outline
	UI::AddRectangle(UIcolor_t(0, 0, 0, data->m_alpha), m_box_start.x - 1.f, m_box_start.y - 1.f, m_end.x, m_end.y);

	// hovering and not dropped down.
	bool should_change_color = m_is_inside && !m_is_open;

	UIcolor_t col = should_change_color ? UIcolor_t(50, 50, 50, data->m_alpha) : UIcolor_t(36, 36, 36, data->m_alpha);

	// inner blob.
	UI::AddFilledRectangle(col, m_box_start.x, m_box_start.y, m_end.x, m_end.y);

	// inner blob label.
	UI::AddString(Font_t::FONT_MENU, UIcolor_t(153, 153, 153, data->m_alpha), m_box_start.x + 10.f, m_box_start.y + 3.f, false, m_items.at(*m_setting).c_str());

	// polygon.
	const UIcolor_t arrow_color(153, 153, 153, data->m_alpha);

	float x = m_end.x - 11.f;
	float y = m_box_start.y + 9.f;

	// draw inner blob polygon.
	m_is_open ? UI::AddLine(arrow_color, x + 2.f, y, x + 3.f, y) : UI::AddLine(arrow_color, x + 2.f, y + 2.f, x + 3.f, y + 2.f);
	m_is_open ? UI::AddLine(arrow_color, x + 1.f, y + 1.f, x + 4.f, y + 1.f) : UI::AddLine(arrow_color, x + 1.f, y + 1.f, x + 4.f, y + 1.f);
	m_is_open ? UI::AddLine(arrow_color, x, y + 2.f, x + 5.f, y + 2.f) : UI::AddLine(arrow_color, x, y, x + 5.f, y);
}

void UI::CDropdown::dropdown(CWindowData* data)
{
	// get the current game's framerate.
	const int fps = 1.f / (g_csgo.m_globals->m_frametime);

	if (m_is_open)
	{
		// if we've closed the menu, close the drop down.
		if (!g_ctx.bMenuOpened)
		{
			m_dropdown_end = 0.f;
			m_is_open = false;

			return;
		}

		// dynamic dropdown speed based on FPS.
		m_dropdown_end += fps <= 63 ? 5.7f : 0.f;
		m_dropdown_end += fps >= 112 && fps < 136 ? 3.2f : 0.f;
		m_dropdown_end += fps >= 136 && fps < 218 ? 1.7f : 0.f;
		m_dropdown_end += fps >= 215 && fps < 300 ? 0.9f : 0.f;
		m_dropdown_end += fps >= 300 && fps < 400 ? 0.7f : 0.f;
		m_dropdown_end += fps >= 400 ? 0.6f : 0.f;

		if (m_dropdown_end + m_end.y > m_box_end.y)
			m_dropdown_end = m_box_end.y - m_end.y;
	}
	else
	{
		if (m_dropdown_end > 0.f)
		{
			// dynamic dropdown speed based on FPS.
			m_dropdown_end -= fps <= 63 ? 5.7f : 0.f;
			m_dropdown_end -= fps >= 112 && fps < 136 ? 3.2f : 0.f;
			m_dropdown_end -= fps >= 136 && fps < 218 ? 1.7f : 0.f;
			m_dropdown_end -= fps >= 215 && fps < 300 ? 0.9f : 0.f;
			m_dropdown_end -= fps >= 300 && fps < 400 ? 0.7f : 0.f;
			m_dropdown_end -= fps >= 400 ? 0.6f : 0.f;
		}

		if (m_dropdown_end <= 0.f)
			m_dropdown_end = 0.f;
	}

	// closed!
	if (m_dropdown_end <= 0.f)
		return;

	UI::AddFilledRectangle(UIcolor_t(0, 0, 0), m_box_start.x - 1.f, m_end.y + 2.f, m_box_end.x + 1.f, m_end.y + 2.f + m_dropdown_end);
	UI::AddFilledRectangle(UIcolor_t(36, 36, 36), m_box_start.x, m_end.y + 2.f + 1.f, m_box_end.x, m_end.y + 2.f + m_dropdown_end - 1.f);

	for (size_t i = 0; i < m_items.size(); i++)
	{
		float x = m_box_start.x + 10.f;
		float y = m_end.y + 3.f + i * m_item_height;

		if (y + (m_item_height * 0.5f) > m_end.y + m_dropdown_end)
			continue;

		UI::AddString(Font_t::FONT_MENU, *m_setting == i ? data->m_color : UIcolor_t(153, 153, 153), x, y, false, m_items.at(i).c_str());
	}
}