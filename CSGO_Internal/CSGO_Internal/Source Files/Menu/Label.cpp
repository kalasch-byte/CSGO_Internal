#include "../../Header Files/Menu.hpp"

void UI::CLabel::handle(CWindowData* data, const char* text)
{
	m_text = text;
	m_start = vec2(data->m_x, data->m_y);

	// setup coord for next item
	data->m_y += 15.f;

	draw(data);
}

void UI::CLabel::draw(CWindowData* data)
{
	UIcolor_t text_color(206, 206, 206, data->m_alpha);
	UIcolor_t text_shadow(0, 0, 0, data->m_alpha);

	if (m_text)
	{
		UI::AddString(Font_t::FONT_MENU, text_shadow, m_start.x + 20.f + 1.f, m_start.y + 1.f - 3.f, false, m_text);
		UI::AddString(Font_t::FONT_MENU, text_color, m_start.x + 20.f, m_start.y - 3.f, false, m_text);
	}
}