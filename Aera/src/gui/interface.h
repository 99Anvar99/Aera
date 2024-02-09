#pragma once
#include "pch/pch.h"
#include "rage/classes.h"
#include "gui/types/submenu.h"
#include "gui/types.h"
#include "gui/menu_handler.h"

namespace helpers
{
	inline float degrees_to_radians(float degrees)
	{
		//Formula: x * PI/180 = y rad
		return static_cast<float>(static_cast<long double>(degrees) * pi / 180.L);
	}

	namespace rotation
	{
		inline i32 g_index{};

		inline void rotate_start(ImDrawList* draw_list)
		{
			g_index = draw_list->VtxBuffer.Size;
		}

		inline ImVec2 rotation_center(ImDrawList* draw_list)
		{
			ImVec2 l{FLT_MAX, FLT_MAX}, u{-FLT_MAX, -FLT_MAX};
			const auto& buf = draw_list->VtxBuffer;
			for (i32 i{g_index}; i != buf.Size; ++i)
			{
				l = ImMin(l, buf[i].pos);
				u = ImMax(u, buf[i].pos);
			}
			return (l + u) / 2.f;
		}

		inline void rotate_end(ImDrawList* draw_list, float rad)
		{
			ImVec2 center{rotation_center(draw_list)};
			float s{std::sin(rad)}, c{std::cos(rad)};
			center = ImRotate(center, s, c) - center;
			auto& buf{draw_list->VtxBuffer};
			for (int i{g_index}; i != buf.Size; ++i)
			{
				buf[i].pos = ImRotate(buf[i].pos, s, c) - center;
			}
		}
	}
}

namespace ui
{
	using Font = ImFont*;
	using ShaderResource = ID3D11ShaderResourceView*;
	inline std::vector<std::string> g_dictonary_requests{};

	inline void add_dictionary_request(const std::string& dictionary)
	{
		for (auto& request : g_dictonary_requests)
		{
			if (request == dictionary)
			{
				return;
			}
		}
		g_dictonary_requests.push_back(dictionary);
	}

	inline bool g_open{true};
	inline size_t m_openDelay = 200; // Open / Close Delay
	inline size_t m_backDelay = 200; // Back Delay
	inline size_t m_enterDelay = 200; // Enter Delay
	inline size_t m_verticalDelay = 100; // Left / Right Delay
	inline size_t m_horizontalDelay = 100; // Up / Down Delay

	class simpleTimer
	{
	public:
		explicit simpleTimer(milliseconds delay) :
			m_Timer(high_resolution_clock::now()),
			m_Delay(std::chrono::duration_cast<high_resolution_clock::duration>(delay))
		{
		}

		bool update()
		{
			if (const auto now = high_resolution_clock::now(); (now.time_since_epoch() - m_Timer.time_since_epoch()).
				count() >= m_Delay.count())
			{
				m_Timer = now;
				return true;
			}

			return false;
		}

		void set_delay(int delay)
		{
			m_Delay = milliseconds(delay);
		}

		void set_delay(milliseconds delay)
		{
			m_Delay = delay;
		}

	private:
		high_resolution_clock::time_point m_Timer;
		high_resolution_clock::duration m_Delay;
	};

	//Key Handlers
	// Keypress Handlers
	inline bool m_open_key_pressed = false;
	inline bool m_back_key_pressed = false;
	inline bool m_enter_key_pressed = false;
	inline bool m_up_key_pressed = false;
	inline bool m_down_key_pressed = false;
	inline bool m_left_key_pressed = false;
	inline bool m_right_key_pressed = false;

	inline bool g_push_menu{true};
	inline std::stack<submenu> g_menus{};
	inline submenu g_menu{};

	namespace menu
	{
#define CURRENT_MENU g_menus.top()
#define OPTIONS CURRENT_MENU.m_options
#define OPTION_INDEX CURRENT_MENU.m_current
#define OPTION_COUNT OPTIONS.size()
#define VALID_OPTION OPTION_COUNT && OPTIONS[OPTION_INDEX]
#define CURRENT_OPTION static_cast<abstractOption&>(*OPTIONS[OPTION_INDEX])
#define IS_STACK_VALID !g_menus.empty() && g_menus.size() > NULL
		extern void push(submenu* sub);
		extern void pop();
	}

	namespace drawing
	{
		inline ImGuiIO* g_imgui_io{};
		inline ImDrawList* g_foreground_drawlist{};
		inline ImDrawList* g_background_drawlist{};
		inline ImVec2 g_resolution{};
		extern void init();
		extern void tick();
		extern const ImVec2& convert_coord_types(const ImVec2& pos, bool is_dc = false);
		extern const ImVec2& get_text_size(i32 font_id, float size, const std::string& text, float wrap = -1.f);
		extern i32 get_line_count(const std::string& text, i32 font_id, float size, float wrap);
		extern float get_text_height(i32 font_id, float size, float wrap = -1.f);
		extern const ImVec2& get_sprite_scale(float size);
		extern rage::grcTexture* get_game_texture(const sprite& sprite);
		extern Font get_font(i32 id);
		extern void rectangle(const ImVec2& pos, const ImVec2& size, color color, bool background = false,
		                      float rounding = 0.f, ImDrawFlags flags = 0.f);
		extern void text(i32 font_id, const std::string& text, ImVec2 pos, float size, color color, eJustify justify,
		                 float padding, float wrap);
		extern void rotated_image(ShaderResource resource, const ImVec2& pos, const ImVec2& size, color color,
		                          float angle);
		extern void image(ShaderResource resource, const ImVec2& pos, const ImVec2& size, const color& color,
		                  float rotation = 0.f);
		extern void image(const sprite& sprite, const ImVec2& pos, const ImVec2& size, const color& color);
	}

	namespace handlers
	{
		namespace sounds
		{
			inline soundData g_open{true, "SELECT"};
			inline soundData g_close{true, "BACK"};
			inline soundData g_enter{true, "SELECT"};
			inline soundData g_back{true, "BACK"};
			inline soundData g_left{true, "NAV_LEFT_RIGHT"};
			inline soundData g_right{true, "NAV_LEFT_RIGHT"};
			inline soundData g_up{true, "NAV_UP_DOWN"};
			inline soundData g_down{true, "NAV_UP_DOWN"};
			extern void queue(const soundData& sound);
		}

		namespace key
		{
			inline void check_if_pressed(bool& value, int padIdx, int key, int keyboardKey, size_t delay);
			inline void check_if_pressed(bool& value, int padIdx, int key, int key2, int keyboardKey, size_t delay);
			extern void reset_input_handler();
			extern void input_check_handler();
			extern void actions();
		}
	}
}
