#pragma once
#include "pch/pch.h"
#include "methods/vmt.h"
#include "methods/detour.h"

static constexpr auto g_swapchain_num_funcs = 19;
static constexpr auto g_swapchain_present_index = 8;
static constexpr auto g_swapchain_resizebuffers_index = 13;

struct hooks
{
	static void* c_task_jump_constructor(u64 a1, u32 flags);
	static void* c_task_fall_constructor(u64 a1, u32 flags);
	static int queue_dependency(void* a1, int a2, int64_t dependency);
	static bool http_start_request(void* _this, const char* uri);
	static LPVOID convert_thread_to_fiber(LPVOID param);

	static HRESULT resize_buffers(IDXGISwapChain* swap_chain, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swap_chain_flags);
	static HRESULT present(IDXGISwapChain* swap_chain, UINT sync_interval, UINT flags);
};

class dynamic_function_parser
{
public:
	dynamic_function_parser(LPCSTR moduleName, LPCSTR exportName) :
		m_moduleName(moduleName), m_exportName(exportName)
	{
		if (!m_module)
		{
			m_module = GetModuleHandleA(m_moduleName);
		}
	}

	FARPROC operator*()
	{
		if (m_module)
			return GetProcAddress(m_module, m_exportName);
		return nullptr;
	}

private:
	LPCSTR m_moduleName{};
	LPCSTR m_exportName{};
	HMODULE m_module{};
};

class hooking
{
	friend struct hooks;

public:
	hooking();
	~hooking();

	void enable();
	void disable();

	MinHook m_minhook;
	hookVFT m_DX;
	detour m_cTaskJumpConstructor;
	detour m_cTaskFallConstructor;
	detour m_queue_dependency;
	detour m_http_start_request;
	detour m_convertThreadToFiber;
};

inline hooking* g_hooking{};
