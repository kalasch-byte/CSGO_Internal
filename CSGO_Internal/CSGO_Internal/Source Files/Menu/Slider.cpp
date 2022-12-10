#include "../../Header Files/Menu.hpp"

inline bool mouse_in_area(vec2 mouse_pos, const int x_pos, const int y_pos, const int width, const int height)
{
	return mouse_pos.x > x_pos && mouse_pos.y > y_pos && mouse_pos.x < width + x_pos && mouse_pos.y < height + y_pos;
};

void UI::CSlider::draw(CWindowData* data)
{
	const UIcolor_t text_color(206, 206, 206, data->m_alpha);
	const UIcolor_t text_shadow(0, 0, 0, data->m_alpha);

	// label.
	if (m_text && strlen(m_text) > 0)
	{
		UI::AddString(Font_t::FONT_MENU, text_shadow, m_start.x + 20.f + 1.f, m_start.y + 1.f - 3.f, false, m_text);
		UI::AddString(Font_t::FONT_MENU, text_color, m_start.x + 20.f, m_start.y - 3.f, false, m_text);
	}

	// decrement label.
	{
		if (*m_value > m_min)
		{
			UI::AddString(Font_t::FONT_MENU, text_color, m_slider_start.x - 17.f, m_slider_start.y - 3.f, false, "-");

			bool hovering_decrement_label = mouse_in_area({ m_cursor_x, m_cursor_y }, m_slider_start.x - 22.f, m_slider_start.y - 3.f, 10, 8);

			if (hovering_decrement_label && g_input.IsKeyPressed(KEYS_MOUSE1) && !data->m_ignore)
			{
				*m_value -= 1;
				data->m_ignore = true;
			}
		}
	}

	// increment label.
	{
		if (*m_value < m_max)
		{
			UI::AddString(Font_t::FONT_MENU, text_color, m_slider_start.x + m_bounds.x + 12, m_slider_start.y - 3.f, false, "+");

			bool hovering_increment_label = mouse_in_area({ m_cursor_x, m_cursor_y }, m_slider_start.x + m_bounds.x + 10, m_slider_start.y - 3.f, 10, 10);

			if (hovering_increment_label && g_input.IsKeyPressed(KEYS_MOUSE1) && !data->m_ignore)
			{
				*m_value += 1;
				data->m_ignore = true;
			}
		}
	}

	// blob outline.
	UI::AddRectangle(UIcolor_t(0, 0, 0, data->m_alpha), m_slider_start.x - 1.f, m_slider_start.y - 1.f, m_end.x, m_end.y);

	// idle blob.
	{
		const UIcolor_t background_color_top(69, 69, 69, data->m_alpha);
		const UIcolor_t background_color_bottom(44, 44, 44, data->m_alpha);

		UI::AddRectangleGradient(background_color_top, background_color_bottom, m_slider_start.x, m_slider_start.y, m_bounds.x, m_bounds.y);
	}

	// color fill blob.
	{
		UIcolor_t slider_color_top = data->m_color;
		UIcolor_t slider_color_bottom(slider_color_top.R > 25 ? slider_color_top.R - 25 : 0,
									  slider_color_top.G > 25 ? slider_color_top.G - 25 : 0,
									  slider_color_top.B > 25 ? slider_color_top.B - 25 : 0);

		// draw blob.
		UI::AddRectangleGradient(UIcolor_t(slider_color_top.R, slider_color_top.G, slider_color_top.B, data->m_alpha),
								 UIcolor_t(slider_color_bottom.R, slider_color_bottom.G, slider_color_bottom.B, data->m_alpha),
								 m_slider_start.x, m_slider_start.y, m_slider_pos, m_bounds.y);
	}

	// value label on the slider.
	UI::AddString(Font_t::FONT_MENU, UIcolor_t(0, 0, 0, data->m_alpha), m_slider_start.x + m_slider_pos + 1.f, m_slider_start.y + 1.f, true, std::to_string(*m_value).c_str());
	UI::AddString(Font_t::FONT_MENU, UIcolor_t(255, 255, 255, data->m_alpha), m_slider_start.x + m_slider_pos, m_slider_start.y, true, std::to_string(*m_value).c_str());
}

void UI::CSlider::handle(CWindowData* data, const char* name, int* setting, int min, int max, const wchar_t* suffix)
{
	g_input.InputGetCursorPos(m_cursor_x, m_cursor_y);

	m_text = strlen(name) > 1 ? name : nullptr;
	m_min = min;
	m_max = max;

	m_value = setting;
	m_suffix = suffix;

	m_start = vec2(data->m_x, data->m_y);
	m_slider_start = vec2(m_start.x + 20.f, m_start.y);
	m_end = m_slider_start + m_bounds;

	data->m_y += 15.f;

	if (m_text)
	{
		m_slider_start.y += 15.f;
		m_end.y += 15.f;
		data->m_y += 15.f;
	}

	m_is_inside = m_cursor_x > m_slider_start.x - 1.0f && m_cursor_x < m_end.x + 1.0f && m_cursor_y > m_slider_start.y - 1.0f && m_cursor_y < m_end.y + 1.0f;

	if (m_dragging && !data->m_left_click)
		m_dragging = false;

	if (m_value)
	{
		*m_value = std::clamp< int >(*m_value, m_min, m_max);

		if (m_dragging)
		{
			float dragged_pos = std::clamp< float >(m_cursor_x - m_slider_start.x, 0, m_slider_width);
			*m_value = m_min + ((m_max - m_min) * dragged_pos / m_slider_width);
		}

		if (data->m_first_click && m_is_inside)
		{
			m_dragging = true;
			data->m_ignore = true;
		}

		m_slider_pos = (float(*m_value - m_min) / float(m_max - m_min) * m_slider_width);
		m_slider_pos = std::clamp<float>(m_slider_pos, 0.f, m_slider_width * data->m_animation_progress);
	}

	draw(data);
}