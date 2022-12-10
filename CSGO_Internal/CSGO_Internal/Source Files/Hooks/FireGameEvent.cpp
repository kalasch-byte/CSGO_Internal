#include "../../Header Files/Includes.hpp"

bool __fastcall Hooked::FireGameEvent(void* ecx, void* edx, IGameEvent* game_event)
{
	// Events: https://wiki.alliedmods.net/Counter-Strike:_Global_Offensive_Events#round_start

	if (g_ctx.pLocal && g_csgo.m_engine->IsInGame())
	{
		if (game_event != nullptr)
		{
			/*if (!strcmp(game_event->get_name(), "player_death"))
				MessageBoxA(nullptr, "player_death", "Player died", MB_OK | MB_ICONASTERISK);*/
		}
	}

	return Original::oFireGameEvent(ecx, edx, game_event);
}