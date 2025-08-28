#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "imageLoaderHelpers.h"
#include "imgui/imgui.h"
#include <imgui/stb_image_write.h>
#include "gui/gui.h"
#include "gui/interface.h"
#include "renderer/renderer.h"

namespace image_loader
{
    img_data write_png_to_mem(int x, int y, int comp, const void* data, int stride_bytes, int delay)
    {
        int len{};
        unsigned char* png = stbi_write_png_to_mem(static_cast<const unsigned char*>(data), stride_bytes, x, y, comp, &len);
        return {.img_bytes = {png, free}, .img_len = len, .frame_delay = delay };
    }

    img load_img_from_mem(const img_data& data)
    {
        int width{}, height{};
        unsigned char* raw = stbi_load_from_memory(data.img_bytes.get(), data.img_len, &width, &height, nullptr, 0);
        if (!raw) return {};
        return {.data = {raw, stbi_image_free}, .size = {.x = width, .y = height} };
    }

    std::vector<img_data> load_gif(const fs::path& path)
    {
        std::ifstream file(path, std::ios::binary);
        if (!file) return {};

        file.seekg(0, std::ios::end);
        auto size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<char> buffer(size);
        if (!file.read(buffer.data(), size)) return {};

        int* delays = nullptr;
        int width{}, height{}, frame_count{}, comp{};
        stbi_uc* data = stbi_load_gif_from_memory(reinterpret_cast<stbi_uc*>(buffer.data()), size, &delays, &width, &height, &frame_count, &comp, 0);

        if (!data) return {};

        std::vector<img_data> frames;
        frames.reserve(frame_count);

        size_t bytes_per_frame = static_cast<size_t>(width) * comp * height;
        for (int i = 0; i < frame_count; i++)
        {
            frames.push_back(write_png_to_mem(width, height, comp, data + bytes_per_frame * i, 0, delays[i]));
        }
        stbi_image_free(data);
        STBIW_FREE(delays);
        return frames;
    }

    ComPtr<ID3D11ShaderResourceView> create_texture(const fs::path& file_path)
    {
        vec2 size{};
        unsigned char* raw = stbi_load(file_path.string().c_str(), &size.x, &size.y, nullptr, 4);
        if (!raw) return nullptr;

        auto res = create_resource_view(raw, size);
        stbi_image_free(raw);
        return res;
    }

    std::vector<frame_data> create_gif_texture(const fs::path& path)
    {
        auto gif_data = load_gif(path);
        std::vector<frame_data> frames;
        frames.reserve(gif_data.size());

        for (auto& data : gif_data)
        {
            auto img = load_img_from_mem(data);
            frames.push_back({.frame_delay = data.frame_delay, .res_view = create_resource_view(img.data.get(), img.size)});
        }
        return frames;
    }

    ComPtr<ID3D11ShaderResourceView> create_resource_view(unsigned char* img_data, vec2 img_size)
    {
        if (!img_data) return nullptr;

        D3D11_TEXTURE2D_DESC desc{};
        desc.Width = img_size.x;
        desc.Height = img_size.y;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

        D3D11_SUBRESOURCE_DATA sub_resource{ img_data, (desc.Width * 4), 0 };
        ComPtr<ID3D11Texture2D> texture;

        if (FAILED(g_renderer->m_device->CreateTexture2D(&desc, &sub_resource, &texture)))
        {
            return nullptr;
        }

        ComPtr<ID3D11ShaderResourceView> res;
        D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc{};
        srv_desc.Format = desc.Format;
        srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srv_desc.Texture2D.MipLevels = desc.MipLevels;

        if (FAILED(g_renderer->m_device->CreateShaderResourceView(texture.Get(), &srv_desc, &res)))
        {
            return nullptr;
        }

        return res;
    }

    void flip_through_frames(int delay)
    {
        static ui::simpleTimer tick(0ms);
        tick.set_delay(delay);

        if (tick.update() && last_frame_count != ImGui::GetFrameCount())
        {
            last_frame_count = ImGui::GetFrameCount();
            if (m_header.size() > 1)
            {
                m_header_frame = (m_header_frame + 1) % m_header.size();
            }
        }
    }

    void header_handler()
    {
        if (ui::g_open && m_has_header_loaded && !m_header.empty())
        {
            flip_through_frames(m_header[m_header_frame].frame_delay);
            ui::drawing::image(m_header[m_header_frame].res_view.Get(), {ui::g_pos.x, ui::g_pos.y + ui::g_header.m_size / 2.f}, {ui::g_width, ui::g_header.m_size}, ui::g_options.m_textColor);
        }
    }
}