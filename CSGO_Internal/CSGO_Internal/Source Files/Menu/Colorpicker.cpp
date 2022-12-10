#include "../../Header Files/Menu.hpp"

void UI::CColorpicker::draw(CWindowData* data)
{
	if (m_text && strlen(m_text) > 0)
	{
		const UIcolor_t text_color(206, 206, 206, data->m_alpha);
		const UIcolor_t text_shadow(0, 0, 0, data->m_alpha);

		UI::AddString(Font_t::FONT_MENU, text_shadow, m_start.x + 20.f + 1.f, m_start.y + 1.f - 3.f, false, m_text);
		UI::AddString(Font_t::FONT_MENU, text_color, m_start.x + 20.f, m_start.y - 3.f, false, m_text);
	}

	// picker outline.
	UI::AddRectangle(UIcolor_t(60, 62, 62, data->m_alpha), m_box_start.x - 1.f, m_box_start.y - 1.f, m_box_end.x, m_box_end.y);

	// inner box
	UIcolor_t box_color_top(temp_r, temp_g, temp_b, data->m_alpha);
	UIcolor_t box_color_bottom
	(
		box_color_top.R > 25 ? box_color_top.R - 25 : 0,
		box_color_top.G > 25 ? box_color_top.G - 25 : 0,
		box_color_top.B > 25 ? box_color_top.B - 25 : 0,
		data->m_alpha
	);

	UI::AddRectangleGradient(box_color_top, box_color_bottom, m_box_start.x, m_box_start.y, m_box_bounds.x, m_box_bounds.y);
}

void UI::CColorpicker::handle(CWindowData* data, const char* text, uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* a)
{
	g_input.InputGetCursorPos(m_cursor_x, m_cursor_y);

	m_text = text;
	m_start = vec2(data->m_x, data->m_y);

	m_box_start = vec2(data->m_x + 150.f, data->m_y);
	m_box_end = m_box_start + m_box_bounds;

	m_picker_start = vec2(m_box_start.x, m_box_start.y + 10);
	m_picker_end = m_picker_start + m_picker_bounds;

	temp_r = *r;
	temp_g = *g;
	temp_b = *b;

	// setup coord for next item
	data->m_y += 15.f;

	if (a)
	{
		m_alpha_value = (int)((float)(*a) / 2.55f);

		m_alpha_slider.handle(data, "", &m_alpha_value, 0, 100, L"%");

		m_alpha_value = std::clamp<int>(m_alpha_value, 0, 100);

		*a = (uint8_t)((float)(m_alpha_value) * 2.55f);
	}

	m_is_inside = m_cursor_x > m_box_start.x && m_cursor_x < m_box_end.x&& m_cursor_y > m_box_start.y && m_cursor_y < m_box_end.y;

	if (m_is_inside && data->m_first_click && !data->m_ignore)
	{
		m_is_open = true;
		data->m_ignore = true;
	}

	if (m_is_open)
	{
		// is the menu closed?
		// if so, then lets close the picker.
		// we dont want to select a color when the menu is closed.
		if (!g_ctx.bMenuOpened)
			m_is_open = false;

		if (m_cursor_x > m_picker_start.x && m_cursor_x < m_picker_end.x && m_cursor_y > m_picker_start.y && m_cursor_y < m_picker_end.y)
		{
			//set temp color
			float height = m_picker_bounds.y - 2;
			float half_height = height * 0.5f;

			float x_offset = m_cursor_x - (m_picker_start.x + 1);
			float y_offset = m_cursor_y - (m_picker_start.y + 1);

			float hue = x_offset / height;

			UIcolor_t color;

			if (y_offset > half_height)
			{
				float brightness = 1.f - std::clamp<float>((y_offset - half_height) / half_height, 0.f, 1.f);
				color = UIcolor_t::HsbToRgb(hue, 1.f, brightness);
			}
			else
			{
				float saturation = std::clamp<float>(y_offset / half_height, 0.f, 1.f);
				color = UIcolor_t::HsbToRgb(hue, saturation, 1.f);
			}

			temp_r = color.R;
			temp_g = color.G;
			temp_b = color.B;
		}

		if (!data->m_left_click)
		{
			*r = temp_r;
			*g = temp_g;
			*b = temp_b;

			m_is_open = false;
		}
	}

	draw(data);
}

void UI::CColorpicker::picker(CWindowData* data)
{
	if (!m_is_open)
		return;

	// outer edge
	UI::AddFilledRectangle(UIcolor_t(0, 0, 0, data->m_alpha), m_picker_start.x, m_picker_start.y, m_picker_end.x, m_picker_end.y);

	// picker
	int height = m_picker_bounds.y - 2;
	int half_height = height / 2;

	for (int i = 0; i < height; i++)
	{
		float hue = float(i) / float(height);

		UIcolor_t top = UIcolor_t::HsbToRgb(hue, 0.f, 1.f);
		UIcolor_t middle = UIcolor_t::HsbToRgb(hue, 1.f, 1.f);
		UIcolor_t bottom = UIcolor_t::HsbToRgb(hue, 1.f, 0.f);

		top.A = data->m_alpha;
		middle.A = data->m_alpha;
		bottom.A = data->m_alpha;

		UI::AddRectangleGradient(top, middle, m_picker_start.x + 1 + i, m_picker_start.y + 1, 1, half_height);
		UI::AddRectangleGradient(middle, bottom, m_picker_start.x + 1 + i, m_picker_start.y + 1 + half_height, 1, half_height);
	}
}