#pragma once

#pragma warning(disable : 4307) // '*': integral constant overflow
#pragma warning(disable : 4244) // possible loss of data

#include <cstdint>
#include <string>

using Hash_t = unsigned int;

// used for compile-time FNV-1a 32bit hashes.
#define fnv( str )                                 \
    [&]() {                                        \
        constexpr Hash_t out = Hash::Fnv1a(str); \
                                                   \
        return out;                                \
    }()

// used for compile-time FNV-1a 32bit hashes when above macro cant be used for constexpr variables.
#define FNV_CONST(str) Hash::fnv1a_32(str)

namespace Hash // FNV-1a ( Fowler-Noll-Vo hash ).
{
	// FNV-1a constants.
	enum : Hash_t
	{
		FNV1A_PRIME = 0x1000193,
		FNV1A_BASIS = 0x811C9DC5
	};

	// Compile-time strlen.
	__forceinline constexpr size_t CtStrlen(const char* str)
	{
		size_t out = 1;

		for (; str[out] != '\0'; ++out);

		return out;
	}

	// Hash data.
	__forceinline constexpr Hash_t Fnv1a(const uint8_t* data, const size_t len)
	{
		Hash_t out = FNV1A_BASIS;

		for (size_t i = 0; i < len; ++i)
		{
			out = (out ^ data[i]) * FNV1A_PRIME;
		}

		return out;
	}

	// Hash c-style string.
	__forceinline constexpr Hash_t Fnv1a(const char* str)
	{
		Hash_t out = FNV1A_BASIS;
		size_t len = CtStrlen(str);

		for (size_t i = 0; i < len; ++i)
		{
			out = (out ^ str[i]) * FNV1A_PRIME;
		}

		return out;
	}

	// Hash C++-style string (runtime only).
	__forceinline Hash_t Fnv1a(const std::string& str)
	{
		return Fnv1a((uint8_t*)str.c_str(), str.size());
	}
}