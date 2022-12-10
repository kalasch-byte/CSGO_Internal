#pragma once

#include "Includes.hpp"
#include "Utility/Utility.hpp"

#include <algorithm>
#include <fstream>

namespace ConfigManager
{
	// Note - To do; Encrypt all strings on save, and decrypt them on load.

	inline std::vector<std::string>szConfigStrings =
	{
		 "g_config.m_menuColor_r ",
		 "g_config.m_menuColor_g ",
		 "g_config.m_menuColor_b ",
		 "g_config.m_menuColor_a ",
		 "g_config.m_iFadeSpeed ",
		 "g_config.m_bRainbowAccent ",
		 "g_config.m_iRestrictionType ",
		 "g_config.m_bWatermark ",
		 "g_config.m_bAutoBunnyhop ",
		 "g_config.m_bNoFog "
	};

	inline std::vector<std::string>GetAllConfigs(std::vector<std::string>list)
	{
		// D:\\Steam\\steamapps\\common\\Counter-Strike Global Offensive\\*
		std::string szCsgoDirectory = Utility::GetGameDirectory().c_str();
		szCsgoDirectory += "*";

		WIN32_FIND_DATA Data{};
		HANDLE FileHandle = FindFirstFile(szCsgoDirectory.c_str(), &Data);

		while (FindNextFile(FileHandle, &Data))
		{
			if (!strstr(Data.cFileName, ".cfg") || strlen(Data.cFileName) < 5) // a.cfg == 5
				continue;

			list.push_back(Data.cFileName);
		}

		for (int i = 0; i < list.size(); i++)
		{
			if (strstr(list[i].c_str(), ".cfg"))
				list[i].replace(list[i].find(".cfg"), sizeof(".cfg") - 1, "");
		}

		return list;
	}

	inline void SaveConfig(const char* pFilePathOrName)
	{
		std::ofstream fout{};

		fout.open(pFilePathOrName, std::ios::trunc);

#define SAVE_ITEM(string, variable) \
		{ \
			fout << string << variable << std::endl; \
		} \

		SAVE_ITEM(szConfigStrings[0], *g_config.m_menuColor.r_ptr());
		SAVE_ITEM(szConfigStrings[1], *g_config.m_menuColor.g_ptr());
		SAVE_ITEM(szConfigStrings[2], *g_config.m_menuColor.b_ptr());
		SAVE_ITEM(szConfigStrings[3], *g_config.m_menuColor.a_ptr());

		SAVE_ITEM(szConfigStrings[4], g_config.m_iFadeSpeed);
		SAVE_ITEM(szConfigStrings[5], g_config.m_bRainbowAccent);
		SAVE_ITEM(szConfigStrings[6], g_config.m_iRestrictionType);
		SAVE_ITEM(szConfigStrings[7], g_config.m_bWatermark);

		SAVE_ITEM(szConfigStrings[8], g_config.m_bAutoBunnyhop);
		SAVE_ITEM(szConfigStrings[9], g_config.m_bNoFog);

		fout.close();
	}

	inline void LoadConfig(const char* pFilePathOrName)
	{
		std::ifstream fin;
		std::string   word = "";

		fin.open(pFilePathOrName, std::ifstream::in);

#define LOAD_ITEM(item) \
		{ \
			fin >> word >> item; \
		} \

		LOAD_ITEM(*g_config.m_menuColor.r_ptr());
		LOAD_ITEM(*g_config.m_menuColor.g_ptr());
		LOAD_ITEM(*g_config.m_menuColor.b_ptr());
		LOAD_ITEM(*g_config.m_menuColor.a_ptr());

		LOAD_ITEM(g_config.m_iFadeSpeed);
		LOAD_ITEM(g_config.m_bRainbowAccent);
		LOAD_ITEM(g_config.m_iRestrictionType);
		LOAD_ITEM(g_config.m_bWatermark);

		LOAD_ITEM(g_config.m_bAutoBunnyhop);
		LOAD_ITEM(g_config.m_bNoFog);

		fin.close();
	}

	// Note - To do.
	inline void ResetConfig(const char* name)
	{
		// Default.
	}
}