#include "../../Header Files/Includes.hpp"

bool __fastcall Hooked::ShouldDrawFog(void* ecx, void* edx)
{
	// Return the opposite of the state.
	// i.e; We have the bool enabled, so its disabled.
	return !g_config.m_bNoFog;
}