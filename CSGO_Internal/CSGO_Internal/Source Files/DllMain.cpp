#include "../Header Files/Includes.hpp"

uintptr_t __stdcall StartAddress()
{
	uint16_t count = 0;
	count++;

	// Load the cheat once.
	if (count == 1)
		Hack::Init();

	while (true)
	{
		SLEEP(100);

		// We've pressed the unload button on the gui.
		if (g_ctx.bShouldUnload)
		{
			static bool hasUnloadedOnce = false;

			if (!hasUnloadedOnce)
			{
				// Restore hooked functions.
				g_ctx.vmt1->Unhook();
				g_ctx.vmt2->Unhook();
				g_ctx.vmt3->Unhook();
				g_ctx.vmt4->Unhook();
				g_ctx.vmt5->Unhook();
				g_ctx.vmt6->Unhook();
				g_ctx.vmt7->Unhook();
				g_ctx.vmt8->Unhook();

				// Delete our hook pointers.
				// Note - We can also DEALLOC them after initialization if we don't need to unload.
				DEALLOC(g_ctx.vmt1);
				DEALLOC(g_ctx.vmt2);
				DEALLOC(g_ctx.vmt3);
				DEALLOC(g_ctx.vmt4);
				DEALLOC(g_ctx.vmt5);
				DEALLOC(g_ctx.vmt6);
				DEALLOC(g_ctx.vmt7);
				DEALLOC(g_ctx.vmt8);

				hasUnloadedOnce = true;

				// Release the module.
				FreeLibraryAndExitThread(g_ctx.thisModule, EXIT_SUCCESS);
			}
		}
	}
}

BOOL APIENTRY DllMain(HMODULE thisBase, uintptr_t attachReason, void* reserved)
{
	if (attachReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(thisBase);

		g_ctx.thisModule = thisBase;

#if defined DEVELOPER || defined _DEBUG
		FILE* f{};
		AllocConsole();
		freopen_s(&f, "CONOUT$", "w", stdout);

		// Create a safe thread for our hack.
		const HANDLE hThread = Utility::CreateSpoofedThread((LPTHREAD_START_ROUTINE)StartAddress);

		if (hThread)
			CloseHandle(hThread);
#else
		// Direct initialization.
		// Should be used for actual use.
		if (!Hack::Init())
			return FALSE;
#endif
	}

	return TRUE;
}