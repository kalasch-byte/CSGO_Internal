#pragma once

#pragma warning(disable : 4996)
#pragma warning(disable : 4018)

#include <Windows.h>
#include <new>
#include <string>
#include <iterator>
#include <vector>
#include <sstream>
#include <intrin.h>

namespace Memory
{
	template <typename t>
	__forceinline t GetVFunc(void* pClassBase, uint16_t uIndex)
	{
		return (t)(*(uintptr_t**)pClassBase)[uIndex];
	}

	inline std::uint8_t* FindSignature(const char* pModule, const char* pSignature)
	{
		const auto hModule = GetModuleHandleA(pModule);

		if (!hModule)
			return nullptr;

		static auto PatternToByte = [](const char* pPattern)
		{
			auto bytes = std::vector<int>{};
			auto start = const_cast<char*>(pPattern);
			auto end = const_cast<char*>(pPattern) + std::strlen(pPattern);

			for (auto current = start; current < end; ++current)
			{
				if (*current == '?')
				{
					++current;

					if (*current == '?')
						++current;

					bytes.push_back(-1);
				}
				else
					bytes.push_back(std::strtoul(current, &current, 16));
			}

			return bytes;
		};

		auto pDosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(hModule);
		auto pNtHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(hModule) + pDosHeader->e_lfanew);

		auto dwImageSize = pNtHeaders->OptionalHeader.SizeOfImage;
		auto iPatternBytes = PatternToByte(pSignature);
		auto pScannedBytes = reinterpret_cast<std::uint8_t*>(hModule);

		auto s = iPatternBytes.size();
		auto d = iPatternBytes.data();

		for (auto i = 0ul; i < dwImageSize - s; ++i)
		{
			bool bFound = true;

			for (auto j = 0ul; j < s; ++j)
			{
				if (pScannedBytes[i + j] != d[j] && d[j] != -1)
				{
					bFound = false;
					break;
				}
			}

			if (bFound)
				return &pScannedBytes[i];
		}

		throw std::runtime_error(std::string("Wrong signature: ") + pSignature);
	}
}