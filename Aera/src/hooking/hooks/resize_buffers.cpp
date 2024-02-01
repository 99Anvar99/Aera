#include "hooking/hooking.h"

HRESULT hooks::resize_buffers(IDXGISwapChain* swap_chain, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swap_chain_flags)
{
	if (g_running)
	{
		ImGui_ImplDX11_InvalidateDeviceObjects();
		const auto result = g_hooking->m_DX.get_original<decltype(&resize_buffers)>(g_swapchain_resizebuffers_index)(swap_chain, buffer_count, width, height, new_format, swap_chain_flags);
		if (SUCCEEDED(result))
		{
			ImGui_ImplDX11_CreateDeviceObjects();
		}
		return result;
	}
	return g_hooking->m_DX.get_original<decltype(&resize_buffers)>(g_swapchain_resizebuffers_index)(swap_chain, buffer_count, width, height, new_format, swap_chain_flags);
}
