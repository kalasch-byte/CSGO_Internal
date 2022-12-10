#include "../Header Files/Includes.hpp"
#include "math.h"
#include "../Header Files/Interfaces.hpp"
#include "../Header Files/Utility/Utility.hpp"

mstudiobbox_t* c_base_entity::GetHitbox(int hitbox)
{
	if (!Model())
		return nullptr;

	studiohdr_t* Hdr = g_csgo.m_model_info->GetStudioModel(Model());
	if (!Hdr)
		return nullptr;

	mstudiohitboxset_t* Set = Hdr->pHitboxSet(HitboxSet());
	if (!Set)
		return nullptr;

	return Set->pHitbox(hitbox);
}

bool c_base_entity::GetHitboxPos(int hitbox, vec3& out)
{
	matrix3x4_t matrix[128];
	mstudiobbox_t* box;

	box = GetHitbox(hitbox);

	if (box)
	{
		vec3 min, max;

		box->m_Mins = Math::VectorRotate(box->m_Mins, box->m_Rotation);
		box->m_Maxs = Math::VectorRotate(box->m_Maxs, box->m_Rotation);

		Math::VectorTransformASM(&box->m_Mins[0], matrix[box->m_Bone], &min[0]);
		Math::VectorTransformASM(&box->m_Maxs[0], matrix[box->m_Bone], &max[0]);

		out = (min + max).center();

		return !out.is_zero();
	}

	return false;
}

bool c_base_entity::Info(player_info_t* playerInfo)
{
	return g_csgo.m_engine->GetPlayerInfo(this->EntIndex(), playerInfo);
}

bool c_base_entity::IsFlashed()
{
	// Use animlayers.
	const auto& layer = this->GetAnimLayer(9); // FLASHED
	auto sequence = this->GetSequenceAct(layer.m_nSequence);

	return false;
}

c_base_weapon* c_base_entity::Weapon()
{
	return reinterpret_cast<c_base_weapon*>(g_csgo.m_entity_list->get_entity_from_base_handle(this->ActiveWeapon()));
}

void c_base_entity::RunThink()
{
	// Probably outdated.
	static const auto unknownFunction = reinterpret_cast<void(__thiscall*)(int)>(Memory::FindSignature("client.dll", "55 8B EC 56 57 8B F9 8B B7 ? ? ? ? 8B C6"));

	int& think = *(int*)((uintptr_t)this + 64);

	if (think != -1 && think > 0 && think <= this->GetTickbase())
	{
		think = -1;
		unknownFunction(0);
		this->Think();
	}
}

void c_base_entity::invalidate_bone_cache()
{
	// int __thiscall InvalidateBoneCache(int this)
	// 80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81

	/*
	.text:1019CDD0 80 3D 88 AA A5 10+                cmp     s_bEnableInvalidateBoneCache, 0
	.text:1019CDD7 74 16                             jz      short locret_1019CDEF
	.text:1019CDD9 A1 44 F4 F7 14                    mov     eax, MostRecentBoneCounter
	.text:1019CDDE 48                                dec     eax
	.text:1019CDDF C7 81 14 29 00 00+                mov     dword ptr [ecx+2914h], 0FF7FFFFFh
	.text:1019CDE9 89 81 80 26 00 00                 mov     [ecx+2680h], eax
	*/

	static const auto invalidateBoneCache = Memory::FindSignature("client.dll", "80 ? ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 ? ? ? ? ? ? ? ? ? 89 ? ? ? ? ? C3");

	ulong recentBoneCounter = **(ulong**)(invalidateBoneCache + 10);

	*(ulong*)(uintptr_t(this) + 0x2680) = recentBoneCounter - 1;
	*(ulong*)(uintptr_t(this) + 0x2914) = 0xFF7FFFFF;

	WritableBones() = 0;
}

bool c_base_entity::IsVisible(c_base_entity* attacker, const vec3& src, const vec3& dst)
{
	c_trace_filter filter(attacker);
	c_game_trace tr;
	ray_t ray;

	ray.Init(src, dst);

	g_csgo.m_engine_trace->TraceRay(ray, MASK_SHOT | CONTENTS_HITBOX, &filter, &tr);

	return tr.m_ent == this || tr.m_fraction > 0.97f;
}

bool c_base_weapon::CanFire()
{
	float time = g_ctx.pLocal->GetTickbase() * g_csgo.m_globals->m_curtime;

	// Something wrong in prediction, tickbase lagging behind by 1 tick
	// Remove this once prediction is actually p plz
	time += g_csgo.m_globals->m_interval_per_tick;

	if (g_ctx.pLocal->NextAttack() <= time)
		return (GetNextPrimaryAttack() <= time);

	return false;
}

std::string c_base_weapon::PrintName()
{
	const auto raw = GetName();
	const auto localized = g_csgo.m_localize->localize(raw);

	return Utility::UnicodeToAscii(localized);
}