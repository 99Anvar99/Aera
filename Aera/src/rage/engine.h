#pragma once
#include "classes.h"
#include "joaat.h"
#include "fiber/threads.h"
#include "memory/pointers.h"
#include "core/logger.h"
namespace engine
{
	inline u32 g_frame_count{};

	class thread : public GtaThread
	{
	public:
		void init()
		{
			return pointers::g_scrThreadInit(this);
		}

		virtual void do_run() = 0;

		rage::eThreadState reset(uint32_t script_hash, void* args, uint32_t arg_count) override
		{
			RtlZeroMemory(&m_serialised, sizeof(m_serialised));
			m_serialised.m_state = rage::eThreadState::blocked;
			m_serialised.m_script_hash = script_hash;
			m_serialised.m_min_pc = -1;
			m_serialised.m_max_pc = -1;
			m_serialised.m_priority = 1;
			init();
			m_exit_message = "Normal exit";
			return m_serialised.m_state;
		}

		rage::eThreadState run() override
		{
			auto tls{rage::tlsContext::get()};
			if (!m_handler)
			{
				pointers::g_scriptHandlerMgr->attach_thread(this);
				m_safe_for_network_game = true;
			}
			auto og_thr = tls->m_script_thread;
			tls->m_script_thread = this;
			if (m_serialised.m_state != rage::eThreadState::halted && g_frame_count != *pointers::g_frame_count)
			{
				if (call_thread_cache_inside_a_non_script_hook) {
					cache_thread_handles();
					call_thread_cache_inside_a_non_script_hook = false;
				}	
				do_run();
				g_frame_count = *pointers::g_frame_count;
			}
			tls->m_script_thread = og_thr;
			return m_serialised.m_state;
		}

		rage::eThreadState tick(uint32_t ops_to_execute) override
		{
			return pointers::g_scrThreadTick(this, ops_to_execute);
		}

		void kill() override
		{
			return pointers::g_scrThreadKill(this);
		}
	};

	template <typename T>
	using pgCollection = rage::atArray<T>;
	template <typename T>
	using pgPtrCollection = rage::atArray<T*>;
	inline std::vector<std::pair<thread*, thread*>> g_owned_threads;

	inline pgPtrCollection<thread>* get_thread_collection()
	{
		return reinterpret_cast<pgPtrCollection<thread>*>(pointers::g_gtaThreads);
	}

	inline uint32_t& get_thread_id()
	{
		return *pointers::g_threadId;
	}

	inline uint32_t& get_thread_count()
	{
		return *pointers::g_threadCount;
	}

	inline void create_thread(thread* thread)
	{
		auto& collection{*get_thread_collection()};
		auto& id{get_thread_id()};
		auto& count{get_thread_count()};
		int slot{};
		//Get slot
		for (const auto& thr : collection)
		{
			if (auto& [m_thread_id, m_script_hash, m_state, m_pointer_count, m_frame_pointer, m_stack_pointer, m_timer_a
					, m_timer_b, m_wait, m_min_pc, m_max_pc, m_tls, m_stack_size, m_catch_pointer_count,
					m_catch_frame_pointer, m_catch_stack_pointer, m_priority, m_call_depth, unk_0061, unk_0062,
					m_callstack]
				{thr->m_serialised}; !m_thread_id)
				break;
			slot++;
		}
		if (slot != collection.size())
		{
			const std::string& name{std::format("thr_{}", count + 1)};
			auto& [m_thread_id, m_script_hash, m_state, m_pointer_count, m_frame_pointer, m_stack_pointer, m_timer_a,
				m_timer_b, m_wait, m_min_pc, m_max_pc, m_tls, m_stack_size, m_catch_pointer_count, m_catch_frame_pointer
				, m_catch_stack_pointer, m_priority, m_call_depth, unk_0061, unk_0062, m_callstack]{
				thread->m_serialised
			};
			strcpy_s(thread->m_name, name.c_str());
			thread->m_script_hash = rage::joaat(thread->m_name);
			thread->reset(thread->m_script_hash, nullptr, 0);
			thread->m_can_be_paused = false;
			if (!id)
			{
				id++;
			}
			m_thread_id = id;
			count++;
			id++;
			auto og{collection[static_cast<u16>(slot)]};
			collection[static_cast<u16>(slot)] = thread;
			g_owned_threads.emplace_back(thread, og);
			LOG(Info, "Successfully created thread '{}' at id {}", thread->m_name, m_thread_id)
		}
	}

	inline void cleanup_threads()
	{
		if (g_owned_threads.empty())
		{
			return;
		}
		std::ranges::reverse(g_owned_threads);
		auto& collection{*get_thread_collection()};
		int slot{};
		for (const auto& thr : collection)
		{
			for (auto& [ownedThr, ogThr] : g_owned_threads)
			{
				if (thr == ownedThr)
				{
					collection[static_cast<u16>(slot)] = ogThr;
				}
			}
			slot++;
		}
		g_owned_threads.clear();
	}
}
