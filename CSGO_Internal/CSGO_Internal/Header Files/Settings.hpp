#pragma once

#include <memory>
#include <vector>
#include "ValveSDK.hpp"

enum RestrictionType_t
{
	NONE = 0,
	ANTI_UNTRUSTED,
	ANTI_SMAC
};

struct Settings_t
{
	// Hack.
	bool m_bAutoBunnyhop = false;
	bool m_bNoFog = false;

	// GUI.
	color_t m_menuColor = { 251, 199, 85, 255 };
	bool    m_bRainbowAccent = false;
	bool    m_bWatermark = false;
	int     m_iFadeSpeed = 8;

	// Config.
	int m_iRestrictionType = 1; // Start on Anti-Untrust
	int m_iSelectedConfig = 0;
};

inline Settings_t g_config{};