#pragma once

#include <Windows.h>
#include <new>
#include <string>
#include <iterator>
#include <vector>
#include <sstream>

namespace Utility
{
	static __forceinline std::string UnicodeToAscii(const std::wstring& szUnciode)
	{
		std::string ascii_str(szUnciode.begin(), szUnciode.end());
		return ascii_str;
	}

	static __forceinline std::wstring AsciiToUnicode(const std::string& szAscii)
	{
		std::wstring unicode_str(szAscii.begin(), szAscii.end());
		return unicode_str;
	}

	inline std::string GetGameDirectory()
	{
		char cBuf[MAX_PATH];
		GetModuleFileNameA(nullptr, cBuf, sizeof(cBuf));

		std::string szGamePath = cBuf;

		// Remove the csgo.exe from the path.
		szGamePath.replace(szGamePath.find("csgo.exe"), sizeof("csgo.exe") - 1, "");

		return szGamePath;
	}

	inline void DeleteFileFromDisk(const char* pFilePathOrName)
	{
		// Valid file?
		if (!std::filesystem::exists(pFilePathOrName))
			return;

		// Delete.
		std::filesystem::remove(pFilePathOrName);
	}

	inline short GetServerPlayerCount()
	{
		uintptr_t uModuleBase = (uintptr_t)GetModuleHandleA("client.dll");

		// Invalid base address.
		if (!uModuleBase)
			return 0;

		// Note - To find; Cheat Engine > go into game > add bot -> Increment, kick bot -> Decrement. (4 byte type)

		int* pPlayerCount = (int*)(uModuleBase + 0xDEADB0);

		if (!pPlayerCount)
			return 0;

		// Dereference pointer to acquire the address.
		return *pPlayerCount + 1;
	}

	inline HANDLE CreateSpoofedThread(LPTHREAD_START_ROUTINE function)
	{
		// Start address for spoofed thread.
		uintptr_t uStartAddress = (uintptr_t)GetProcAddress(GetModuleHandleA("ntdll.dll"), "TpCallbackIndependent") + 0x140;

		// Valid address?
		if (uStartAddress == NULL)
			return nullptr;

		// Create a thread in a suspended state.
		const HANDLE hThread = CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)uStartAddress, nullptr, CREATE_SUSPENDED, nullptr);

		// Valid thread?
		if (hThread == nullptr)
			return nullptr;

		// Get thread registers.
		CONTEXT ctx{};
		ctx.ContextFlags = CONTEXT_ALL;
		GetThreadContext(hThread, &ctx);

		// Set ecx / eax to function.
		ctx.Eax = reinterpret_cast<decltype(ctx.Eax)>(function);

		// Set thread context.
		SetThreadContext(hThread, &ctx);

		// Resume thread execution.
		ResumeThread(hThread);

		return hThread;
	}
}