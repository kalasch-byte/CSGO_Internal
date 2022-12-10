#pragma once

#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <thread>
#include <iterator>
#include <sstream>
#include <algorithm>
#include <functional>
#include <filesystem>

#include "../Header Files/Renderer/Renderer.hpp"
#include "../Header Files/Renderer/SurfaceRenderer.hpp"

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "ValveSDK.hpp"
#include "Player.hpp"
#include "Interfaces.hpp"
#include "Memory.h"
#include "Math.hpp"
#include "Input.hpp"
#include "Settings.hpp"
#include "GUI.hpp"
#include "Macros.hpp"
#include "Configuration.hpp"

#include "Utility/VMT.hpp"
#include "Utility/Utility.hpp"

// Get-Project -All | Foreach { $_.ConfigurationManager.DeleteConfigurationRow("Release") }

struct Context_t
{
	bool bShouldUnload{};
	bool bInitialized{};
	bool bMenuOpened{};
	bool bShouldUpdateScreenSize{};

	c_base_entity* pLocal{};
	c_user_cmd* pCmd{};
	VMatrix        matrix{};
	uint32_t       uTickrate{};

	int iScreenWidth{};
	int iScreenHeight{};

	std::unique_ptr<CRender>renderer{};
	CGui					gui{};

	std::vector<std::string>szConfigList{};
	uint16_t                uCurrentSelectedConfig{};

	c_render_context* renderCtx{};

	HMODULE thisModule{};

	EasyVMT* vmt1{};
	EasyVMT* vmt2{};
	EasyVMT* vmt3{};
	EasyVMT* vmt4{};
	EasyVMT* vmt5{};
	EasyVMT* vmt6{};
	EasyVMT* vmt7{};
	EasyVMT* vmt8{};
};

inline Context_t g_ctx;

namespace Hooked
{
	void __fastcall Shutdown(void*, void*);
	using Shutdown_t = void(__fastcall*)(void*, void*);

	void __fastcall SceneEnd(void*, void*);
	using SceneEnd_t = void(__fastcall*)(void*, void*);

	void __fastcall OnScreenSizeChanged(void* ecx, void* edx, int old_w, int old_h);
	using OnScreenSizeChanged_t = void(__fastcall*)(void*, void*, int, int);

	void __fastcall PaintTraverse(void*, void*, uint32_t panel, bool repaint, bool allow_force);
	using PaintTraverse_t = void(__fastcall*)(void*, void*, uint32_t, bool, bool);

	long __stdcall WndProc(HWND hwnd, uint32_t msg, uint32_t uparam, long param);
	using WndProc_t = long(__stdcall*)(HWND, uint32_t, uint32_t, long);

	long __stdcall Present(IDirect3DDevice9* device, RECT* src, const RECT* dst, HWND dst_window_override, const RGNDATA* dirty_region);
	using Present_t = long(__stdcall*)(IDirect3DDevice9*, RECT*, const RECT*, HWND, const RGNDATA*);

	long __stdcall Reset(IDirect3DDevice9* device, void* params);
	using Reset_t = long(__stdcall*)(IDirect3DDevice9*, void*);

	bool __stdcall CreateMove(float sample_frametime, c_user_cmd* cmd);
	using CreateMove_t = bool(__stdcall*)(float, c_user_cmd*);

	bool __fastcall FireGameEvent(void* ecx, void* edx, IGameEvent* game_event);
	using FireGameEvent_t = bool(__fastcall*)(void*, void*, IGameEvent*);

	bool __fastcall ShouldDrawFog(void* ecx, void* edx);
	using ShouldDrawFog_t = bool(__fastcall*)(void*, void*);

	namespace Original
	{
		inline Shutdown_t			 oShutdown = nullptr;
		inline SceneEnd_t			 oSceneEnd = nullptr;
		inline OnScreenSizeChanged_t oOnScreenSizeChanged = nullptr;
		inline PaintTraverse_t		 oPaintTraverse = nullptr;
		inline WndProc_t			 oWndProc = nullptr;
		inline Present_t			 oPresent = nullptr;
		inline Reset_t				 oReset = nullptr;
		inline CreateMove_t			 oCreateMove = nullptr;
		inline FireGameEvent_t		 oFireGameEvent = nullptr;
		inline ShouldDrawFog_t		 oShouldDrawFog = nullptr;
	}
}

namespace drawing
{
	template< class t = vec3 >
	bool screen_transform(const vec3& world, t& screen)
	{
		screen.x = g_ctx.matrix[0][0] * world[0] + g_ctx.matrix[0][1] * world[1] + g_ctx.matrix[0][2] * world[2] + g_ctx.matrix[0][3];
		screen.y = g_ctx.matrix[1][0] * world[0] + g_ctx.matrix[1][1] * world[1] + g_ctx.matrix[1][2] * world[2] + g_ctx.matrix[1][3];

		float w = g_ctx.matrix[3][0] * world[0] + g_ctx.matrix[3][1] * world[1] + g_ctx.matrix[3][2] * world[2] + g_ctx.matrix[3][3];

		bool behind = false;

		if (w < 0.001f)
		{
			behind = true;

			screen.x *= 100000;
			screen.y *= 100000;
		}
		else
		{
			behind = false;

			float invw = 1.0f / w;

			screen.x *= invw;
			screen.y *= invw;
		}

		return behind;
	}

	template< class t = vec3 >
	bool w2s(const vec3& world, t& screen)
	{
		if (!screen_transform(world, screen))
		{
			float x = float(g_ctx.iScreenWidth) / 2.0f;
			float y = float(g_ctx.iScreenHeight) / 2.0f;

			x += 0.5f * screen.x * g_ctx.iScreenWidth + 0.5f;
			y -= 0.5f * screen.y * g_ctx.iScreenHeight + 0.5f;

			screen.x = x;
			screen.y = y;

			return true;
		}
		return false;
	}

	inline bool screen_transform_r(const vec3& world, vec3& screen)
	{
		screen.x = g_ctx.matrix[0][0] * world[0] + g_ctx.matrix[0][1] * world[1] + g_ctx.matrix[0][2] * world[2] + g_ctx.matrix[0][3];
		screen.y = g_ctx.matrix[1][0] * world[0] + g_ctx.matrix[1][1] * world[1] + g_ctx.matrix[1][2] * world[2] + g_ctx.matrix[1][3];

		float w = g_ctx.matrix[3][0] * world[0] + g_ctx.matrix[3][1] * world[1] + g_ctx.matrix[3][2] * world[2] + g_ctx.matrix[3][3];
		screen.z = 0.0f;

		bool behind = false;

		float invw = 1.0f / w;

		screen.x *= invw;
		screen.y *= invw;

		return behind;
	}

	inline bool w2r(const vec3& world, vec3& screen)
	{
		if (!screen_transform_r(world, screen))
		{
			float x = float(g_ctx.iScreenWidth) / 2.0f;
			float y = float(g_ctx.iScreenHeight) / 2.0f;

			x += 0.5f * screen.x * g_ctx.iScreenWidth + 0.5f;
			y -= 0.5f * screen.y * g_ctx.iScreenHeight + 0.5f;

			screen.x = x;
			screen.y = y;

			return true;
		}

		return false;
	}

	inline void circle3d(vec3 pos, color_t color, int point_count, float radius, bool fade = false, float rot_start = 0.f, float fade_start = 0.5f, float fade_length = 0.25f)
	{
		float step = Math::pi * 2.0f / point_count;
		std::vector<vec3> points3d;

		int alpha = 255;
		int fade_start_point = 0;
		int fade_end_point = 0;
		int fade_step = 0;

		if (fade)
		{
			fade_start_point = (int)(point_count * fade_start);
			fade_end_point = fade_start_point + (int)(point_count * fade_length);
			fade_step = 255 / (fade_end_point - fade_start_point);
		}

		color_t outer_color = color_t(0, 0, 0, 255);
		color_t inner_color = color;

		for (int i = 0; i < point_count; i++)
		{
			if (fade && i > fade_end_point)
				break;

			float theta = (i * step) - (Math::pi * 2.f * rot_start);

			vec3 world_start(radius * cosf(theta) + pos.x, radius * sinf(theta) + pos.y, pos.z);
			vec3 world_end(radius * cosf(theta + step) + pos.x, radius * sinf(theta + step) + pos.y, pos.z);

			vec2 start, end;
			if (!w2s(world_start, start) || !w2s(world_end, end))
				return;

			if (fade && i >= fade_start_point)
			{
				alpha -= fade_step;

				if (alpha < 0)
					alpha = 0;
			}

			outer_color.a(alpha);
			inner_color.a(alpha);

			g_ctx.renderer->line(outer_color, start.x, start.y + 1, end.x, end.y + 1);
			g_ctx.renderer->line(outer_color, start.x, start.y - 1, end.x, end.y - 1);
			g_ctx.renderer->line(inner_color, start.x, start.y, end.x, end.y);
		}
	}
}