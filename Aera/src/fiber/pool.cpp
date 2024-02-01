#include "pool.h"

void pool::create()
{
	g_manager.add("FiberPool", m_numberOfFibers, &routine);
}

void pool::add(std::function<void()> func)
{
	if (func)
	{
		std::lock_guard lck(m_mutex);
		m_jobs.push(std::move(func));
	}
}

void pool::tick()
{
	std::unique_lock lck(m_mutex);
	if (!m_jobs.empty())
	{
		auto job = m_jobs.top();
		if (job)
		{
			m_jobs.pop();
			lck.unlock();
			job();
		}
	}
}

void pool::routine()
{
	while (true)
	{
		g_pool.tick();
		fiber::current()->sleep();
	}
}
