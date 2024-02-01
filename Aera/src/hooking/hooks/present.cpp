#include "hooking/hooking.h"
#include "renderer/renderer.h"

HRESULT hooks::present(IDXGISwapChain* swap_chain, UINT sync_interval, UINT flags)
{
	if (g_running && (flags & static_cast<UINT>(DXGI_PRESENT_TEST)) != static_cast<UINT>(DXGI_PRESENT_TEST))
	{
		ImGui_ImplWin32_NewFrame();
		ImGui_ImplDX11_NewFrame();
		ImGui::NewFrame();
		g_renderer->on_present();
		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
	return g_hooking->m_DX.get_original<decltype(&present)>(g_swapchain_present_index)(swap_chain, sync_interval, flags);
}
