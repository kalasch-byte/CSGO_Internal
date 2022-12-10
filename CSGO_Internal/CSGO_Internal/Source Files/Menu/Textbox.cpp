#include "../../Header Files/Menu.hpp"

static bool key_state[256];
static bool prev_key_state[256];
static bool input_loop_started = false;

void poll_input(std::string_view window_name)
{
	if (window_name.empty())
		throw std::exception("No window from where input should be read from specified in function parameter.");

	for (int i = 0; i < 256; i++)
	{
		prev_key_state[i] = key_state[i];
		key_state[i] = GetAsyncKeyState(i);
	}

	input_loop_started = true;
}

bool key_pressed(const int key)
{
	return key_state[key] && !prev_key_state[key];
}

bool key_down(const int key)
{
	return key_state[key];
}

bool key_released(const int key)
{
	return !key_state[key] && prev_key_state[key];
}

void UI::CTextbox::handle(CWindowData* data, const char* text, std::string* string_data)
{
	g_input.InputGetCursorPos(m_cursor_x, m_cursor_y);

	m_text = text;
	m_start = vec2(data->m_x + 20.f, data->m_y + 5.f);
	m_box_start = vec2(m_start.x, m_start.y + 12.f);

	data->m_y += 42.f;

	m_bounds = vec2(150, 20);
	m_end = m_box_start + m_bounds;

	m_is_inside = m_cursor_x > m_box_start.x && m_cursor_x < m_end.x&& m_cursor_y > m_box_start.y + 10 && m_cursor_y < m_end.y;

	// Magic number 10 is so to ensure we're hovering over the textbox.
	const bool hovering = m_cursor_x > m_start.x && m_cursor_x < m_end.x&& m_cursor_y > m_start.y + 10 && m_cursor_y < m_end.y;

	// draw label.
	if (m_text)
	{
		const UIcolor_t text_color(206, 206, 206, data->m_alpha);
		const UIcolor_t text_shadow(0, 0, 0, data->m_alpha);

		UI::AddString(Font_t::FONT_MENU, text_shadow, m_start.x + 1.f, m_start.y + 1.f - 6.f, false, m_text);
		UI::AddString(Font_t::FONT_MENU, text_color, m_start.x, m_start.y - 6.f, false, m_text);
	}

	// are we ready to accept input?
	if (hovering && g_input.IsKeyPressed(KEYS_MOUSE1))
		m_ready_to_type = true;

	// we should abort typing.
	if (m_ready_to_type)
	{
		if (g_input.IsKeyPressed(KEYS_ESCAPE) || !hovering && g_input.IsKeyPressed(KEYS_MOUSE1) || g_input.IsKeyPressed(KEYS_RETURN))
			m_ready_to_type = false;
	}

	// draw inner blob.
	{
		const UIcolor_t blob_color = m_ready_to_type ? UIcolor_t(72, 72, 72, data->m_alpha) : hovering ? UIcolor_t(51, 51, 51, data->m_alpha) : UIcolor_t(36, 36, 36, data->m_alpha);

		UI::AddFilledRectangle(UIcolor_t(0, 0, 0, data->m_alpha), m_box_start.x - 1.f, m_box_start.y - 1.f, m_end.x + 1.f, m_end.y + 1.f);
		UI::AddFilledRectangle(blob_color, m_box_start.x, m_box_start.y, m_end.x, m_end.y);
	}

	// clear text.
	if (string_data->length() > 0)
	{
		UI::AddString(Font_t::FONT_MENU, UIcolor_t(153, 153, 153, data->m_alpha), m_start.x + m_bounds.x - 12.f, m_start.y + 15.f, false, "x");

		if (m_ready_to_type && this->IsMouseInRegion(m_start.x + m_bounds.x - 14.f, m_start.y + 15.f, 7, 12) && g_input.IsKeyPressed(KEYS_MOUSE1))
			string_data->clear();
	}

	// idle text box?
	if (!m_ready_to_type && string_data->length() == 0)
		UI::AddString(Font_t::FONT_MENU, UIcolor_t(153, 153, 153, data->m_alpha), m_start.x + 10.f, m_start.y + 15.f, false, "...");

	// for key relative.
	// move key functions in g_input?
	poll_input("Valve001");

	// we're ready to accept input and append to our string data.
	if (m_ready_to_type)
	{
		// backspace.
		if (key_pressed(VK_BACK) && !string_data->empty())
			string_data->pop_back();

		for (int i = 0; i < 222; i++)
		{
			// a-Z
			if (i > 57 && i <= 90)
			{
				if (string_data->length() > 16)
					break;

				// allow capital letters when holding shift.
				if (key_pressed(i))
				{
					*string_data += key_down(VK_SHIFT) ? static_cast<char>(i) : static_cast<char>(i + 32);
				}
			}
		}
	}

	// render the string.
	UI::AddString(Font_t::FONT_MENU, UIcolor_t(153, 153, 153, data->m_alpha), m_start.x + 10.f, m_start.y + 15.f, false, string_data->c_str());

	// blinker.
	if (m_ready_to_type)
	{
		const float text_size = UI::GetTextWidth(Font_t::FONT_MENU, string_data->c_str());

		if ((GetTickCount() / 215) % 2)
		{
			UI::AddFilledRectangle(UIcolor_t(105, 105, 105, data->m_alpha),
								   m_start.x + 10.f + text_size,
								   m_start.y + 17.f, m_start.x + 12.f + text_size,
								   m_start.y + 26.f);

			UI::AddFilledRectangle(UIcolor_t(60, 60, 60, data->m_alpha),
								   m_start.x + 12.f + text_size,
								   m_start.y + 17.f,
								   m_start.x + 14.f + text_size,
								   m_start.y + 26.f);
		}
	}
}