#pragma once
#include "pch/pch.h"
#include "rage/commands/list.h"

struct Rect
{
	float left, right{};
	float top, bottom{};
	ImVec2 size{};

	void get(const RECT& handle)
	{
		left = static_cast<float>(handle.left);
		right = static_cast<float>(handle.right);
		top = static_cast<float>(handle.top);
		bottom = static_cast<float>(handle.bottom);
		size.x = right - left;
		size.y = bottom - top;
	}
};

class renderer
{
public:
	renderer();
	~renderer();

	void on_present();

	static LRESULT wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	ImFontConfig m_fontCfg{};
	ImFont* m_arial{};
	ImFont* m_arialBold{};
	ImFont* m_arialItalic{};
	ImFont* m_arialBoldItalic{};
	ImFont* m_arialBlack{};
	ImFont* m_tahoma{};
	ImFont* m_impact{};

	Rect m_window{};
	WNDPROC m_wndProc{};
	comPtr<IDXGISwapChain> m_swapchain{};
	comPtr<ID3D11Device> m_device{};
	comPtr<ID3D11DeviceContext> m_context{};
};

inline renderer* g_renderer{};
