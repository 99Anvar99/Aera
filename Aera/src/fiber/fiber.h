#pragma once
#include "pch/pch.h"
#include "core/logger.h"

class fiber
{
public:
	fiber(const fnptr<void()> fn) : m_func(fn)
	{
		m_fibers.first = CreateFiber(0, &fiber::routine, this);
	}

	~fiber()
	{
		if (m_fibers.first)
		{
			DeleteFiber(m_fibers.first);
		}
	}

	void sleep(const std::optional<high_resolution_clock::duration> time = std::nullopt)
	{
		if (time.has_value())
		{
			m_time = high_resolution_clock::now() + time.value();
		}
		else
		{
			m_time = std::nullopt;
		}
		SwitchToFiber(m_fibers.second);
	}

	void tick()
	{
		m_fibers.second = GetCurrentFiber();
		if (!m_time.has_value() || m_time.value() <= high_resolution_clock::now())
		{
			SwitchToFiber(m_fibers.first);
		}
	}

	static void routine(PVOID param)
	{
		const auto f{static_cast<fiber*>(param)};
		
		f->m_func();
		
		while (true)
		{
			f->sleep();
		}
	}

	static fiber* current()
	{
		return static_cast<fiber*>(GetFiberData());
	}

	std::pair<void*, void*> m_fibers{};
	fnptr<void()> m_func{};
	std::optional<high_resolution_clock::time_point> m_time{};
};
