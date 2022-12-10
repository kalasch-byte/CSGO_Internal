#include "../../Header Files/Includes.hpp"

long __stdcall Hooked::Reset(IDirect3DDevice9* device, void* params)
{
	g_ctx.renderer->release();

	auto ret = Hooked::Original::oReset(device, params);

	g_ctx.renderer->create();

	return ret;
}