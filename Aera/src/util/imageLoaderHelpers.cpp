#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "imageLoaderHelpers.h"
#include "imgui/imgui.h"
#include <imgui/stb_image_write.h>
#include "gui/gui.h"
#include "gui/interface.h"
#include "imgui/stb_image.h"
#include "renderer/renderer.h"

IMGUI_IMPL_API

namespace image_loader
{
	img_data write_png_to_mem(int x, int y, int comp, const void* data, int stride_bytes, const int delay)
	{
		int len;
		unsigned char* png = stbi_write_png_to_mem(static_cast<const unsigned char*>(data), stride_bytes, x, y, comp,
		                                           &len);
		if (!png)
			return {};
		return {png, len, delay};
	}

	img load_img_from_mem(const img_data& data)
	{
		int width, height;
		const auto img = stbi_load_from_memory(data.img_bytes, data.img_len, &width, &height, nullptr, 0);
		if (img == nullptr)
		{
			return {};
		}
		return {img, {static_cast<int>(std::round(width)), static_cast<int>(std::round(height))}};
	}

	std::map<int, img_data> load_gif(const fs::path& path)
	{
		std::map<int, img_data> frames;
		std::ifstream file(path, std::ios::binary);
		if (!file)
		{
			return frames;
		}
		file.seekg(0, std::ios::end);
		const std::streampos size = file.tellg();
		file.seekg(0, std::ios::beg);
		std::vector<char> buffer(size);
		if (!file.read(buffer.data(), size))
		{
			return frames;
		}
		int* delays = nullptr;
		int width, height, frame_count, comp;
		const auto data = stbi_load_gif_from_memory(reinterpret_cast<stbi_uc*>(buffer.data()), (int)size, &delays,
		                                            &width,
		                                            &height, &frame_count, &comp, 0);
		if (!data)
		{
			return frames;
		}
		const size_t bytes = static_cast<size_t>(width) * comp;
		for (auto i = 0; i < frame_count; i++)
		{
			frames.insert({i, write_png_to_mem(width, height, comp, data + bytes * height * i, 0, delays[i])});
		}
		stbi_image_free(data);
		return frames;
	}

	ID3D11ShaderResourceView* create_texture(const fs::path& file_path)
	{
		vec2 v2{};
		unsigned char* img_data = stbi_load(file_path.string().c_str(), &v2.x, &v2.y, nullptr, 4);
		if (img_data == nullptr)
		{
			return nullptr;
		}
		return create_resource_view(img_data, v2);
	}

	std::map<int, frame_data> create_gif_texture(const fs::path& path)
	{
		const auto gif_data = load_gif(path);
		std::map<int, frame_data> tmp_arr;
		for (const auto& [fst, snd] : gif_data)
		{
			const auto [data, v2] = load_img_from_mem(snd);
			tmp_arr.try_emplace(fst, snd.frame_delay, create_resource_view(data, v2));
		}
		return tmp_arr;
	}

	ID3D11ShaderResourceView* create_resource_view(unsigned char* img_data, const vec2 img_size)
	{
		if (!img_data) return nullptr;
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = img_size.x;
		desc.Height = img_size.y;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		ID3D11Texture2D* p_texture = nullptr;
		const D3D11_SUBRESOURCE_DATA sub_resource = {img_data, desc.Width * 4, 0};
		g_renderer->m_device->CreateTexture2D(&desc, &sub_resource, &p_texture);
		D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
		srv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srv_desc.Texture2D.MipLevels = desc.MipLevels;
		ID3D11ShaderResourceView* tmp_res = nullptr;
		g_renderer->m_device->CreateShaderResourceView(p_texture, &srv_desc, &tmp_res);
		p_texture->Release();
		return tmp_res;
	}

	void flip_through_frames(const int delay)
	{
		static ui::simpleTimer tick(0ms);
		tick.set_delay(delay);
		if (tick.update() && last_frame_count != ImGui::GetFrameCount())
		{
			last_frame_count = ImGui::GetFrameCount();
			if (m_header.size() > 1)
			{
				m_header_frame = static_cast<int>((m_header_frame + 1) % m_header.size());
			}
		}
	}

	void header_handler()
	{
		if (ui::g_open && m_has_header_loaded)
		{
			flip_through_frames(m_header[m_header_frame].frame_delay);
			ui::drawing::image(m_header[m_header_frame].res_view,
			                   {ui::g_pos.x, ui::g_pos.y + ui::g_header.m_size / 2.f},
			                   {ui::g_width, ui::g_header.m_size}, ui::g_options.m_textColor);
		}
	}
}
