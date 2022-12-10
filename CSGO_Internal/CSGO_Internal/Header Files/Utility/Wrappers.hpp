#pragma once

#include "../../Header Files/Includes.hpp"
#include "../../Header Files/Renderer/Renderer.hpp"

namespace UI
{
	struct UIcolor_t
	{
		uint8_t R = 255;
		uint8_t G = 255;
		uint8_t B = 255;
		uint8_t A = 255;

		constexpr UIcolor_t() : R(0), G(0), B(0), A(0) {}
		constexpr UIcolor_t(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : R(r), G(g), B(b), A(a) {}
		constexpr UIcolor_t(uint8_t r, uint8_t g, uint8_t b) : R(r), G(g), B(b), A(255) {}

		__forceinline color_t ToRendererColor() const
		{
			return color_t(R, G, B, A);
		}

		__forceinline static UIcolor_t GetRainbow(const float& flAlpha, const float& flSpeed = 0.0005f)
		{
			static float hue_cycle{};
			UIcolor_t ret;

			hue_cycle = std::fmod(hue_cycle + flSpeed, 1.0f);

			ret = HsbToRgb(hue_cycle, 0.7f, 1.0f);

			return ret;
		}

		__forceinline static UIcolor_t GetRainbow(const float& flAlpha, float& flHueCycle)
		{
			UIcolor_t ret;

			flHueCycle = std::fmod(flHueCycle + 0.003f, 1.0f);

			ret = HsbToRgb(flHueCycle, 0.7f, 1.0f);

			return ret;
		}

		__forceinline static UIcolor_t GetSinbow()
		{
			static float flCycle{};
			UIcolor_t	 ret;

			flCycle = std::fmod(flCycle + 0.003f, 3.0f);

			ret.R = cosf(flCycle) * .5f + .5f;
			ret.G = cosf(flCycle - 2.f * Math::pi / 3.f) * .5f + .5f;
			ret.B = cosf(flCycle - 4.f * Math::pi / 3.f) * .5f + .5f;

			ret.R *= 255.f;
			ret.G *= 255.f;
			ret.B *= 255.f;

			return ret;
		}

		__forceinline static UIcolor_t HsbToRgb(float flHue, float flSaturation, float flBrightness)
		{
			float h = flHue == 1.0f ? 0 : flHue * 6.0f;
			float f = h - (int)h;
			float p = flBrightness * (1.0f - flSaturation);
			float q = flBrightness * (1.0f - flSaturation * f);
			float t = flBrightness * (1.0f - flSaturation * (1.0f - f));

			if (h < 1)
				return UIcolor_t((uint8_t)(flBrightness * 255), (uint8_t)(t * 255), (uint8_t)(p * 255));

			if (h < 2)
				return UIcolor_t((uint8_t)(q * 255), (uint8_t)(flBrightness * 255), (uint8_t)(p * 255));

			if (h < 3)
				return UIcolor_t((uint8_t)(p * 255), (uint8_t)(flBrightness * 255), (uint8_t)(t * 255));

			if (h < 4)
				return UIcolor_t((uint8_t)(p * 255), (uint8_t)(q * 255), (uint8_t)(flBrightness * 255));

			if (h < 5)
				return UIcolor_t((uint8_t)(t * 255), (uint8_t)(p * 255), (uint8_t)(flBrightness * 255));

			return UIcolor_t((uint8_t)(flBrightness * 255), (uint8_t)(p * 255), (uint8_t)(q * 255));
		}
	};

	__forceinline void AddFilledRectangle(UIcolor_t color, float flX, float flY, float flWidth, float flHeight)
	{
		color_t rendererColor = color.ToRendererColor();

		g_ctx.renderer->filled_rect(rendererColor, flX, flY, flWidth, flHeight);
	}

	__forceinline void AddRectangle(UIcolor_t color, float flX, float flY, float flWidth, float flHeight)
	{
		color_t rendererColor = color.ToRendererColor();

		g_ctx.renderer->rect(rendererColor, flX, flY, flWidth, flHeight);
	}

	__forceinline void _AddFilledRectangle(UIcolor_t color, float flX, float flY, float flWidth, float flHeight)
	{
		color_t rendererColor = color.ToRendererColor();

		g_ctx.renderer->filled_rect(rendererColor, flX, flY, flX + flWidth, flY + flHeight);
	}

	__forceinline void AddRectangleGradient(UIcolor_t top, UIcolor_t bottom, float flX, float flY, float flWidth, float flHeight)
	{
		color_t rendererTop = top.ToRendererColor();
		color_t rendererBottom = bottom.ToRendererColor();

		g_ctx.renderer->rect_gradient(Gradient_t::VERTICAL, rendererTop, rendererBottom, flX, flY, flWidth, flHeight);
	}

	__forceinline void AddLine(UIcolor_t color, float flX0, float flY0, float flX1, float flY1)
	{
		color_t rendererColor = color.ToRendererColor();

		g_ctx.renderer->line(rendererColor, flX0, flY0, flX1, flY1);
	}

	static void AddString(Font_t font, UIcolor_t color, float flX, float flY, bool bCenter, const char* pText, ...)
	{
		char cBuf[256];
		va_list args;
		va_start(args, pText);
		vsprintf(cBuf, pText, args);
		va_end(args);

		color_t rendererColor = color.ToRendererColor();

		g_ctx.renderer->string(font, rendererColor, flX, flY, bCenter, cBuf);
	}

	static void AddString(Font_t font, UIcolor_t color, float flX, float flY, bool bCenter, const wchar_t* pText, ...)
	{
		wchar_t cBuf[256];
		va_list args;
		va_start(args, pText);
		vswprintf(cBuf, 256, pText, args);
		va_end(args);

		color_t rendererColor = color.ToRendererColor();

		g_ctx.renderer->wide_string(font, rendererColor, flX, flY, bCenter, cBuf);
	}

	static float GetTextWidth(Font_t font, const char* pText, ...)
	{
		char cBuf[256];
		va_list args;
		va_start(args, pText);
		vsprintf(cBuf, pText, args);
		va_end(args);

		return g_ctx.renderer->text_width(font, cBuf);
	}
}