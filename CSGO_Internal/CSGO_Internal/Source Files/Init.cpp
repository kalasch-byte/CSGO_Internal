#include <algorithm>

#include "../Header Files/Interfaces.hpp"
#include "../Header Files/Includes.hpp"

bool Hack::Init()
{
	// Clear the console of any previous data.
	system("cls");

	bool shouldStart = false;

	// Use a bool rather then SLEEP so we can inject when the game isn't fully loaded.
	while (!GetModuleHandleA("serverbrowser.dll"))
		shouldStart = false;

	// Found serverbrowser.dll, lets begin.
	shouldStart = true;

	if (shouldStart)
	{
		// Create the default configuration file ONLY if it doesn't already exists.
		// Note - Should we just use the CREATE_NEW parameter instead?
		if (!std::filesystem::exists(".\\default.cfg"))
		{
			const HANDLE hFile = CreateFile(".\\default.cfg", (GENERIC_READ | GENERIC_WRITE), FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

			if (hFile)
				CloseHandle(hFile);
		}

		// Grab our configs.
		g_ctx.szConfigList = ConfigManager::GetAllConfigs(g_ctx.szConfigList);

		// Set the start config dropdown list value to the default configuration.
		{
			uint16_t iterator = 0;

			for (int i = 0; i < g_ctx.szConfigList.size(); i++)
			{
				if (strstr(g_ctx.szConfigList[i].c_str(), "default"))
				{
					iterator = i;
					break;
				}
			}

			g_config.m_iSelectedConfig = iterator;
		}

		// Get CS:GO window name.
		g_csgo.m_window = FindWindowA("Valve001", nullptr);

		// Initialize interfaces.
		g_csgo.m_client = GetInterface<c_base_client>("client.dll", "VClient018");
		g_csgo.m_entity_list = GetInterface<c_entity_list>("client.dll", "VClientEntityList003");
		g_csgo.m_engine = GetInterface<CEngineClient>("engine.dll", "VEngineClient014");
		g_csgo.m_panel = GetInterface<VPanel>("vgui2.dll", "VGUI_Panel009");
		g_csgo.m_surface = GetInterface<ISurface>("vguimatsurface.dll", "VGUI_Surface031");
		g_csgo.m_material_system = GetInterface<IMaterialSystem>("materialsystem.dll", "VMaterialSystem080");
		g_csgo.m_model_info = GetInterface<IVModelInfoClient>("engine.dll", "VModelInfoClient004");
		g_csgo.m_model_render = GetInterface<IVModelRender>("engine.dll", "VEngineModel016");
		g_csgo.m_render_view = GetInterface<IVRenderView>("engine.dll", "VEngineRenderView014");
		g_csgo.m_engine_cvars = GetInterface<c_var>("vstdlib.dll", "VEngineCvar007");
		g_csgo.m_localize = GetInterface<c_localize>("localize.dll", "Localize_001");
		g_csgo.m_event_manager = GetInterface<IGameEventManager2>("engine.dll", "GAMEEVENTSMANAGER002");
		g_csgo.m_debug_overlay = GetInterface<CDebugOverlay>("engine.dll", "VDebugOverlay004");
		g_csgo.m_input = GetInterface<c_input>("inputsystem.dll", "InputSystemVersion001");
		g_csgo.m_engine_trace = GetInterface<CEngineTrace>("engine.dll", "EngineTraceClient004");
		g_csgo.m_movement = GetInterface<IGameMovement>("client.dll", "GameMovement001");
		g_csgo.m_prediction = GetInterface<IPrediction>("client.dll", "VClientPrediction001");
		g_csgo.m_mdl_cache = GetInterface<IMDLCache>("datacache.dll", "MDLCache004");

		// Manual interfaces.
		g_csgo.m_globals = **reinterpret_cast<c_global_vars***>((*reinterpret_cast<uintptr_t**>(g_csgo.m_client))[11] + 10);
		g_csgo.m_device = **reinterpret_cast<c_direct_device_vtable***>(Memory::FindSignature("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);
		g_csgo.m_input = *reinterpret_cast<c_input**>(Memory::FindSignature("client.dll", "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10") + 1);
		g_csgo.m_client_mode = **reinterpret_cast<c_client_mode***>((*reinterpret_cast<uintptr_t**>(g_csgo.m_client))[10] + 5);
		g_csgo.m_client_state = **reinterpret_cast<c_client_state***>(Memory::FindSignature("engine.dll", "A1 ? ? ? ? 8B 80 ? ? ? ? C3") + 1);

		// Acquire netvars.
		g_netvars.init();

		// Create new VMT instances.
		g_ctx.vmt1 = new EasyVMT(g_csgo.m_device);
		g_ctx.vmt2 = new EasyVMT(g_csgo.m_device);
		g_ctx.vmt3 = new EasyVMT(g_csgo.m_client);
		g_ctx.vmt4 = new EasyVMT(g_csgo.m_render_view);
		g_ctx.vmt5 = new EasyVMT(g_csgo.m_surface);
		g_ctx.vmt6 = new EasyVMT(g_csgo.m_panel);
		g_ctx.vmt7 = new EasyVMT(g_csgo.m_client_mode);
		g_ctx.vmt8 = new EasyVMT(g_csgo.m_event_manager);

		// Set the hook originals.
		Hooked::Original::oReset = (Hooked::Reset_t)g_ctx.vmt1->GetVirtualFunction(g_csgo.m_device, 16);
		Hooked::Original::oPresent = (Hooked::Present_t)g_ctx.vmt2->GetVirtualFunction(g_csgo.m_device, 17);
		Hooked::Original::oShutdown = (Hooked::Shutdown_t)g_ctx.vmt3->GetVirtualFunction(g_csgo.m_client, 4);
		Hooked::Original::oSceneEnd = (Hooked::SceneEnd_t)g_ctx.vmt4->GetVirtualFunction(g_csgo.m_render_view, 9);
		Hooked::Original::oOnScreenSizeChanged = (Hooked::OnScreenSizeChanged_t)g_ctx.vmt5->GetVirtualFunction(g_csgo.m_surface, 116);
		Hooked::Original::oPaintTraverse = (Hooked::PaintTraverse_t)g_ctx.vmt6->GetVirtualFunction(g_csgo.m_panel, 41);
		Hooked::Original::oCreateMove = (Hooked::CreateMove_t)g_ctx.vmt7->GetVirtualFunction(g_csgo.m_client_mode, 24);
		Hooked::Original::oFireGameEvent = (Hooked::FireGameEvent_t)g_ctx.vmt8->GetVirtualFunction(g_csgo.m_event_manager, 9);
		Hooked::Original::oShouldDrawFog = (Hooked::ShouldDrawFog_t)g_ctx.vmt7->GetVirtualFunction(g_csgo.m_client_mode, 17);
		Hooked::Original::oWndProc = reinterpret_cast<decltype(&Hooked::WndProc)>(SetWindowLong(g_csgo.m_window, -4, (long)Hooked::WndProc));

		// Create our VMT hooks.
		g_ctx.vmt1->Hook(Hooked::Reset, 16);
		g_ctx.vmt2->Hook(Hooked::Present, 17);
		g_ctx.vmt3->Hook(Hooked::Shutdown, 4);
		g_ctx.vmt4->Hook(Hooked::SceneEnd, 9);
		g_ctx.vmt5->Hook(Hooked::OnScreenSizeChanged, 116);
		g_ctx.vmt6->Hook(Hooked::PaintTraverse, 41);
		g_ctx.vmt7->Hook(Hooked::CreateMove, 24);
		g_ctx.vmt8->Hook(Hooked::FireGameEvent, 9);
		g_ctx.vmt7->Hook(Hooked::ShouldDrawFog, 17);

		// Clear the console.
		g_csgo.m_engine->ClientCmd_Unrestricted("clear");

		// Smooth the menu.
		g_csgo.m_engine->ClientCmd_Unrestricted("fps_max 0");
	}

	// Succeeded.
	return true;
}