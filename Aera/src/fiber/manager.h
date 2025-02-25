#pragma once
#include "pch/pch.h"
#include "fiber.h"
#include <functional>
#include "threads.h"

class manager
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
		for (const auto& key : m_fibers)
		{
			if (std::string_view{ key.first }.starts_with(base_id))
			{
				m_fibers.erase(key.first);
			}
		}
	}

	void destroy()
	{
		std::lock_guard lck(m_mutex);
		m_fibers.clear();
	}

	void on_game_tick()
	{
		execute_as_script("main_persistent"_joaat, [this]() { this->do_run(); });
	}

	void do_run()
	{
		static bool ensure_main_fiber{(ConvertThreadToFiber(nullptr), true)};
		std::lock_guard lck(m_mutex);
		for (const auto& val : m_fibers)
		{
			val.second->tick();
		}
	}

private:
	std::recursive_mutex m_mutex{};
	std::map<cc*, std::unique_ptr<fiber>> m_fibers{};
};

inline manager g_manager{};
