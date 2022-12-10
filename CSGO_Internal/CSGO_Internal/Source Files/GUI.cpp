#include "../Header Files/Renderer/Renderer.hpp"
#include "../Header Files/Includes.hpp"
#include "../Header Files/GUI.hpp"
#include "../Header Files/Menu.hpp"

#include <stdio.h>
#include <algorithm>
#include <fstream>

// °

CGui		 gui;
UI::CWindow menu;
UI::CCursor cursor;

void CGui::LegitTab()
{
	static UI::CGroupbox group1{};
	static UI::CGroupbox group2{};
	static UI::CCheckbox checkbox1{};
	static UI::CLabel	  pad{};
	static UI::CCheckbox checkbox2{};
	static UI::CButton	  button1{};

	static bool v1 = false;
	static bool v2 = false;

	menu.ColumnLeft();
	{
		checkbox1.handle(menu.data(), "enable", &v1, "masterswitch for the legitbot");

		group1.start(menu.data(), "group1");
		checkbox2.handle(menu.data(), "checkbox1", &v2, "an example checkbox");

		button1.handle(menu.data(), "button1", "an example button", [] {});
		group1.end(menu.data());
	}

	menu.ColumnRight();
	{
		// Allow for correct alignment.
		pad.handle(menu.data(), nullptr);

		group2.start(menu.data(), "group2");
		group2.end(menu.data());
	}
}

void CGui::RageTab()
{
	static UI::CGroupbox group1{};
	static UI::CGroupbox group2{};
	static UI::CGroupbox group3{};
	static UI::CLabel    pad{};
	static UI::CCheckbox checkbox1{};

	static bool v1 = false;

	menu.ColumnLeft();
	{
		checkbox1.handle(menu.data(), ("enable"), &v1, "masterswitch for the ragebot");

		group1.start(menu.data(), ("group1"));
		group1.end(menu.data());

		group2.start(menu.data(), ("group3"));
		group2.end(menu.data());
	}

	menu.ColumnRight();
	{
		// Allow for correct alignment.
		pad.handle(menu.data(), nullptr);

		group3.start(menu.data(), ("group2"));
		group3.end(menu.data());
	}
}

void CGui::AntiHitTab()
{
	static UI::CGroupbox group1;
	static UI::CGroupbox group2;
	static UI::CGroupbox group3;
	static UI::CCheckbox enable;
	static UI::CLabel	  pad{};

	static bool v1 = false;

	menu.ColumnLeft();
	{
		enable.handle(menu.data(), ("enable"), &v1, "masterswitch for the anti-hit system");

		group1.start(menu.data(), ("group1"));
		group1.end(menu.data());

		group2.start(menu.data(), ("group2"));
		group2.end(menu.data());
	}

	menu.ColumnRight();
	{
		// Allow for correct alignment.
		pad.handle(menu.data(), nullptr);

		group3.start(menu.data(), ("group3"));
		group3.end(menu.data());
	}
}

void CGui::PlayersTab()
{
	static UI::CGroupbox group1{};
	static UI::CGroupbox group2{};

	menu.ColumnLeft();
	{
		group1.start(menu.data(), ("group1"));
		group1.end(menu.data());
	}

	menu.ColumnRight();
	{
		group2.start(menu.data(), ("group2"));
		group2.end(menu.data());
	}
}

void CGui::VisualsTab()
{
	static UI::CGroupbox group1{};
	static UI::CGroupbox group2{};

	menu.ColumnLeft();
	{
		group1.start(menu.data(), "group1");
		group1.end(menu.data());
	}

	menu.ColumnRight();
	{
		group2.start(menu.data(), "group2");
		group2.end(menu.data());
	}
}

void CGui::SkinsTab()
{
	static UI::CGroupbox group1{};
	static UI::CButton   saveConfig{};
	static UI::CButton   loadConfig{};

	static UI::CGroupbox group2{};
	static UI::CGroupbox group3{};
	static UI::CGroupbox group4{};
	static UI::CGroupbox group5{};

	menu.ColumnLeft();
	{
		group1.start(menu.data(), ("config"));
		saveConfig.handle(menu.data(), "save", "saves the configuration for skins", [] {});
		loadConfig.handle(menu.data(), "load", "loads the configuration for skins", [] {});
		group1.end(menu.data());

		group2.start(menu.data(), ("skins"));
		group2.end(menu.data());

		group3.start(menu.data(), ("models"));
		group3.end(menu.data());
	}

	menu.ColumnRight();
	{
		group4.start(menu.data(), ("viewmodel"));
		group4.end(menu.data());

		group5.start(menu.data(), ("sound"));
		group5.end(menu.data());
	}
}

void CGui::MiscTab()
{
	static UI::CGroupbox group1{};
	static UI::CGroupbox group2{};
	static UI::CGroupbox group3{};
	static UI::CGroupbox group4{};
	static UI::CGroupbox group5{};
	static UI::CGroupbox group6{};
	static UI::CGroupbox group7{};

	static UI::CCheckbox autoBhop{};
	static UI::CCheckbox noFog{};

	menu.ColumnLeft();
	{
		group1.start(menu.data(), "matchmaking");
		group1.end(menu.data());

		group2.start(menu.data(), "movement");
		autoBhop.handle(menu.data(), "auto bhop", &g_config.m_bAutoBunnyhop, "allows you to always consistently bunnyhop");
		group2.end(menu.data());

		group5.start(menu.data(), "freecam");
		group5.end(menu.data());

		group6.start(menu.data(), "performance");
		noFog.handle(menu.data(), "no fog", &g_config.m_bNoFog, "entirely remove fog effects");
		group6.end(menu.data());
	}

	menu.ColumnRight();
	{
		group3.start(menu.data(), "misc");
		group3.end(menu.data());

		group4.start(menu.data(), "identity");
		group4.end(menu.data());

		group7.start(menu.data(), "buy bot");
		group7.end(menu.data());
	}
}

void CGui::ConfigTab()
{
	static UI::CGroupbox group1{};
	static UI::CGroupbox group2{};
	static UI::CGroupbox group3{};
	static UI::CGroupbox group4{};

	static UI::CColorpicker menuColor{};
	static UI::CSlider       menuFadeSpeed{};
	static UI::CCheckbox     rainbowMode{};
	static UI::CCheckbox	  watermark{};
	static UI::CDropdown	  restrictionMode{};
	static UI::CButton		  unloadHack{};

	static UI::CDropdown configList{};
	static UI::CButton   createConfig{};
	static UI::CButton   saveConfig{};
	static UI::CButton   defaultConfig{};
	static UI::CButton   deleteConfig{};
	static UI::CButton   loadConfig{};
	static UI::CButton   refreshConfigs{};
	static UI::CButton	  openConfigPath{};

	static UI::CTextbox configNameTextBox{};

	menu.ColumnLeft();
	{
		group1.start(menu.data(), "hack");

		menuColor.handle(menu.data(), "accent", g_config.m_menuColor.r_ptr(), g_config.m_menuColor.g_ptr(), g_config.m_menuColor.b_ptr());
		menuFadeSpeed.handle(menu.data(), "fade speed", &g_config.m_iFadeSpeed, 1, 14);
		rainbowMode.handle(menu.data(), "rainbow mode", &g_config.m_bRainbowAccent, "sets the gui forecolor to rainbow");
		restrictionMode.handle(menu.data(), "restrictions", { "none", "anti-untrusted", "anti-smac" }, &g_config.m_iRestrictionType);
		watermark.handle(menu.data(), "watermark", &g_config.m_bWatermark, "display the onetap.su trademark in the top right corner");
		unloadHack.handle(menu.data(), "rage quit", "releases the module from memory", [] { g_ctx.bShouldUnload = true; });

		openConfigPath.handle(menu.data(), "open path", "opens the directory which contains configurations", []
							  {
								  char buf[128];
								  const std::string dir = Utility::GetGameDirectory();

								  sprintf_s(buf, "%s%s", "explorer.exe ", dir.c_str());
								  system(buf);
							  });

		group1.end(menu.data());
	}

	menu.ColumnRight();
	{
		group2.start(menu.data(), ("new config"));

		// textbox should be here for the name.
		static std::string cfg_name = "";
		configNameTextBox.handle(menu.data(), "new config", &cfg_name);

		createConfig.handle(menu.data(), "create", "creates a new configuration", []
							{
								char buf[128];
								sprintf_s(buf, "%s%s", cfg_name.c_str(), ".cfg");

								// Does the file not already exist and has a valid length?
								if (!std::filesystem::exists(buf) && strlen(buf) > 4)
								{
									char buf2[128];
									sprintf_s(buf2, "%s%s", ".\\", buf);

									HANDLE hFile = CreateFile(buf2, (GENERIC_READ | GENERIC_WRITE), FILE_SHARE_READ, nullptr, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);

									if (hFile)
										CloseHandle(hFile);

									// Clear the config name textbox.
									cfg_name.clear();

									// Refresh after creating a new configuration.
									if (g_ctx.szConfigList.size())
										g_ctx.szConfigList.clear();

									g_ctx.szConfigList = ConfigManager::GetAllConfigs(g_ctx.szConfigList);
								}
							});

		group2.end(menu.data());

		char buf[32];
		std::string temp = g_ctx.szConfigList.at(g_config.m_iSelectedConfig).c_str();

		sprintf_s(buf, "%s%s", "config: ", temp.c_str());

		group3.start(menu.data(), buf);

		saveConfig.handle(menu.data(), "save", "saves the current configuration", []
						  {
							  std::string e = g_ctx.szConfigList.at(g_config.m_iSelectedConfig).c_str();
							  e += ".cfg";

							  ConfigManager::SaveConfig(e.c_str());
						  });

		defaultConfig.handle(menu.data(), "default", "resets the current configuration to the default settings", [] {});
		deleteConfig.handle(menu.data(), "delete", "deletes the current configuration", []
							{
								// Dont delete the default config.
								// Will result in a crash.
								if (!strstr(g_ctx.szConfigList.at(g_config.m_iSelectedConfig).c_str(), "default"))
								{
									std::string e = g_ctx.szConfigList.at(g_config.m_iSelectedConfig).c_str();
									e += ".cfg";

									Utility::DeleteFileFromDisk(e.c_str());

									// Refresh the configurations.
									if (g_ctx.szConfigList.size())
										g_ctx.szConfigList.clear();

									g_ctx.szConfigList = ConfigManager::GetAllConfigs(g_ctx.szConfigList);
								}
							});

		group3.end(menu.data());

		group4.start(menu.data(), ("select config"));

		configList.handle(menu.data(), "", g_ctx.szConfigList, &g_config.m_iSelectedConfig);
		loadConfig.handle(menu.data(), "load", "loads the content from the currently selected configuration", []
						  {
							  std::string e = g_ctx.szConfigList.at(g_config.m_iSelectedConfig).c_str();
							  e += ".cfg";

							  ConfigManager::LoadConfig(e.c_str());
						  });

		refreshConfigs.handle(menu.data(), "refresh", "updates the current config list to acquire newly imported configurations", []
							  {
								  // Manually deleted the default config.
								  // Will result in a crash, lets handle this.
								  if (!std::filesystem::exists(".\\default.cfg"))
								  {
									  HANDLE hFile = CreateFile(".\\default.cfg", (GENERIC_READ | GENERIC_WRITE), FILE_SHARE_READ, nullptr, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);

									  if (hFile)
										  CloseHandle(hFile);
								  }

								  // Refresh.
								  if (g_ctx.szConfigList.size())
									  g_ctx.szConfigList.clear();

								  // Reacquire.
								  g_ctx.szConfigList = ConfigManager::GetAllConfigs(g_ctx.szConfigList);
							  });

		group4.end(menu.data());
	}

	// At the end to prevent underlap.
	restrictionMode.dropdown(menu.data());
	configList.dropdown(menu.data());
	menuColor.picker(menu.data());
}

void CGui::Render()
{
	static bool doneOnce = false;

	if (!doneOnce)
	{
		// Start on the config tab.
		// Note - to do; just set both variables 7 in the storage type.
		menu.m_current_tab = 7;
		menu.m_tab = 7;

		doneOnce = true;
	}

	if (!menu.handle(szMenuTabs, UI::UIcolor_t(g_config.m_menuColor.r(), g_config.m_menuColor.g(), g_config.m_menuColor.b())))
		return;

	switch (menu.m_current_tab)
	{
		case Tabs_t::TAB_LEGIT:
			LegitTab();
			break;

		case Tabs_t::TAB_RAGE:
			RageTab();
			break;

		case Tabs_t::TAB_ANTI_HIT:
			AntiHitTab();
			break;

		case Tabs_t::TAB_PLAYERS:
			PlayersTab();
			break;

		case Tabs_t::TAB_VISUALS:
			VisualsTab();
			break;

		case Tabs_t::TAB_SKINS:
			SkinsTab();
			break;

		case Tabs_t::TAB_MISC:
			MiscTab();
			break;

		case Tabs_t::TAB_CONFIG:
			ConfigTab();
			break;
	}

	cursor.handle();
}