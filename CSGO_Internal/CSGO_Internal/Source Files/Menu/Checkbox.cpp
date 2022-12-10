#include "../../Header Files/Menu.hpp"

void UI::CCheckbox::draw(CWindowData* data)
{
	// label.
	if (m_text && strlen(m_text) > 0)
	{
		const UIcolor_t textColor(207, 207, 207, data->m_alpha);
		const UIcolor_t textShadow(0, 0, 0, data->m_alpha);

		UI::AddString(Font_t::FONT_MENU, textShadow, m_start.x + 20.f + 1.f, m_start.y + 1.f - 3.f, false, m_text);
		UI::AddString(Font_t::FONT_MENU, textColor, m_start.x + 20.f, m_start.y - 3.f, false, m_text);
	}

	// outer edge
	UI::AddFilledRectangle(UIcolor_t(0, 0, 0, data->m_alpha), m_start.x, m_start.y, m_start.x + m_bounds.x, m_start.y + m_bounds.y);

	// tooltip.
	if (m_is_inside && m_tooltip.length() > 0)
	{
		char buf[256];
		sprintf_s(buf, "%s - %s", m_text, m_tooltip.c_str());

		UI::AddString(Font_t::FONT_MENU, UIcolor_t(207, 207, 207, data->m_alpha), g_ctx.gui.BottomLeftOfMenu.x, g_ctx.gui.BottomLeftOfMenu.y, false, buf);
	}

	// inner box empty
	UIcolor_t emptyColorTop = UIcolor_t(m_is_inside ? 83 : 69, m_is_inside ? 83 : 69, m_is_inside ? 83 : 69, data->m_alpha);
	UIcolor_t emptyColorBottom
	(
		emptyColorTop.R > 25 ? emptyColorTop.R - 25 : 0,
		emptyColorTop.G > 25 ? emptyColorTop.G - 25 : 0,
		emptyColorTop.B > 25 ? emptyColorTop.B - 25 : 0,
		data->m_alpha
	);

	UI::AddRectangleGradient(emptyColorTop, emptyColorBottom, m_start.x + 1.f, m_start.y + 1.f, m_bounds.x - 2.f, m_bounds.y - 2.f);

	// inner box
	if (*m_value)
	{
		int animAlpha = (float(data->m_alpha) * data->m_animation_progress);

		UIcolor_t checkboxColorTop = data->m_color;
		UIcolor_t checkboxColorBottom
		(
			checkboxColorTop.R > 25 ? checkboxColorTop.R - 25 : 0,
			checkboxColorTop.G > 25 ? checkboxColorTop.G - 25 : 0,
			checkboxColorTop.B > 25 ? checkboxColorTop.B - 25 : 0,
			animAlpha
		);

		UI::AddRectangleGradient(checkboxColorTop, checkboxColorBottom, m_start.x + 1.f, m_start.y + 1.f, m_bounds.x - 2.f, m_bounds.y - 2.f);
	}
}

void UI::CCheckbox::handle(CWindowData* data, const char* name, bool* setting, std::string tooltip)
{
	g_input.InputGetCursorPos(m_cursor_x, m_cursor_y);

	// setup data.
	m_text = name;
	m_value = setting;
	m_tooltip = tooltip;

	m_start = vec2(data->m_x, data->m_y);
	m_end = m_start + m_bounds;

	// setup coord for next item
	data->m_y += 15.f;

	float textWidth = m_text ? UI::GetTextWidth(Font_t::FONT_MENU, m_text) : 0.f;

	m_is_inside = m_cursor_x > m_start.x && m_cursor_x < m_start.x + 20.f + textWidth + 5.f && m_cursor_y > m_start.y - 3.f && m_cursor_y < m_end.y + 3.f;

	if (m_value && data->m_left_click && !data->m_ignore && m_is_inside)
	{
		*m_value = !*m_value;
		data->m_ignore = true;
	}

	draw(data);
}