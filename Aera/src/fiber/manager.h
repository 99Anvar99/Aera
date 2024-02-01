#pragma once
#include "pch/pch.h"
#include "rage/engine.h"
#include "fiber.h"

class manager : public engine::thread
{
public:
	void add(cc* id, fnptr<void()> fn, bool log = true)
	{
		std::lock_guard lck(m_mutex);
		m_fibers.insert({id, std::make_unique<fiber>(fn)});
		if (log)
		LOG_DEBUG("Created fiber {}", id)
	}

	void add(cc* id, u64 count, const fnptr<void()> fn)
	{
		for (u64 i{count}; i; --i)
		{
			add(std::format("{}_{}", id, i).c_str(), fn, false);
		}
		LOG_DEBUG("Created fiber group '{}' with {} fibers", id, count)
	}

	void remove(cc* id)
	{
		std::lock_guard lck(m_mutex);
		m_fibers.erase(id);
	}

	void remove_base(cc* base_id)
	{
		std::lock_guard lck(m_mutex);
		for (const auto& key : m_fibers | std::views::keys)
		{
			if (const auto fbr_id{key}; strstr(fbr_id, base_id))
			{
				m_fibers.erase(fbr_id);
				return;
			}
		}
	}

	void destroy()
	{
		std::lock_guard lck(m_mutex);
		m_fibers.clear();
	}

	void do_run() override
	{
		static bool ensure_main_fiber{(ConvertThreadToFiber(nullptr), true)};
		std::lock_guard lck(m_mutex);
		for (const auto& val : m_fibers | std::views::values)
		{
			val->tick();
		}
	}

private:
	std::recursive_mutex m_mutex{};
	std::map<cc*, std::unique_ptr<fiber>> m_fibers{};
};

inline manager g_manager{};
