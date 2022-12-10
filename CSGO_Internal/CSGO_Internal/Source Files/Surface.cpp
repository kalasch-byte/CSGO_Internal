#include "../Header Files/Includes.hpp"

void SurfaceRender::DrawRectangle(short x, short y, short width, short height, clr_t color)
{
	if (width <= 0 || height <= 0)
		return;

	g_csgo.m_surface->DrawSetColor(color);
	g_csgo.m_surface->DrawOutlinedRect(x, y, width, height);
}

void SurfaceRender::DrawFilledRectangle(short x, short y, short width, short height, clr_t color)
{
	if (width <= 0 || height <= 0)
		return;

	g_csgo.m_surface->DrawSetColor(color);
	g_csgo.m_surface->DrawFilledRect(x, y, width, height);
}