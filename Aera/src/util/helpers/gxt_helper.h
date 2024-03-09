#pragma once
#include <string>
#include "memory/pointers.h"

namespace gxt
{
	inline std::optional<std::string> get_gxt_label(const std::string& key, bool return_key = false)
	{
		if (const auto gxt_via_string = pointers::g_getGxtLabelFromTable(pointers::g_gxtLabels, key.c_str()))
			return gxt_via_string;

		if (const auto gxt_via_joaat = pointers::g_getJoaatedGxtLabelFromTable(pointers::g_gxtLabels, rage::joaat(key)))
			return gxt_via_joaat;

		if (return_key)
			return key;

		return std::nullopt;
	}
}
