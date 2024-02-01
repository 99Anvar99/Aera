#pragma once
#include "fiber.h"
#include "threads.h"
#include "core/logger.h"

class dxFiberMgr {
public:
	void add(std::unique_ptr<fiber> fbr, char const* fbr_id)
	{
		std::lock_guard lck(m_mutex);
		m_fibers.insert({ fbr_id, std::move(fbr) });
	}

	void remove(char const* fbr_id)
	{
		std::lock_guard lck(m_mutex);
		m_fibers.erase(fbr_id);
	}

	void remove_all()
	{
		std::lock_guard lck(m_mutex);
		m_fibers.clear();
	}

	void scr_tick()
	{
		execute_under_hybrid_thr(std::mem_fn(&dxFiberMgr::tick), this);
	}

	void tick()
	{
		if (static bool ensure_main_fbr = (ConvertThreadToFiber(nullptr), true); !ensure_main_fbr) 
		{
			LOG(Info, "DxFiberMgr Failed to ensureMainFbr")
			return;
		}

		std::lock_guard lck(m_mutex);
		for (const auto& val : m_fibers | std::views::values)
			val->tick();
	}

	std::recursive_mutex m_mutex;
	std::map<char const*, std::unique_ptr<fiber>> m_fibers{};
};
inline dxFiberMgr g_dx_fiber_mgr;