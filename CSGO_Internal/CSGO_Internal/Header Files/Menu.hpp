#pragma once

#include "Includes.hpp"
#include "../Header Files/utility/Wrappers.hpp"

namespace UI
{
	class CCursor
	{
		int    cursor_x{ }, cursor_y{};
		float  hue_cycle{};

	public:
		void handle()
		{
			g_input.InputGetCursorPos(cursor_x, cursor_y);

			hue_cycle = std::clamp<float>(hue_cycle + 0.002f, 0.f, 1.f);
		}
	};

	class CControl
	{
	public:
		vec2 m_start{};
		vec2 m_bounds{};
		vec2 m_end{};

		int   m_cursor_x{};
		int   m_cursor_y{};
		float m_drag_x{};
		float m_drag_y{};

		float m_alpha = 255;

		bool m_dragging{};
		bool m_should_draw = true;
		bool m_is_inside{};

		std::string m_tooltip{};

		int    m_tab{};
		size_t m_current_tab{};

		inline bool IsMouseInRegion(const float& x, const float& y, const float& w, const float& h)
		{
			return m_cursor_x > x && m_cursor_x < x + w && m_cursor_y > y && m_cursor_y < y + h + 1.0f;
		}
	};

	class CWindowData
	{
	public:
		bool m_first_click{};
		bool m_left_click{};
		bool m_right_click{};
		bool m_ignore{};

		float m_x{};
		float m_y{};

		UIcolor_t m_color = UIcolor_t(251, 199, 85, 255);
		int       m_alpha = 0;
		int       m_test = 0;
		float     m_animation_progress = 0.f;
	};

	class CWindow : public CControl
	{
		bool m_is_open{};
		bool m_was_open{};
		bool m_should_open = true;

		vec2          m_window_size{};
		CWindowData m_data{};
		const float   m_edge_size = 8.f;
		bool          m_is_inside_edge{};

		void DrawBox(float x, float y, float w, float h) const
		{
			// draw black outline.
			UI::AddRectangle(UIcolor_t(1, 1, 1, m_data.m_alpha), x, y, x + w, y + h);

			// draw grey outline.
			UI::AddRectangle(UIcolor_t(44, 44, 44, m_data.m_alpha), x + 1.f, y + 1.f, x + w - 1.f, y + h - 1.f);

			// draw inner box
			UI::AddFilledRectangle(UIcolor_t(9, 9, 9, m_data.m_alpha), x + 2.f, y + 2.f, x + w - 1.f, y + h - 1.f);
		};

	public:
		CWindow(float x = 100.f, float y = 100.f, float w = 482.f, float h = 550.f)
		{
			m_start = vec2(x, y);
			m_window_size = vec2(w, h);
			m_bounds = vec2(w + 40.f, h + 60.f);
			m_end = m_start + m_bounds;
		}

		bool is_open() const
		{
			return m_is_open;
		}

		bool handle(std::vector<std::string>& tabs, UIcolor_t color);

		void ColumnLeft()
		{
			m_data.m_x = m_start.x + 50.f;
			m_data.m_y = m_start.y + 60.f;
		}

		void ColumnRight()
		{
			m_data.m_x = m_start.x + 280.f;
			m_data.m_y = m_start.y + 60.f;
		}

		CWindowData* data()
		{
			return &m_data;
		}
	};

	class CGroupbox : public CControl
	{
		const char* m_text{};

	public:
		void start(CWindowData* data, const char* text)
		{
			m_text = text;
			m_start = vec2(data->m_x - 10.f, data->m_y + 5.f);

			// setup coord for next item
			data->m_y += 15.f;
		}

		void end(CWindowData* data)
		{
			m_end = vec2(data->m_x - 10.f, data->m_y + 5.f);

			// setup coord for next item
			data->m_y += 15.f;

			draw(data);
		}

		void draw(CWindowData* data);
	};

	class CLabel : public CControl
	{
		const char* m_text{};
	public:
		CLabel()
		{

		}

		void draw(CWindowData* data);
		void handle(CWindowData* data, const char* text);
	};

	class CCheckbox : public CControl
	{
		const char* m_text{};
		bool* m_value{};
	public:
		CCheckbox()
		{
			m_bounds = vec2(8, 8);
		}

		void draw(CWindowData* data);
		void handle(CWindowData* data, const char* name, bool* setting, std::string tooltip);
	};

	class CSlider : public CControl
	{
		int* m_value;
		int  m_min;
		int  m_max;

		const char* m_text;
		const wchar_t* m_suffix;

		vec2  m_slider_start;
		float m_slider_pos;

		float m_slider_width;
		float m_slider_height;

	public:
		CSlider()
		{
			m_slider_width = 150.f;
			m_slider_height = 6.f;

			m_bounds = vec2(m_slider_width, m_slider_height);
		}

	public:
		void draw(CWindowData* data);
		void handle(CWindowData* data, const char* name, int* setting, int min, int max, const wchar_t* suffix = L"");
	};

	class CButton : public CControl
	{
		const char* m_text{};
		std::string m_tooltip{};

	public:
		void draw(CWindowData* data);
		void handle(CWindowData* data, const char* text, std::string tooltip, std::function< void() > function);
	};

	class CTextbox : public CControl
	{
		const char* m_text{};
		bool        m_ready_to_type{};
		vec2        m_box_start{};

	public:
		void handle(CWindowData* data, const char* text, std::string* string_data);
	};

	class CKeybind : public CControl
	{
		const char* m_text{};
		int* m_key{};
		vec2        m_box_start;
		bool        m_is_waiting{};

	public:
		void handle(CWindowData* data, const char* text, int* key);
		void draw(CWindowData* data);
	};

	class CDropdown : public CControl
	{
		const char* m_text{};
		std::vector<std::string> m_items{};
		int* m_setting{};
		bool                     m_is_open{};

		vec2        m_box_start;
		vec2        m_box_end;
		bool        m_is_inside_items{};
		float       m_dropdown_end{};
		const float m_item_height = 16.f;
	public:

		void handle(CWindowData* data, const char* text, const std::vector<std::string>& items, int* setting);
		void draw(CWindowData* data);
		void dropdown(CWindowData* data);
	};

	struct DropdownItem_t
	{
		bool* m_setting;
		std::string m_text;
	};

	class CMultiDropdown : public CControl
	{
		const char* m_text{};
		std::vector<DropdownItem_t> m_items{};
		bool                        m_is_open{};
		vec2                        m_box_start;
		vec2                        m_box_end;
		bool                        m_is_inside_items{};
		float                       m_dropdown_end{};
		const float                 m_item_height = 16.f;

	public:
		void handle(CWindowData* data, const char* text, const std::vector<DropdownItem_t>& items);
		void draw(CWindowData* data);
		void dropdown(CWindowData* data);
	};

	class CColorpicker : public CControl
	{
		uint8_t temp_r{};
		uint8_t temp_g{};
		uint8_t temp_b{};

		const char* m_text{};

		bool m_is_open{};

		vec2 m_box_start{};
		vec2 m_box_end{};
		vec2 m_box_bounds{ 20, 8 };

		vec2 m_picker_start{};
		vec2 m_picker_end{};
		vec2 m_picker_bounds{ 256, 256 };

		int      m_alpha_value{};
		CSlider m_alpha_slider{};

	public:
		void draw(CWindowData* data);
		void handle(CWindowData* data, const char* text, uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* a = nullptr);
		void picker(CWindowData* data);
	};
}