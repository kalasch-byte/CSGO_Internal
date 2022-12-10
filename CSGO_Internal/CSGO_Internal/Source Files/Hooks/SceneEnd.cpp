#include <algorithm>

#include "../../Header Files/Interfaces.hpp"
#include "../../Header Files/Includes.hpp"
#include "../../Header Files/Player.hpp"

void __fastcall Hooked::SceneEnd(void* ecx, void* edx)
{
	Hooked::Original::oSceneEnd(ecx, edx);
}