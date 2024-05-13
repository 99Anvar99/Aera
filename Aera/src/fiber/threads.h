#pragma once
#include "memory/pointers.h"
#include "pch/pch.h"

inline rage::scrThread** g_cached_scr_thread{};
inline rage::sysMemAllocator** g_cached_scr_allocator{};
inline rage::sysMemAllocator** g_cached_tls_entry{};
inline bool call_thread_cache_inside_a_non_script_hook{};

inline void cache_thread_handles()
{
	g_cached_scr_allocator = rage::sysMemAllocator::getPointer();
	g_cached_tls_entry = rage::sysMemAllocator::getEntryPointer();
	g_cached_scr_thread = rage::scrThread::getPointer();
}

template <typename Call, typename... Arguments>
	requires is_invokable_with_args<Call, Arguments...>
void execute_under_hybrid_thr(Call&& callback, Arguments&&... args)
{
	if (!g_cached_scr_allocator || !g_cached_tls_entry || !g_cached_scr_thread)
	{
		if (!call_thread_cache_inside_a_non_script_hook)
			call_thread_cache_inside_a_non_script_hook = true;
		return;
	}
	static auto tls_ctx = rage::tlsContext::get();
	if (g_cached_scr_allocator != nullptr)
	{
		auto og_thr = rage::scrThread::get();
		auto og_alloc = rage::sysMemAllocator::get();
		auto og_a_tls_entry = rage::sysMemAllocator::getEntry();
		tls_ctx->m_allocator2 = *g_cached_tls_entry;
		tls_ctx->m_script_thread = *g_cached_scr_thread;
		tls_ctx->m_is_script_thread_active = true;
		tls_ctx->m_allocator = *g_cached_scr_allocator;
		std::invoke(std::forward<Call>(callback), std::forward<Arguments>(args)...);
		tls_ctx->m_allocator2 = og_a_tls_entry;
		tls_ctx->m_script_thread = og_thr;
		tls_ctx->m_is_script_thread_active = og_thr != nullptr;
		tls_ctx->m_allocator = og_alloc;
	}
}

inline auto get_thr = [=](const char* name) -> GtaThread* {
	for (auto& thr : *pointers::g_gtaThreads)
	{
		if (thr->m_name == name)
			return thr;
	}
	return nullptr;
};
inline auto get_thr_using_hash = [=](uint32_t hash) -> GtaThread* {
	for (auto& thr : *pointers::g_gtaThreads)
	{
		if (thr && thr->m_instance_id && thr->m_handler && thr->m_script_hash == hash) 
			return thr;
	}
	return nullptr;
};
