#pragma once

#include <memory>
#include <Windows.h>
#include <winternl.h>
#include <Vector>
#include <string>

#include "ValveSDK.hpp"

// interfaceAddress.
#pragma warning(disable : 6387)

namespace Hack
{
	template <typename ret>
	__forceinline ret* GetInterface(const std::string& szModuleName, const std::string& szInterfaceName)
	{
		using CreateInterface_t = void* (*)(const char*, int*);

		// Note - To do; xor, bypass return address check.
		const auto InterfaceAddress = reinterpret_cast<CreateInterface_t>(GetProcAddress(GetModuleHandleA(szModuleName.c_str()), "CreateInterface"));

		if (InterfaceAddress)
		{
			void* pInterface = nullptr;
			char  cBuf[128];

			// Bruteforce.
			for (uint32_t i = 0; i <= 100; i++)
			{
				memset(static_cast<void*>(cBuf), 0, sizeof(cBuf));

				pInterface = InterfaceAddress(szInterfaceName.c_str(), nullptr);

				// Valid address.
				// Get out.
				if (pInterface != nullptr)
					break;
			}

			// After 100 iterations, if we still have an invalid interface pointer, we want to abort.
			if (pInterface == nullptr)
			{
				char buf[128];
				sprintf_s(buf, "Interface: %s wasnt found in: %s", szInterfaceName.c_str(), szModuleName.c_str());

				throw std::runtime_error(buf);
				exit(EXIT_FAILURE);
			}

			// Succeeded; return the address.
			return static_cast<ret*>(pInterface);
		}
		else
		{
			// note - to do; sprintf_s with data.
			throw std::runtime_error(szModuleName + " wasn't found");
			exit(EXIT_FAILURE);
		}
	}

	class CSGO
	{
	public:
		c_base_client* m_client;
		c_client_mode* m_client_mode;
		c_entity_list* m_entity_list;
		c_var* m_engine_cvars;
		CEngineClient* m_engine;
		CEngineTrace* m_engine_trace;
		c_global_vars* m_globals;
		CDebugOverlay* m_debug_overlay;
		IVModelInfoClient* m_model_info;
		ISurface* m_surface;
		VPanel* m_panel;
		c_direct_device_vtable* m_device;
		c_input* m_input;
		IGameMovement* m_movement;
		IPrediction* m_prediction;
		IMaterialSystem* m_material_system;
		IVRenderView* m_render_view;
		IVModelRender* m_model_render;
		c_client_state* m_client_state;
		c_localize* m_localize;
		IGameEventManager2* m_event_manager;
		IMDLCache* m_mdl_cache;
		void* m_hdn;
		CHudChat* m_hud_chat;
		HWND m_window;
	};

	bool Init();
}

inline Hack::CSGO g_csgo;
