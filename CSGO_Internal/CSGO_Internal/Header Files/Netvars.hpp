#pragma once

#include "ValveSDK.hpp"
#include "../Header Files/Utility/Hash.hpp"

#include <vector>

#define NETVAR( funcname, type, table, name ) \
__forceinline auto& funcname() \
{ \
	static std::ptrdiff_t offset = g_netvars.get_netvar( fnv( table ), fnv( name ) ); \
    if ( !offset ) \
	{ \
		offset = g_netvars.get_netvar( fnv( table ), fnv( name ) ); \
	} \
	return *reinterpret_cast< type* >( uintptr_t( this ) + offset ); \
}

#define PNETVAR( funcname, type, table, name ) \
__forceinline auto* funcname() \
{ \
	static std::ptrdiff_t offset = g_netvars.get_netvar( fnv( table ), fnv( name ) ); \
    if ( !offset ) \
	{ \
		offset = g_netvars.get_netvar( fnv( table ), fnv( name ) ); \
	} \
	return reinterpret_cast< type* >( uintptr_t( this ) + offset ); \
}

#define NETVAR_OFFSET( funcname, type, table, name, extra ) \
__forceinline auto& funcname() \
{ \
	static std::ptrdiff_t offset = g_netvars.get_netvar( fnv( table ), fnv( name ) ); \
	return *reinterpret_cast< type* >( uintptr_t( this ) + offset + extra ); \
}

class CNetvars
{
public:
	void init();
	std::ptrdiff_t get_netvar(Hash_t table, Hash_t hash) const;
	RecvProp* get_prop(Hash_t table, Hash_t name) const;

public:
	std::ptrdiff_t get_entry(Hash_t hash, RecvTable* table) const;
	RecvTable* get_table(Hash_t hash) const;

private:
	std::vector< RecvTable* > m_tables;
};

extern CNetvars g_netvars;