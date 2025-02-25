#include "renderer.h"
#include "commands/features.h"
#include "gui/gui.h"
#include <util/imageLoaderHelpers.h>

// Create font using existing font path
ImFont* create_im_font(const fs::path& path, float size, const ImFontConfig* config, const ImWchar* glyph_ranges = nullptr)
{
	return ImGui::GetIO().Fonts->AddFontFromFileTTF(path.string().c_str(), size, config, glyph_ranges);
}

// Crate font from loading in memory
ImFont* create_im_font(void* font_data, int font_size, float size, const ImFontConfig* config, const ImWchar* glyph_ranges = nullptr)
{
	return ImGui::GetIO().Fonts->AddFontFromMemoryTTF(font_data, font_size, size, config, glyph_ranges);
}

renderer::renderer() : m_swapchain(*pointers::g_swapChain)
{
	m_wndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrA(pointers::g_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&renderer::wndProc)));
	if (FAILED(m_swapchain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(m_device.GetAddressOf()))))
		throw std::runtime_error("Failed to get the D3D device!");
	m_device->GetImmediateContext(m_context.GetAddressOf());
	const auto ctx = ImGui::CreateContext();
	ctx->IO.IniFilename = nullptr;
	ctx->IO.LogFilename = nullptr;
	ImGui_ImplDX11_Init(m_device.Get(), m_context.Get());
	ImGui_ImplWin32_Init(pointers::g_hwnd);
	ImGui::StyleColorsClassic();
	ui::drawing::init();
	const auto win_fonts = std::filesystem::path(std::getenv("SYSTEMROOT")) / "Fonts";
	const auto atlas{ui::drawing::g_imgui_io->Fonts};
	RECT handle{};
	if (GetWindowRect(FindWindowA("grcWindow", nullptr), &handle))
	{
		m_window.get(handle);
	}
	const float global_size{55.f / 1920.f * m_window.size.x};
	m_fontCfg.FontDataOwnedByAtlas = false;

	// ## You can add fonts to support different languages
	// Example: support for Cyrillic
	// io.Fonts->GetGlyphRangesCyrillic() || after &m_fontCfg
	// m_arial = create_im_font(win_fonts / "arial.ttf", global_size, &m_fontCfg, atlas->GetGlyphRangesCyrillic());
	// More can be found in ImGui_draw.cpp or https://github.com/ocornut/imgui/blob/master/imgui_draw.cpp

	m_arial = create_im_font(win_fonts / "arial.ttf", global_size, &m_fontCfg);
	m_arialBold = create_im_font(win_fonts / "arialbd.ttf", global_size, &m_fontCfg);
	m_arialBoldItalic = create_im_font(win_fonts / "arialbi.ttf", global_size, &m_fontCfg);
	m_arialBlack = create_im_font(win_fonts / "ariblk.ttf", global_size, &m_fontCfg);
	m_arialItalic = create_im_font(win_fonts / "ariali.ttf", global_size, &m_fontCfg);
	m_tahoma = create_im_font(win_fonts / "tahoma.ttf", global_size, &m_fontCfg);
	m_impact = create_im_font(win_fonts / "impact.ttf", global_size, &m_fontCfg);

	ui::drawing::g_imgui_io->FontAllowUserScaling = true;
	g_renderer = this;
}

renderer::~renderer()
{
	g_renderer = nullptr;
	SetWindowLongPtrA(pointers::g_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_wndProc));
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void renderer::on_present()
{
	ui::drawing::tick();
	image_loader::header_handler();
	ui::draw();
}

LRESULT renderer::wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (g_renderer)
	{
		if (ImGui::GetCurrentContext())
		{
			ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		}
		return CallWindowProcA(g_renderer->m_wndProc, hWnd, uMsg, wParam, lParam);
	}
	return S_FALSE;
}
