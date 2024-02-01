#pragma once
#include <pch/pch.h>
#include <source_location>

// if this limit is hit you did something wrong coding wise.
constexpr auto max_pool_size = 32u;

struct thread_pool_job
{
	std::function<void()> m_func;
	std::source_location m_source_location;
};

class thread_pool
{
	std::atomic<bool> m_accept_jobs_;
	std::condition_variable m_data_condition_;

	std::stack<thread_pool_job> m_job_stack_;
	std::mutex m_lock_;
	std::vector<std::thread> m_thread_pool_;

	// the amount of threads active in the pool
	std::atomic<size_t> m_allocated_thread_count_;
	// the amount of threads currently on a job
	std::atomic<size_t> m_busy_threads_;

public:
	// YimMenu only has 2 blocking threads, 4 should be sufficient but the pool should automatically allocate more if needed
	explicit thread_pool(std::size_t pre_allocated_thread_count = 4);
	~thread_pool();

	void destroy();
	void push(const std::function<void()>& func,
	          const std::source_location& location = std::source_location::current());

	std::pair<size_t, size_t> usage() const
	{
		return {m_busy_threads_, m_allocated_thread_count_};
	}

private:
	void run();
	void rescale_thread_pool();
};

inline thread_pool* g_thread_pool{};
