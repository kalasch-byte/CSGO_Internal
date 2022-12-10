#include "../../Header Files/Includes.hpp"

void __fastcall Hooked::PaintTraverse(void* ecx, void* edx, uint32_t panel, bool repaint, bool allow_force)
{
	const auto panel_name = Hash::Fnv1a(g_csgo.m_panel->get_name(panel));

	// screen size changed.
	if (g_ctx.bShouldUpdateScreenSize)
	{
		// get new screen size.
		g_csgo.m_engine->GetScreenSize(g_ctx.iScreenWidth, g_ctx.iScreenHeight);

		// important: don't update the screen size any more then its need to be.
		g_ctx.bShouldUpdateScreenSize = false;
	}

	switch (panel_name)
	{
		// for surface drawing.
		case Hash::Fnv1a("MatSystemTopPanel"):
			break;

			// for panel class related stuff.
		case Hash::Fnv1a("FocusOverlayPanel"):
			g_csgo.m_panel->enable_mouse_input(panel, g_ctx.bMenuOpened);
			break;

			// when we're scoped.
		case Hash::Fnv1a("HudZoom"):
			break;
	}

	Hooked::Original::oPaintTraverse(ecx, edx, panel, repaint, allow_force);
}