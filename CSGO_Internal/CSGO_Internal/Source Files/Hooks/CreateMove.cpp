#include "../../Header Files/Includes.hpp"

/*class CLBYResolver
{
public:
	void Think(c_base_entity* pEnt);
};

void CLBYResolver::Think(c_base_entity* pEnt)
{
	float	 flFirstDelta  = 0.f;
	float	 flSecondDelta = 0.f;
	float	 flThirdDelta  = 0.f;
	int		 iWay          = 0.f;
	int		 iSide         = 0.f;

	enum AnimLayer_t : uint16_t
	{
		AIMMATRIX              = 0,
		WEAPON_ACTION          = 1,
		WEAPON_ACTION_RECROUCH = 2,
		ADJUST                 = 3,
		MOVEMENT_JUMP_OR_FALL  = 4,
		MOVEMENT_LAND_OR_CLIMB = 5,
		MOVEMENT_MOVE          = 6,
		MOVEMENT_STRAFECHANGE  = 7,
		WHOLE_BODY             = 8,
		FLASHED                = 9,
		FLINCH                 = 0xA,
		ALIVELOOP              = 0xB,
		LEAN                   = 0xC,
		COUNT                  = 0xD
	};

	bool bValidPlayer = pEnt->IsAlive() && !pEnt->IsDormant() && pEnt->GetTeam() != g_ctx.pLocal->GetTeam() && pEnt != g_ctx.pLocal;

	// Invalid target? Reset data.
	if (!bValidPlayer)
	{
		flFirstDelta  = 0.f;
		flSecondDelta = 0.f;
		flThirdDelta  = 0.f;
		iWay          = 0;
		iSide         = 0;
		return;
	}

	const float flVelocity = pEnt->GetVelocity().length();
	const bool  bHasLBY    = pEnt->GetAnimLayer(AnimLayer_t::ADJUST).m_flWeight == 0.0f && pEnt->GetAnimLayer(AnimLayer_t::ADJUST).m_flCycle == 0.0f;

	printf("bHasLBY: %i%s", bHasLBY, "\n");

	if (flVelocity <= 0.1f)
	{
		if (bHasLBY)
		{
			pEnt->GetEyeAngles().y = pEnt->GetEyeAngles().y;

			iWay  = 1;
			iSide = 2 * (Math::AngleDifference(pEnt->GetEyeAngles().y, pEnt->GetAnimState()->goal_feet_yaw) <= 0.0f) - 1;
		}
	}
	else if (!(pEnt->GetAnimLayer(AnimLayer_t::LEAN).m_flWeight * 1000.0f) &&
			 (pEnt->GetAnimLayer(AnimLayer_t::MOVEMENT_MOVE).m_flWeight * 1000.0f) == (pEnt->GetAnimLayer(AnimLayer_t::MOVEMENT_MOVE).m_flWeight * 1000.0f))
	{
		flFirstDelta  = (pEnt->GetAnimLayer(AnimLayer_t::MOVEMENT_MOVE).m_flPlaybackRate - pEnt->GetAnimLayer(AnimLayer_t::MOVEMENT_MOVE).m_flPlaybackRate);
		flSecondDelta = (pEnt->GetAnimLayer(AnimLayer_t::MOVEMENT_MOVE).m_flPlaybackRate - pEnt->GetAnimLayer(AnimLayer_t::MOVEMENT_MOVE).m_flPlaybackRate);
		flThirdDelta  = (pEnt->GetAnimLayer(AnimLayer_t::MOVEMENT_MOVE).m_flPlaybackRate - pEnt->GetAnimLayer(AnimLayer_t::MOVEMENT_MOVE).m_flPlaybackRate);

		if (flFirstDelta < flSecondDelta || flThirdDelta <= flSecondDelta || (flSecondDelta * 1000.0f))
		{
			if (flFirstDelta >= flThirdDelta && flSecondDelta > flThirdDelta && !(flThirdDelta * 1000.0f))
			{
				iWay  = 1;
				iSide = 1;
			}
		}
		else
		{
			iWay  = 1;
			iSide = -1;
		}
	}

	printf("LBY Side: %i%s", iSide, "\n");

	/*switch (uSide)
	{
		case LBYSide_t::LEFT:
			pEnt->GetAnimState()->m_goal_feet_yaw = pEnt->GetAnimState()->m_goal_feet_yaw - 58.f;
			break;

		case LBYSide_t::RIGHT:
			pEnt->GetAnimState()->m_goal_feet_yaw = pEnt->GetAnimState()->m_goal_feet_yaw + 58.f;
			break;

		// Unknown.
		default:
			return;
			break;
	}
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