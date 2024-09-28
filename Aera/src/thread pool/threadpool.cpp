#include "threadpool.h"
#include <core/logger.h>

thread_pool::thread_pool(const std::size_t pre_allocated_thread_count) :
	m_accept_jobs_(true),
	m_allocated_thread_count_(pre_allocated_thread_count),
	m_busy_threads_(0)
{
	rescale_thread_pool();

	g_thread_pool = this;
}

thread_pool::~thread_pool()
{
	g_thread_pool = nullptr;
}

void thread_pool::rescale_thread_pool()
{
	//std::cout << "Resizing thread pool from " << m_thread_pool_.size() << " to " << m_allocated_thread_count_ << std::endl;

	m_thread_pool_.reserve(m_allocated_thread_count_);

	if (m_thread_pool_.size() < m_allocated_thread_count_)
	{
		for (auto i = m_thread_pool_.size(); i < m_allocated_thread_count_; i++)
			m_thread_pool_.emplace_back(&thread_pool::run, this);
	}
}

void thread_pool::destroy()
{
	{
		std::unique_lock lock(m_lock_);
		m_accept_jobs_ = false;
	}
	m_data_condition_.notify_all();

	for (auto& thread : m_thread_pool_)
		thread.join();

	m_thread_pool_.clear();
}

void thread_pool::push(const std::function<void()>& func, const std::source_location& location)
{
	if (func)
	{
		{
			std::unique_lock lock(m_lock_);
			m_job_stack_.push({func, location});

			if (m_allocated_thread_count_ - m_busy_threads_ < m_job_stack_.size())
			{
				LOG(Warn, "Thread pool potentially starved, resizing to accommodate for load.")

				if (m_allocated_thread_count_ >= max_pool_size)
				{
					LOG(Warn,
					    "The thread pool limit has been reached, whatever you did this should not occur in production.")
				}
				if (m_accept_jobs_ && m_allocated_thread_count_ + 1 <= max_pool_size)
				{
					++m_allocated_thread_count_;
					rescale_thread_pool();
				}
			}
		}
		m_data_condition_.notify_all();
	}
}

void thread_pool::run()
{
	for (;;)
	{
		std::unique_lock lock(m_lock_);

		m_data_condition_.wait(lock, [this]()
		{
			return !m_job_stack_.empty() || !m_accept_jobs_;
		});

		if (!m_accept_jobs_)
			break;
		if (m_job_stack_.empty())
			continue;

		auto [m_func, m_source_location] = m_job_stack_.top();
		m_job_stack_.pop();
		lock.unlock();

		++m_busy_threads_;

		const auto source_file = std::filesystem::path(m_source_location.file_name()).filename().string();

		std::cout << "Thread " << std::this_thread::get_id() << " executing: " << source_file << ":" <<
			m_source_location.line() << '\n';

		std::invoke(m_func);

		--m_busy_threads_;
	}

	std::cout << "Thread " << std::this_thread::get_id() << " exiting" << '\n';
}
