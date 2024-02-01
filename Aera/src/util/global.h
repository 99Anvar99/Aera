#pragma once
#include "pch/pch.h"
#include "rage/classes.h"
#include "memory/pointers.h"

class global
{
public:
	global(u64 index)
		: index(index)
	{
	}

	global at(u64 value)
	{
		return {index + value};
	}

	global at(u64 array, u64 size)
	{
		return {index + 1 + (array * size)};
	}

	rage::scrValue* value()
	{
		return pointers::g_globals[index >> 18 & 0x3F] + (index & 0x3FFFF);
	}

private:
	u64 index;
};
