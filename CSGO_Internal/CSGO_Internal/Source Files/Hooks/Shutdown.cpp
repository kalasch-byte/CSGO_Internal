#include "../../Header Files/Includes.hpp"

void __fastcall Hooked::Shutdown(void* ecx, void* edx)
{
	// purpose:
	// allow hack to be unloaded
	// as the game is being closed
	// to prevent dumping, crashed process, etc

	// inform other thread that we've closed the game
	g_ctx.bShouldUnload = true;

	// sleep to prevent the game closing too fast for us to unhook
	// it's possible unhooking doesn't start until 1s after this
	// is called, so this step is vital, please don't question me
	//std::this_thread::sleep_for( std::chrono::seconds( 2 ) );

	// call shutdown, resulting in the process closing, without our module present
	Hooked::Original::oShutdown(ecx, edx);
}