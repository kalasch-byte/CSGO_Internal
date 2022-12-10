#include <windows.h>

#include "../../Header Files/Includes.hpp"

long __stdcall Hooked::WndProc(HWND hwnd, uint32_t msg, uint32_t uparam, long param)
{
	if (msg == WM_MOUSEMOVE)
		g_input.CaptureMouseMove(param);

	g_input.RegisterKeyPress(static_cast<VirtualKeyEvents_t>(msg), static_cast<VirtualKeys_t>(uparam));

	// only apply when menu is open.
	if (g_ctx.bMenuOpened)
	{
		// block mouse input when menu is opened.
		// why? so we can move around with the menu open.
		if (msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_LBUTTONUP || msg == WM_RBUTTONUP ||
			msg == WM_LBUTTONDBLCLK || msg == WM_RBUTTONDBLCLK || msg == WM_MOUSEFIRST || msg == WM_MOUSEWHEEL)
		{
			return false;
		}
	}

	return CallWindowProc(Hooked::Original::oWndProc, hwnd, msg, uparam, param);
}