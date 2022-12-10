#include "../../Header Files/Includes.hpp"

void __fastcall Hooked::OnScreenSizeChanged(void* ecx, void* edx, int old_w, int old_h)
{
	// Handled in PaintTraverse.
	g_ctx.bShouldUpdateScreenSize = true;

	Hooked::Original::oOnScreenSizeChanged(ecx, edx, old_w, old_h);

	// May not work / crash.
	// g_csgo.m_engine->GetScreenSize( g_ctx.iScreenWidth, g_ctx.iScreenHeight );
}