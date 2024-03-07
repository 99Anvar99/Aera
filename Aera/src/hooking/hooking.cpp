#include "hooking.h"
#include "memory/pointers.h"

hooking::hooking() :
	m_minhook(),
	m_DX(VFT({*pointers::g_swapChain, g_swapchain_num_funcs}), VFT({ VFTFunctionData(&hooks::present, g_swapchain_present_index), VFTFunctionData(&hooks::resize_buffers, g_swapchain_resizebuffers_index) })),
	m_cTaskJumpConstructor("CTJC", pointers::g_cTaskJumpConstructor, &hooks::c_task_jump_constructor),
	m_cTaskFallConstructor("CTFC", pointers::g_cTaskFallConstructor, &hooks::c_task_fall_constructor),
	m_queue_dependency("QD", pointers::g_queueDependency, &hooks::queue_dependency),
	m_http_start_request("HSR", pointers::g_httpStartRequest, &hooks::http_start_request),
	m_convertThreadToFiber("CTTF", *dynamic_function_parser{"kernel32.dll", "ConvertThreadToFiber"}, &hooks::convert_thread_to_fiber)
{
	g_hooking = this;
}

hooking::~hooking()
{
	m_DX.unhook(g_swapchain_present_index);
	m_DX.unhook(g_swapchain_resizebuffers_index);
	g_hooking = nullptr;
}

void hooking::enable()
{
	m_DX.enable();
	detour::enable_all();
	MH_ApplyQueued();
}

void hooking::disable()
{
	//Sometimes the pointer is de-allocated, but disabled wasn't called.
	// We'll just do a MinHook call and have the OS cleanup our mess
	if (this)
	{
		m_DX.disable();
	}
	detour::disable_all();
	MH_ApplyQueued();
	MH_Uninitialize();
}
