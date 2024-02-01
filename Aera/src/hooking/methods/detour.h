#pragma once
#include "memory/mem.h"
#include "minhook/MinHook.h"

class MinHook
{
public:
	MinHook()
	{
		MH_Initialize();
	}

	~MinHook()
	{
		MH_Uninitialize();
	}

	template <typename T>
	static std::optional<LPVOID> get_corrected_target(T& target)
	{
		mem ptr{static_cast<void*>(target)};
		//Fix jumps (E8 = call, E9 = jmp)
		while (ptr.as<u8&>() == 0xE9 || ptr.as<u8&>() == 0xE8)
		{
			ptr = ptr.call();
		}
		return ptr.as<PVOID>();
	}

	template <typename T>
	static bool create(T target, T detour, T& original)
	{
		if (!target || !detour)
		{
			return false;
		}
		MH_STATUS status{MH_CreateHook(target, static_cast<LPVOID>(detour), static_cast<LPVOID*>(&original))};
		if (status == MH_OK)
		{
			return true;
		}
		if (status != MH_ERROR_NOT_INITIALIZED && status != MH_ERROR_UNSUPPORTED_FUNCTION && status !=
			MH_ERROR_MEMORY_ALLOC && status != MH_ERROR_MEMORY_PROTECT)
		{
			if (const std::optional<LPVOID> corrected_target{get_corrected_target<T>(target)}; corrected_target.has_value())
			{
				status = MH_CreateHook(corrected_target.value(), static_cast<LPVOID>(detour),
				                       static_cast<LPVOID*>(&original));
				if (status == MH_OK)
				{
					return true;
				}
				return false;
			}
		}
		return false;
	}

	template <typename T>
	static bool destroy(T target)
	{
		return MH_RemoveHook(static_cast<LPVOID>(target)) == MH_OK;
	}

	template <typename T>
	static bool queue_enable(T target)
	{
		return MH_QueueEnableHook(static_cast<LPVOID>(target)) == MH_OK;
	}

	template <typename T>
	static bool queue_disable(T target)
	{
		return MH_QueueDisableHook(static_cast<LPVOID>(target)) == MH_OK;
	}

	static bool apply()
	{
		return MH_ApplyQueued() == MH_OK;
	}
};

class detour
{
public:
	detour(cc* name, void* ptr, void* dtr, bool hook = true) : m_name(name), m_dtr(dtr), m_ptr(ptr), m_og(ptr), m_hook(hook)
	{
		if (m_hook)
		{
			MinHook::create(m_ptr, m_dtr, m_og);
		}
	}

	~detour()
	{
		if (m_hook)
		{
			MinHook::destroy(m_ptr);
		}
	}

	void enable()
	{
		if (m_hook)
		{
			MinHook::queue_enable(m_ptr);
		}
	}

	void disable()
	{
		if (m_hook)
		{
			MinHook::queue_disable(m_ptr);
		}
	}

	static void enable_all()
	{
		MH_QueueEnableHook(nullptr);
	}

	static void disable_all()
	{
		MH_QueueDisableHook(nullptr);
	}

	template <typename T>
	T get_original()
	{
		return static_cast<T>(m_og);
	}

	cc* m_name{};
	void* m_dtr{};
	void* m_ptr{};
	void* m_og{};
	bool m_hook{};
};
