#include "../../Header Files/Menu.hpp"

void UI::CGroupbox::draw(CWindowData* data)
{
	UIcolor_t text_color(207, 207, 207, data->m_alpha);
	UIcolor_t shadow_color(0, 0, 0, data->m_alpha);
	UIcolor_t edge_color(44, 44, 44, data->m_alpha);

	float width = 211.f;
	float text_width = UI::GetTextWidth(Font_t::FONT_MENU, m_text);

	// group label.
	if (m_text && strlen(m_text) > 0)
	{
		UI::AddString(Font_t::FONT_MENU, shadow_color, m_start.x + 10.f + 1.f, m_start.y + 1.f - 6.f, false, m_text);
		UI::AddString(Font_t::FONT_MENU, text_color, m_start.x + 10.f, m_start.y - 6.f, false, m_text);
	}

	// top
	UI::AddLine(edge_color, m_start.x, m_start.y, m_start.x + 8.f, m_start.y);
	UI::AddLine(edge_color, m_start.x + 12.f + text_width, m_start.y, m_start.x + width, m_start.y);

	// left
	UI::AddLine(edge_color, m_start.x, m_start.y, m_end.x, m_end.y);

	// right
	UI::AddLine(edge_color, m_start.x + width, m_start.y, m_end.x + width, m_end.y + 1.f);
	UI::AddLine(shadow_color, m_start.x + width + 1.f, m_start.y, m_end.x + width + 1.f, m_end.y + 1.f);

	// bottom
	UI::AddLine(edge_color, m_end.x, m_end.y, m_end.x + width + 1.f, m_end.y);
	UI::AddLine(shadow_color, m_end.x, m_end.y + 1.f, m_end.x + width + 2.f, m_end.y + 1.f);
}