#include "../../Header Files/Includes.hpp"

HRESULT __stdcall Hooked::Present(IDirect3DDevice9* device, RECT* src, const RECT* dst, HWND dst_window_override, const RGNDATA* dirty_region)
{
	// initialize renderer.
	if (!g_ctx.bInitialized)
	{
		g_ctx.renderer = std::make_unique<CRender>(device);
		g_ctx.bInitialized = true;
	}

	// initialize view-matrix.
	{
		static uintptr_t view_matrix = 0;

		if (!view_matrix)
			view_matrix = *reinterpret_cast<std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(Memory::FindSignature("client.dll", "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9")) + 3) + 176;

		g_ctx.matrix = *reinterpret_cast<VMatrix*>(view_matrix);
	}

	static bool once = false;

	// acquire screen size ONCE.
	if (!once)
	{
		g_csgo.m_engine->GetScreenSize(g_ctx.iScreenWidth, g_ctx.iScreenHeight);

		once = true;
	}

	g_ctx.renderer->start();
	{
		g_ctx.gui.Render();

		const short fps = 1.f / (g_csgo.m_globals->m_frametime);

		g_ctx.renderer.get()->string(Font_t::FONT_MENU, color_t(0, 0, 0, 255), 11, 11, false, std::to_string(fps).c_str());
		g_ctx.renderer.get()->string(Font_t::FONT_MENU, color_t(255, 255, 255, 255), 10, 10, false, std::to_string(fps).c_str());
	}
	g_ctx.renderer->finish();

	return Hooked::Original::oPresent(device, src, dst, dst_window_override, dirty_region);
}