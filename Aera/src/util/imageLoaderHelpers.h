#pragma once
#include <pch/pch.h>
#include <unordered_set>
#include <wrl/client.h>
#include "imgui/stb_image.h"

namespace image_loader
{
    using Microsoft::WRL::ComPtr;

    struct vec2 { int x{}, y{}; };

    struct img
    {
        std::unique_ptr<unsigned char, void(*)(void*)> data{nullptr, stbi_image_free};
        vec2 size{};
    };

    struct img_data
    {
        std::unique_ptr<unsigned char, void(*)(void*)> img_bytes{nullptr, free};
        int img_len{};
        int frame_delay{};
    };

    struct frame_data
    {
        int frame_delay{};
        ComPtr<ID3D11ShaderResourceView> res_view;
    };

    ComPtr<ID3D11ShaderResourceView> create_texture(const fs::path& file_path);
    std::vector<frame_data> create_gif_texture(const fs::path& path);
    ComPtr<ID3D11ShaderResourceView> create_resource_view(unsigned char* img_data, vec2 img_size);

    inline int m_header_frame = 0;
    inline int last_frame_count = 0;
    inline std::vector<frame_data> m_header;

    inline bool texture_loading_in_progress = false;
    inline bool m_has_header_loaded = false;

    void flip_through_frames(int delay);
    void header_handler();

    inline const std::unordered_set<std::string_view> supported_image_formats = {
        ".png", ".jpeg", ".bmp", ".psd", ".tga", ".gif", ".hdr", ".pic", ".ppm", ".pgm"
    };

    inline bool contain_pict_ext(const fs::path& path)
    {
        return supported_image_formats.contains(path.extension().string());
    }
}
