#pragma once

#include <vector>
#include <string>
#include <iterator>
#include "Renderer/Renderer.hpp"

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

class CGui
{
	enum Tabs_t
	{
		TAB_LEGIT,
		TAB_RAGE,
		TAB_ANTI_HIT,
		TAB_PLAYERS,
		TAB_VISUALS,
		TAB_SKINS,
		TAB_MISC,
		TAB_CONFIG
	};

	// static const?
	std::vector<std::string>szMenuTabs =
	{
		"legit",
		"rage",
		"anti-hit",
		"players",
		"visuals",
		"skins",
		"misc",
		"config"
	};

	void LegitTab();
	void RageTab();
	void AntiHitTab();
	void PlayersTab();
	void VisualsTab();
	void SkinsTab();
	void MiscTab();
	void ConfigTab();

public:
	void Render();

	vec2 BottomLeftOfMenu{};
};

extern CGui gui;