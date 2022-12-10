#include "../../Header Files/Includes.hpp"

/*class CLBYResolver
{
public:
	void Think(c_base_entity* pEnt);
};
}*/

void MousedFix(c_user_cmd* pCmd)
{
	if (!g_ctx.pLocal)
		return;

	static vec3 deltaViewAngles{};
	vec3 delta = pCmd->m_view_angles - deltaViewAngles;
	delta.clamp();

	static cvar_t* pSensitivity = g_csgo.m_engine_cvars->FindVar("sensitivity");

	if (!pSensitivity)
		return;

	if (delta.x != 0.f)
	{
		static cvar_t* pPitch = g_csgo.m_engine_cvars->FindVar("m_pitch");

		if (!pPitch)
			return;

		int iFinalDy = static_cast<int>((delta.x / pPitch->get_float()) / pSensitivity->get_float());

		if (iFinalDy <= 32767)
		{
			if (iFinalDy >= -32768)
			{
				if (iFinalDy >= 1 || iFinalDy < 0)
				{
					if (iFinalDy <= -1 || iFinalDy > 0)
						iFinalDy = iFinalDy;
					else
						iFinalDy = -1;
				}
				else
				{
					iFinalDy = 1;
				}
			}
			else
			{
				iFinalDy = 32768;
			}
		}
		else
		{
			iFinalDy = 32767;
		}

		pCmd->m_mousedy = static_cast<short>(iFinalDy);
	}

	if (delta.y != 0.f)
	{
		static cvar_t* pYaw = g_csgo.m_engine_cvars->FindVar("m_yaw");

		if (!pYaw)
			return;

		int iFinalDx = static_cast<int>((delta.y / pYaw->get_float()) / pSensitivity->get_float());

		if (iFinalDx <= 32767)
		{
			if (iFinalDx >= -32768)
			{
				if (iFinalDx >= 1 || iFinalDx < 0)
				{
					if (iFinalDx <= -1 || iFinalDx > 0)
						iFinalDx = iFinalDx;
					else
						iFinalDx = -1;
				}
				else
				{
					iFinalDx = 1;
				}
			}
			else
			{
				iFinalDx = 32768;
			}
		}
		else
		{
			iFinalDx = 32767;
		}

		pCmd->m_mousedx = static_cast<short>(iFinalDx);
	}

	deltaViewAngles = pCmd->m_view_angles;
}

bool __stdcall Hooked::CreateMove(float sample_frametime, c_user_cmd* cmd)
{
	if (!cmd || !cmd->m_command_number)
		return false;

	if (g_csgo.m_engine->IsConnected())
	{
		// Set local player pointer.
		if (!g_ctx.pLocal)
			g_ctx.pLocal = reinterpret_cast<c_base_entity*>(g_csgo.m_entity_list->GetClientEntity(g_csgo.m_engine->GetLocalPlayer()));

		// Set CUserCmd pointer.
		if (!g_ctx.pCmd)
			g_ctx.pCmd = cmd;

		MousedFix(cmd);

		// Basic auto bunnyhop.
		if (g_config.m_bAutoBunnyhop && g_ctx.pLocal != nullptr && g_ctx.pLocal->IsAlive())
		{
			if (!(g_ctx.pLocal->GetFlags() & FL_ONGROUND))
				cmd->m_buttons &= ~IN_JUMP;
		}

		Math::CorrectMovement(cmd->m_view_angles, cmd, cmd->m_forward_move, cmd->m_side_move);

		cmd->m_forward_move = std::clamp(cmd->m_forward_move, -450.f, 450.f);
		cmd->m_side_move = std::clamp(cmd->m_side_move, -450.f, 450.f);
		cmd->m_up_move = std::clamp(cmd->m_up_move, -320.f, 320.f);

		// Clamp view angles when we are using anti-untrust.
		if (g_config.m_iRestrictionType == RestrictionType_t::ANTI_UNTRUSTED)
		{
			cmd->m_view_angles.normalize();
			cmd->m_view_angles.x = std::clamp(cmd->m_view_angles.x, -89.f, 89.f);
			cmd->m_view_angles.y = std::clamp(cmd->m_view_angles.y, -180.f, 180.f);
			cmd->m_view_angles.z = std::clamp(cmd->m_view_angles.z, 0.f, 0.f);
		}
	}

	return Original::oCreateMove(sample_frametime, cmd);
}
