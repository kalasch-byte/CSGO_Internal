#include "../Header Files/Netvars.hpp"
#include "../Header Files/Includes.hpp"
#include "../Header Files/Interfaces.hpp"

CNetvars g_netvars;

void CNetvars::init()
{
	for (auto clientClass = g_csgo.m_client->get_all_classes(); !!clientClass; clientClass = clientClass->m_next)
	{
		auto table = clientClass->m_table;
		m_tables.push_back(table);
	}
}

RecvTable* CNetvars::get_table(Hash_t hash) const
{
	if (m_tables.empty())
		return nullptr;

	for (auto& table : m_tables)
	{
		if (!table)
			continue;

		if (hash == Hash::Fnv1a(table->m_pNetTableName))
			return table;
	}

	return nullptr;
}

// Iterating this too much results in a stack overflow, so thats cool.
RecvProp* CNetvars::get_prop(Hash_t data, Hash_t name) const
{
	RecvProp* prop{};
	RecvTable* child{};

	auto table = get_table(data);

	if (!table)
		return nullptr;

	for (int i{}; i < table->m_nProps; ++i)
	{
		prop = &table->m_pProps[i];
		child = prop->m_pDataTable;

		if (child && child->m_nProps)
		{
			auto tmp = get_prop(Hash::Fnv1a(child->m_pNetTableName), name);

			if (tmp)
				return tmp;
		}

		if (name != Hash::Fnv1a(prop->m_pVarName))
			continue;

		return prop;
	}

	return nullptr;
}

std::ptrdiff_t CNetvars::get_entry(Hash_t name, RecvTable* table) const
{
	std::ptrdiff_t ret{};
	RecvProp* prop;
	RecvTable* child;

	for (int i{}; i < table->m_nProps; ++i)
	{
		prop = &table->m_pProps[i];
		child = prop->m_pDataTable;

		if (child && child->m_nProps)
		{
			auto tmp = get_entry(name, child);

			if (tmp)
				ret += prop->m_Offset + tmp;
		}

		if (name != Hash::Fnv1a(prop->m_pVarName))
			continue;

		return prop->m_Offset + ret;
	}

	return ret;
}

std::ptrdiff_t CNetvars::get_netvar(Hash_t data, Hash_t name) const
{
	std::ptrdiff_t ret{};

	auto table = get_table(data);
	if (!table)
		return 0;

	ret = get_entry(name, table);

	return ret;
}
