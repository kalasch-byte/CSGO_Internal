#pragma once

#include "Netvars.hpp"
#include "ValveSDK.hpp"
#include "Interfaces.hpp"

#include <codecvt>

enum hitboxes_t
{
	HITBOX_HEAD,
	HITBOX_NECK,
	HITBOX_PELVIS,
	HITBOX_BODY,
	HITBOX_THORAX,
	HITBOX_CHEST,
	HITBOX_UPPER_CHEST,
	HITBOX_RIGHT_THIGH,
	HITBOX_LEFT_THIGH,
	HITBOX_RIGHT_CALF,
	HITBOX_LEFT_CALF,
	HITBOX_RIGHT_FOOT,
	HITBOX_LEFT_FOOT,
	HITBOX_RIGHT_HAND,
	HITBOX_LEFT_HAND,
	HITBOX_RIGHT_UPPER_ARM,
	HITBOX_RIGHT_FOREARM,
	HITBOX_LEFT_UPPER_ARM,
	HITBOX_LEFT_FOREARM,
	HITBOX_MAX,
	HITBOX_CLOSEST = 30,
};

enum SolidType_t
{
	SOLID_NONE = 0,	// no solid model
	SOLID_BSP = 1,	// a BSP tree
	SOLID_BBOX = 2,	// an AABB
	SOLID_OBB = 3,	// an OBB (not implemented yet)
	SOLID_OBB_YAW = 4,	// an OBB, constrained so that it can only yaw
	SOLID_CUSTOM = 5,	// Always call into the entity for tests
	SOLID_VPHYSICS = 6,	// solid vphysics object, get vcollide from the model and collide with that
	SOLID_LAST,
};

enum SolidFlags_t
{
	FSOLID_CUSTOMRAYTEST = 0x0001,	// Ignore solid type + always call into the entity for ray tests
	FSOLID_CUSTOMBOXTEST = 0x0002,	// Ignore solid type + always call into the entity for swept box tests
	FSOLID_NOT_SOLID = 0x0004,	// Are we currently not solid?
	FSOLID_TRIGGER = 0x0008,	// This is something may be collideable but fires touch functions
									// even when it's not collideable (when the FSOLID_NOT_SOLID flag is set)
									FSOLID_NOT_STANDABLE = 0x0010,	// You can't stand on this
									FSOLID_VOLUME_CONTENTS = 0x0020,	// Contains volumetric contents (like water)
									FSOLID_FORCE_WORLD_ALIGNED = 0x0040,	// Forces the collision rep to be world-aligned even if it's SOLID_BSP or SOLID_VPHYSICS
									FSOLID_USE_TRIGGER_BOUNDS = 0x0080,	// Uses a special trigger bounds separate from the normal OBB
									FSOLID_ROOT_PARENT_ALIGNED = 0x0100,	// Collisions are defined in root parent's local coordinate space
									FSOLID_TRIGGER_TOUCH_DEBRIS = 0x0200,	// This trigger will touch debris objects

									FSOLID_MAX_BITS = 10
};

class c_base_weapon;
class c_base_entity
{
	template < typename t >
	__forceinline t& get(std::ptrdiff_t offset)
	{
		return *reinterpret_cast<t*>(uintptr_t(this) + offset);
	}

public:

	NETVAR_OFFSET(CoordinateFrame, matrix3x4_t, "DT_BaseEntity", "m_CollisionGroup", -0x30);
	NETVAR_OFFSET(VAngle, vec3, "DT_BasePlayer", "deadflag", 4);
	NETVAR_OFFSET(WritableBones, int, "DT_BaseAnimating", "m_nForceBone", 0x20);
	NETVAR_OFFSET(FlashTime, float, "DT_CSPlayer", "m_flFlashMaxAlpha", -0x10);
	NETVAR_OFFSET(OldSimTime, float, "DT_BaseEntity", "m_flSimulationTime", 0x4);
	NETVAR_OFFSET(GetMoveType, uint8_t, "DT_BaseEntity", "m_nRenderMode", 1);
	NETVAR_OFFSET(RgflCoordinateFrame, matrix3x4_t, "DT_BaseEntity", "m_CollisionGroup", -48);

	NETVAR(HitboxSet, int, "DT_BaseAnimating", "m_nHitboxSet");
	NETVAR(ForceBone, int, "DT_BaseAnimating", "m_nForceBone");
	NETVAR(VecForce, vec3, "DT_BaseAnimating", "m_vecForce");
	NETVAR(ClientSideAnim, bool, "DT_BaseAnimating", "m_bClientSideAnimation");
	NETVAR(RagdollVelocity, vec3, "DT_CSRagdoll", "m_vecRagdollVelocity");
	NETVAR(Origin, vec3, "DT_BaseEntity", "m_vecOrigin"); // Position.
	NETVAR(GetSimTime, float, "DT_BaseEntity", "m_flSimulationTime");
	NETVAR(ModelIndex, int, "DT_BaseEntity", "m_nModelIndex");
	NETVAR(VecMins, vec3, "DT_BaseEntity", "m_vecMins");
	NETVAR(VecMaxs, vec3, "DT_BaseEntity", "m_vecMaxs");
	NETVAR(GetAnimTime, float, "DT_BaseEntity", "m_flAnimTime");
	NETVAR(Owner, ulong, "DT_BaseEntity", "m_hOwnerEntity");
	NETVAR(GetTeam, int, "DT_BaseEntity", "m_iTeamNum");
	NETVAR(GetCollisionGroup, int, "DT_BaseEntity", "m_CollisionGroup");
	NETVAR(SolidFlags, int, "DT_BaseEntity", "m_usSolidFlags");
	NETVAR(Solid, int, "DT_BaseEntity", "m_nSolidType");
	NETVAR(IsSpottedOnRadar, bool, "DT_BaseEntity", "m_bSpotted");

	NETVAR(GetHealth, int, "DT_BasePlayer", "m_iHealth");
	NETVAR(GetFlags, int, "DT_BasePlayer", "m_fFlags");
	NETVAR(GetLifeState, uint8_t, "DT_BasePlayer", "m_lifeState");
	NETVAR(GetSpectatorMode, int, "DT_BasePlayer", "m_iObserverMode");
	NETVAR(GetSpectatorTarget, uint32_t, "DT_BasePlayer", "m_hObserverTarget");
	NETVAR(AimPunchAngle, vec3, "DT_BasePlayer", "m_aimPunchAngle");
	NETVAR(PunchVelocity, vec3, "DT_BasePlayer", "m_aimPunchAngleVel");
	NETVAR(ViewPunchAngle, vec3, "DT_BasePlayer", "m_viewPunchAngle");
	NETVAR(GetFallVelocity, float, "DT_BasePlayer", "m_flFallVelocity");
	NETVAR(GetTickbase, int, "DT_BasePlayer", "m_nTickBase");
	NETVAR(GetViewOffset, vec3, "DT_BasePlayer", "m_vecViewOffset[0]");
	NETVAR(GetDuckAmount, float, "DT_BasePlayer", "m_flDuckAmount");
	NETVAR(GetDetonationTime, float, "DT_PlantedC4", "m_flC4Blow");
	NETVAR(GetBombTimerLength, float, "DT_PlantedC4", "m_flTimerLength");
	NETVAR(GetArmorValue, int, "DT_CSPlayer", "m_ArmorValue");
	NETVAR(HasDefuser, bool, "DT_CSPlayer", "m_bHasDefuser");
	NETVAR(IsDefusing, bool, "DT_CSPlayer", "m_bIsDefusing");
	NETVAR(HasHelmet, bool, "DT_CSPlayer", "m_bHasHelmet");
	NETVAR(HasSpawnProtection, bool, "DT_CSPlayer", "m_bGunGameImmunity");
	NETVAR(GetEyeAngles, vec3, "DT_CSPlayer", "m_angEyeAngles[0]");
	NETVAR(GetVelocity, vec3, "DT_CSPlayer", "m_vecVelocity[0]");
	NETVAR(GetBaseVelocity, vec3, "DT_CSPlayer", "m_vecBaseVelocity");
	NETVAR(GetLBY, float, "DT_CSPlayer", "m_flLowerBodyYawTarget");
	NETVAR(IsScoped, bool, "DT_CSPlayer", "m_bIsScoped");
	NETVAR(GetMoney, int, "DT_CSPlayer", "m_iAccount");
	NETVAR(GetFlashAlpha, float, "DT_CSPlayer", "m_flFlashMaxAlpha");
	NETVAR(GetFlashTime, float, "DT_CSPlayer", "m_flFlashDuration");
	NETVAR(GetShotAmount, int, "DT_CSPlayer", "m_iShotsFired");
	NETVAR(GetWeapons, uint32_t*, "DT_BaseCombatCharacter", "m_hMyWeapons");
	NETVAR(ActiveWeapon, uint32_t, "DT_BaseCombatCharacter", "m_hActiveWeapon");
	NETVAR(NextAttack, float, "DT_BaseCombatCharacter", "m_flNextAttack");
	NETVAR(GetThrowTime, float, "DT_BaseCSGrenade", "m_fThrowTime");

	PNETVAR(GetPoseParameter, float, "DT_CSPlayer", "m_flPoseParameter");

	inline void* Renderable()
	{
		return reinterpret_cast<void*>(this + 0x4);
	}

	inline void* Networkable()
	{
		return reinterpret_cast<void*>(this + 0x8);
	}

	ICollideable* Collideable()
	{
		using fn = ICollideable * (__thiscall*)(void*);
		return Memory::GetVFunc<fn>(this, 3)(this);
	}

	inline int EntIndex()
	{
		using index_t = int(__thiscall*)(void*);
		return (*(index_t**)Networkable())[10](Networkable());

		// return *reinterpret_cast< int* >( this + 0x64 );
	}

	inline bool IsWeapon()
	{
		const bool IsBaseCombatWeapon = Memory::GetVFunc< bool(__thiscall*)(void*) >(this, 165)(this);
		return IsBaseCombatWeapon || Client()->m_class_id == CC4; /*|| client( )->m_class_id == CEconEntity;*/ // defuse kit, but idk how to get FileWeaponInfo for it
	}

	inline bool IsPlayer()
	{
		// lazy
		//return client( )->m_class_id == CCSPlayer;
		return Memory::GetVFunc< bool(__thiscall*)(void*) >(this, 158)(this);
	}

	inline c_client* Client()
	{
		return Memory::GetVFunc< c_client* (__thiscall*)(void*) >(this->Networkable(), 2)(this->Networkable());
	}

	const char* GetMapAreaName()
	{
		static const size_t offset = g_netvars.get_netvar(fnv("DT_BasePlayer"), fnv("m_szLastPlaceName"));
		return reinterpret_cast<char*>(this + offset);
	}

	inline vec3& GetAbsOrigin()
	{
		using fn = vec3 & (__thiscall*)(void*);

		return Memory::GetVFunc<fn>(this, 10)(this);
	}

	inline vec3 GetOldOrigin()
	{
		// FF 90 ? ? ? ? F3 0F 10 05 ? ? ? ? F3 0F 5E 44 24
		// if it ever changes, C_BasePlayer::PostDataUpdate, has snd_soundmixer and a million other strings
		/*
			float flTimeDelta = m_flSimulationTime - m_flOldSimulationTime;
			if ( flTimeDelta > 0  &&  !( IsEffectActive(EF_NOINTERP) || bForceEFNoInterp ) )
			{
					Vector newVelo = (GetNetworkOrigin() - GetOldOrigin()  ) / flTimeDelta;
					SetAbsVelocity( newVelo);
			}
		*/

		return *reinterpret_cast<vec3*>(this + 936);
	}

	vec3 GetEyeOrigin()
	{
		return Origin() + GetViewOffset();
	}

	vec3& GetAbsAngles()
	{
		return Memory::GetVFunc<vec3& (__thiscall*)(void*)>(this, 11)(this);
	}

	inline vec3 WorldSpaceCenter()
	{
		return vec3((this->VecMins() + this->VecMaxs()) * 0.5f);
	}

	// fyi, server lagcomp uses viewoffset, and modifyeyeposition isnt called
	inline vec3 firebullets_position()
	{
		vec3 out;

		Memory::GetVFunc< void(__thiscall*)(void*, vec3&) >(this, 277)(this, out);

		return out;
	}

	inline vec3 head_position(bool bInterp = false)
	{
		if (!bInterp)
			return (this->GetViewOffset() + this->Origin());
		else
			return (this->GetViewOffset() + this->GetAbsOrigin());
	}

	c_base_weapon* Weapon();

	inline bool IsAlive()
	{
		return this->GetHealth() > 0;
	}

	inline bool IsDormant()
	{
		// static const auto player_dormant = Memory::FindSignature( "client.dll", "8A 81 ? ? ? ? C3 32 C0" );
		using dormant_t = bool(__thiscall*)(void*);
		return (*static_cast<dormant_t**>(Networkable()))[9](Networkable());

		// return *reinterpret_cast< bool* >( reinterpret_cast<uintptr_t>(this) + 0xED );
	}

	std::wstring GetName()
	{
		player_info_t player_info;

		if (!Info(&player_info))
			return L"";

		//clean it up
		char buffer[32];
		for (int i = 0; i < 32; i++)
		{
			if (player_info.name[i] < 0)
			{
				buffer[i] = ' ';
				continue;
			}

			switch (player_info.name[i])
			{
				case '"':
				case '\\':
				case ';':
				case '\n':
					buffer[i] = ' ';
					break;
				default:
					buffer[i] = player_info.name[i];
					break;
			}
		}

		buffer[31] = '\0';

		std::wstring_convert< std::codecvt_utf8< wchar_t >, wchar_t > convert;

		return convert.from_bytes(std::string(buffer));
	}

	vec3& RenderAngles()
	{
		return *reinterpret_cast<vec3*>(reinterpret_cast<uintptr_t>(this) + 0x128);
	}

	CAnimationLayer& GetAnimLayer(uint16_t uIndex)
	{
		// xref: m_AnimOverlay
		return (*(CAnimationLayer**)((uintptr_t)this + 0x2990))[uIndex];
	}

	CCSGOPlayerAnimState* GetAnimState()
	{
		// xref: hand_L
		return *reinterpret_cast<CCSGOPlayerAnimState**>(reinterpret_cast<uintptr_t>(this) + 0x9960);
	}

	__forceinline int GetSequenceAct(uint16_t uSequence)
	{
		auto hdr = g_csgo.m_model_info->GetStudioModel(this->Model());

		// return dumb value to inform us it failed.
		if (!hdr)
			return -1337;

		static const auto GetSequenceActivity = reinterpret_cast<int(__fastcall*)(void*, studiohdr_t*, int)>(Memory::FindSignature("client.dll", "55 8B EC 53 8B 5D 08 56 8B F1 83"));

		return GetSequenceActivity(this, hdr, uSequence);
	}

	void invalidate_bone_cache();

	matrix3x4_t* BoneCache()
	{
		return *reinterpret_cast<matrix3x4_t**>(reinterpret_cast<uintptr_t>(this) + 0x28FC); // m_CachedBoneData.Base()
	}

	size_t BoneCacheCount()
	{
		return *reinterpret_cast<size_t*>(reinterpret_cast<uintptr_t>(this) + 0x2908); // m_CachedBoneData.Count()
	}

	bool IsStandable()
	{
		if (SolidFlags() & FSOLID_NOT_STANDABLE)
			return false;

		if (Solid() == SOLID_BSP || Solid() == SOLID_VPHYSICS || Solid() == SOLID_BBOX)
			return true;

		return false;
	}

	inline model_t* Model()
	{
		return Memory::GetVFunc< model_t* (__thiscall*)(void*) >(this->Renderable(), 8)(this->Renderable());
	}

	inline int DrawModel(int flags, uint8_t alpha = 255)
	{
		return Memory::GetVFunc< int(__thiscall*)(void*, int, uint8_t) >(this->Renderable(), 9)(this->Renderable(), flags, alpha);
	}

	inline bool SetupBones(matrix3x4_t* bone_matrix, int max_bones, int bone_mask, float curtime)
	{
		return Memory::GetVFunc< bool(__thiscall*)(void*, matrix3x4_t*, int, int, float) >(this->Renderable(), 13)(this->Renderable(), bone_matrix, max_bones, bone_mask, curtime);
	}

	inline bool HasBomb()
	{
		// xref: weapon_c4
		static const auto PlayerHasBomb = reinterpret_cast<bool(__thiscall*)(void*)>(Memory::FindSignature("client.dll", "56 8B F1 85 F6 74 31"));

		return PlayerHasBomb(this);
	}

	void UpdateClientSidedAnimations()
	{
		// xref: UpdateClientSideAnimations
		Memory::GetVFunc< void(__thiscall*)(void*) >(this, 224)(this);
	}

	void Think()
	{
		return Memory::GetVFunc< void(__thiscall*)(void*) >(this, 137)(this);
	}

	void RunThink();

	void RunPostThink()
	{
		return Memory::GetVFunc< void(__thiscall*)(void*) >(this, 308)(this);
	}

	vec3 GetBonePos(int bone, matrix3x4_t* matrix)
	{
		return vec3(matrix[bone][0][3], matrix[bone][1][3], matrix[bone][2][3]);
	}

	mstudiobbox_t* GetHitbox(int Hitbox);

	bool GetHitboxPos(int Hitbox, vec3& Out);

	bool Info(player_info_t*);
	bool IsVisible(c_base_entity* attacker, const vec3& src, const vec3& dst);

	inline void SetAbsOrigin(vec3 origin)
	{
		static const auto SetAbsOrigin = reinterpret_cast<void(__thiscall*)(void*, vec3&)>(Memory::FindSignature("client.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8"));
		SetAbsOrigin(this, origin);
	}

	inline void SetAbsAngles(vec3 angles)
	{
		static const auto SetAbsAngles = reinterpret_cast<void(__thiscall*)(void*, vec3&)>(Memory::FindSignature("client.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8"));
		SetAbsAngles(this, angles);
	}

	__forceinline void fuck_ik()
	{
		auto& m_EntClientFlags = *reinterpret_cast<uint16_t*>(uintptr_t(this) + 0x68);

		//#define ENTCLIENTFLAG_DONTUSEIK 0x2
		m_EntClientFlags |= 2;
	}

	__forceinline float GetReloadProgress()
	{
		const auto& gun_layer = GetAnimLayer(1);
		float		 playback_rate = gun_layer.m_flPlaybackRate; // this layer both has shots and reloads, so check playback rate, this seems to be the lowest for awp, 0.6, while reloads never go over 0.5

		if (playback_rate < 0.55f)
			return gun_layer.m_flCycle;

		return 1.f;
	}

	__forceinline bool IsReloading()
	{
		return GetReloadProgress() < 0.99f;
	}

	bool IsFlashed();

	void SetInterpolation(bool interp)
	{
		VarMapping_t* varmapping = reinterpret_cast<VarMapping_t*>(uintptr_t(this) + 0x24);

		for (int j = 0; j < varmapping->m_nInterpolatedEntries; j++)
		{
			if (!varmapping->m_Entries.GetElements())
				continue;

			auto e = &varmapping->m_Entries.GetElements()[j];

			if (e)
				e->m_bNeedsToInterpolate = interp;
		}

		// da real interp killa
		varmapping->m_nInterpolatedEntries = interp ? 6 : 0;
	}
};

enum Weapon_t
{
	RIFLE,
	PISTOL,
	SNIPER,
	SMG,
	HEAVY,
	KNIFE,
	GRENADE,
};

enum ItemDefinitionIndex_t
{
	WEAPON_NONE = 0,
	WEAPON_DEAGLE = 1,
	WEAPON_ELITE = 2,
	WEAPON_FIVESEVEN = 3,
	WEAPON_GLOCK = 4,
	WEAPON_AK47 = 7,
	WEAPON_AUG = 8,
	WEAPON_AWP = 9,
	WEAPON_FAMAS = 10,
	WEAPON_G3SG1 = 11,
	WEAPON_GALILAR = 13,
	WEAPON_M249 = 14,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10 = 17,
	WEAPON_P90 = 19,
	WEAPON_UMP45 = 24,
	WEAPON_XM1014 = 25,
	WEAPON_BIZON = 26,
	WEAPON_MAG7 = 27,
	WEAPON_NEGEV = 28,
	WEAPON_SAWEDOFF = 29,
	WEAPON_TEC9 = 30,
	WEAPON_TASER = 31,
	WEAPON_HKP2000 = 32,
	WEAPON_MP7 = 33,
	WEAPON_MP9 = 34,
	WEAPON_NOVA = 35,
	WEAPON_P250 = 36,
	WEAPON_SCAR20 = 38,
	WEAPON_SG556 = 39,
	WEAPON_SSG08 = 40,
	WEAPON_KNIFE = 42,
	WEAPON_FLASHBANG = 43,
	WEAPON_HEGRENADE = 44,
	WEAPON_SMOKEGRENADE = 45,
	WEAPON_MOLOTOV = 46,
	WEAPON_DECOY = 47,
	WEAPON_INCGRENADE = 48,
	WEAPON_C4 = 49,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER = 60,
	WEAPON_USP_SILENCER = 61,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER = 64,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT = 506,
	WEAPON_KNIFE_KARAMBIT = 507,
	WEAPON_KNIFE_M9_BAYONET = 508,
	WEAPON_KNIFE_TACTICAL = 509,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY = 515,
	WEAPON_KNIFE_PUSH = 516
};

class c_base_weapon : public c_base_entity
{
	template < typename t >
	__forceinline t& get(std::ptrdiff_t offset)
	{
		return *reinterpret_cast<t*>(uintptr_t(this) + offset);
	}
public:
	NETVAR(ItemIndex, int, "DT_BaseAttributableItem", "m_iItemDefinitionIndex");
	NETVAR(GetAmmo1, int, "DT_BaseCombatWeapon", "m_iClip1");
	NETVAR(GetAmmo2, int, "DT_BaseCombatWeapon", "m_iPrimaryReserveAmmoCount");
	NETVAR(GetNextPrimaryAttack, float, "DT_BaseCombatWeapon", "m_flNextPrimaryAttack");
	NETVAR(GetRecoilIndex, float, "DT_WeaponCSBase", "m_flRecoilIndex");
	NETVAR(GetShotTime, float, "DT_WeaponCSBase", "m_fLastShotTime");
	NETVAR(IsPinPulled, bool, "DT_BaseCSGrenade", "m_bPinPulled");
	NETVAR(GetThrowTime, float, "DT_BaseCSGrenade", "m_fThrowTime");

	inline bool IsFiring()
	{
		// Note - to do; account for knives, grenade throw, any type of attack
		// or hook FireBullets?
		bool bAttacking = TIME_TO_TICKS(this->GetShotTime() != this->GetSimTime());

		// bool fire2 = this->shots_fired( ) > 0;
		// bool fire3 = this->pin_pulled( ) && this->throw_time( ) > 0.f;

		return bAttacking;
	}

	// Thx senator
	inline weapon_info_t* GetWeaponData()
	{
		return Memory::GetVFunc< weapon_info_t* (__thiscall*)(void*) >(this, 445)(this);
	}

	// k i don't fucking get this
	// GetPrintName is supposed to be
	// 1 after GetName, however, it's not ):
	inline const char* GetName()
	{
		return Memory::GetVFunc< const char* (__thiscall*)(void*) >(this, 376)(this);
	}

	std::string PrintName();

	/*inline const char* print_name( )
	{
		auto weapon_data = wpn_data( );

		if ( !weapon_data )
			return " ";

		//return weapon_data->hud_name
		return weapon_data->weapon_name2 + 7;
	}*/

	inline int GetMaxClip1()
	{
		auto weapon_info = GetWeaponData();

		if (!weapon_info)
			return 30;

		return weapon_info->max_clip_ammo;
	}

	inline float GetSpread()
	{
		return Memory::GetVFunc< float(__thiscall*)(void*) >(this, 453)(this);
	}

	// Valid?
	inline void UpdateAccuracyPenalty()
	{
		return Memory::GetVFunc< void(__thiscall*)(void*) >(this, 483)(this);
	}

	inline float GetInaccuracy()
	{
		return Memory::GetVFunc< float(__thiscall*)(void*) >(this, 483)(this);
	}

	bool CanFire();

	__forceinline Weapon_t type()
	{
		switch (this->ItemIndex())
		{
			case WEAPON_DEAGLE:
			case WEAPON_ELITE:
			case WEAPON_FIVESEVEN:
			case WEAPON_HKP2000:
			case WEAPON_USP_SILENCER:
			case WEAPON_CZ75A:
			case WEAPON_TEC9:
			case WEAPON_REVOLVER:
			case WEAPON_GLOCK:
			case WEAPON_P250:
				return PISTOL;
				break;
			case WEAPON_AK47:
			case WEAPON_M4A1:
			case WEAPON_M4A1_SILENCER:
			case WEAPON_GALILAR:
			case WEAPON_AUG:
			case WEAPON_FAMAS:
			case WEAPON_SG556:
				return RIFLE;
				break;
			case WEAPON_P90:
			case WEAPON_BIZON:
			case WEAPON_MP7:
			case WEAPON_MP9:
			case WEAPON_MAC10:
			case WEAPON_UMP45:
				return SMG;
				break;
			case WEAPON_AWP:
			case WEAPON_SSG08:
			case WEAPON_G3SG1:
			case WEAPON_SCAR20:
				return SNIPER;
				break;
			case WEAPON_MAG7:
			case WEAPON_SAWEDOFF:
			case WEAPON_NOVA:
			case WEAPON_XM1014:
			case WEAPON_NEGEV:
			case WEAPON_M249:
				return HEAVY;
				break;
			case WEAPON_KNIFE:
			case WEAPON_KNIFE_BAYONET:
			case WEAPON_KNIFE_BUTTERFLY:
			case WEAPON_KNIFE_FALCHION:
			case WEAPON_KNIFE_FLIP:
			case WEAPON_KNIFE_GUT:
			case WEAPON_KNIFE_KARAMBIT:
			case WEAPON_KNIFE_TACTICAL:
			case WEAPON_KNIFE_M9_BAYONET:
			case WEAPON_KNIFE_PUSH:
			case WEAPON_KNIFE_SURVIVAL_BOWIE:
			case WEAPON_KNIFE_T:
				return KNIFE;
				break;
			case WEAPON_HEGRENADE:
			case WEAPON_FLASHBANG:
			case WEAPON_SMOKEGRENADE:
			case WEAPON_MOLOTOV:
			case WEAPON_INCGRENADE:
			case WEAPON_DECOY:
				return GRENADE;
				break;
			default:
				return KNIFE;
		}
	}
};
