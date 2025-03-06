#pragma once
#include "memory/pointers.h"
#include "pch/pch.h"

template <typename F, typename... Args>
void execute_as_script(rage::scrThread* thread, F&& callback, Args&&... args)
{
	auto tls_ctx = rage::tlsContext::get();
	auto og_thread = tls_ctx->m_script_thread;
	tls_ctx->m_script_thread = thread;
	tls_ctx->m_is_script_thread_active = true;
	std::invoke(std::forward<F>(callback), std::forward<Args>(args)...);
	tls_ctx->m_script_thread = og_thread;
	tls_ctx->m_is_script_thread_active = og_thread != nullptr;
}

template <typename F, typename... Args>
void execute_as_script(uint32_t script_hash, F&& callback, Args&&... args)
{
	for (auto thread : *pointers::g_gtaThreads)
	{
		if (!thread || !thread->m_serialised.m_thread_id || thread->m_serialised.m_script_hash != script_hash)
			continue;

		execute_as_script(thread, callback, args...);
		return;
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
