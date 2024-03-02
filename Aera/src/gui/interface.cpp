#include "interface.h"
#include "renderer/renderer.h"
#include "fiber/pool.h"

namespace ui
{
	namespace menu
	{
		void push(submenu* sub)
		{
			g_menu = *sub;
			g_push_menu = true;
		}

		void pop()
		{
			if (g_menus.size() > 2)
			{
				return g_menus.pop();
			}
			g_open = false;
		}
	}

	namespace drawing
	{
		void init()
		{
			//Stores all the handlers for ImGui
			g_imgui_io = &ImGui::GetIO();
			tick(); //Do once here as well
			g_resolution = g_imgui_io->DisplaySize;
		}

		void tick()
		{
			//Needs to be looped otherwise it will have a stroke
			g_foreground_drawlist = ImGui::GetForegroundDrawList();
			g_background_drawlist = ImGui::GetBackgroundDrawList();
			//If we change display sizes, it will die.
			if (g_resolution.x != g_imgui_io->DisplaySize.x || g_resolution.y != g_imgui_io->DisplaySize.y)
			{
				g_resolution = g_imgui_io->DisplaySize;
			}
		}

		const ImVec2& convert_coord_types(const ImVec2& pos, bool is_dc)
		{
			//DC is display coords. We use a 0.00 to 1.00 scale which is called NDC (or Non-Display Coords). 
			// This is because NDC scales across all monitors. However, ImGui uses DC. This is a helper function to swap back and forth between the two.
			if (is_dc)
			{
				return pos / g_resolution;
			}
			return pos * g_resolution;
		}

		const ImVec2& get_text_size(i32 font_id, float size, const std::string& text, float wrap)
		{
			//Wrap will always have a value, it is stored as NDC.
			auto font{get_font(font_id)};
			auto scaled_wrap{wrap * g_resolution.x};
			auto text_size{font->CalcTextSizeA(font->FontSize * size, FLT_MAX, scaled_wrap, text.c_str(), nullptr)};
			text_size.x = IM_FLOOR(text_size.x + 0.99999999999f);
			return convert_coord_types(text_size, true);
		}

		i32 get_line_count(const std::string& text, i32 font_id, float size, float wrap)
		{
			auto line_count{
				static_cast<i32>(get_text_size(font_id, size, text, wrap).y / get_text_height(font_id, size, wrap))
			};
			return line_count;
		}

		float get_text_height(i32 font_id, float size, float wrap)
		{
			//Not used commonly, but this is useful for estimating line count, or line size because it gets the text height of a font
			auto text_size{get_text_size(font_id, size, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", wrap)};
			return text_size.y;
		}

		const ImVec2& get_sprite_scale(float size)
		{
			return {(g_resolution.y / g_resolution.x) * size, size};
		}

		rage::grcTexture* get_game_texture(const sprite& sprite)
		{
			//Pulls a grcTexture from the game, holds the texture shader, texture size and other various data. It holds a high-res copy
			return pointers::g_textureStore->FindTexture(sprite.m_dictionary, sprite.m_texture);
		}

		Font get_font(i32 Id)
		{
			switch (Id)
			{
			case 1: return g_renderer->m_arial;
			case 2: return g_renderer->m_arialBold;
			case 3: return g_renderer->m_arialItalic;
			case 4: return g_renderer->m_arialBoldItalic;
			case 5: return g_renderer->m_arialBlack;
			case 6: return g_renderer->m_tahoma;
			default: return g_renderer->m_arial;
			}
		}

		void rectangle(const ImVec2& pos, const ImVec2& size, color color, bool background, float rounding, ImDrawFlags flags)
		{
			auto draw_list{background ? g_background_drawlist : g_foreground_drawlist};
			const auto& scaled_pos{convert_coord_types(pos)};
			const auto& scaled_size{convert_coord_types(size)};
			const auto& final_pos{scaled_pos - (scaled_size / 2.f)};
			//For some reason, DirectX/ImGui has a very weird way of doing rectangles. It uses two triangles and a four-point corner system.
			draw_list->AddRectFilled(final_pos, final_pos + scaled_size, color.pack(), rounding, flags);
		}

		void text(i32 font_id, const std::string& text, ImVec2 pos, float size, color color, eJustify justify,
		          float padding, float wrap)
		{
			auto draw_list{g_foreground_drawlist};
			auto font{get_font(font_id)};
			switch (justify)
			{
			//By default, the alignment is center. This is because it is the least amount of math when compared to everything else.
			//If I did left, right would be "pos.x = (pos.x + ((wrap / padding) * 2.f)) - textSize.x" which is more when compared to how it currently is.
			case eJustify::Left:
				{
					pos.x = pos.x - wrap / padding;
				}
				break;
			case eJustify::Right:
				{
					const auto& text_size{get_text_size(font_id, size, text, wrap)};
					pos.x = pos.x + (wrap / padding);
					pos.x -= text_size.x;
				}
				break;
			case eJustify::Center:
				{
					const auto& text_size{get_text_size(font_id, size, text, wrap)};
					pos.x -= text_size.x / 2.f;
				}
				break;
			}
			auto scaled_pos{convert_coord_types(pos)};
			draw_list->AddText(font, font->FontSize * size, scaled_pos, color.pack(), text.data(), nullptr,
			                   wrap * g_resolution.x);
		}

		void rotated_image(ShaderResource resource, const ImVec2& pos, const ImVec2& size, color color, float angle)
		{
			auto draw_list{g_foreground_drawlist};
			if (angle)
			{
				helpers::rotation::rotate_start(draw_list);
			}
			draw_list->AddImage(resource, pos, size, {}, {1, 1}, color.pack());
			if (angle)
			{
				helpers::rotation::rotate_end(draw_list, helpers::degrees_to_radians(angle));
			}
		}

		void image(ShaderResource resource, const ImVec2& pos, const ImVec2& size, const color& color, float rotation)
		{
			auto& scaled_pos{convert_coord_types(pos)};
			auto& scaled_size{convert_coord_types(size)};
			auto final_pos{scaled_pos - scaled_size / 2.f};
			rotated_image(resource, final_pos, {final_pos + scaled_size}, color, rotation);
		}

		void image(const sprite& sprite, const ImVec2& pos, const ImVec2& size, const color& color)
		{
			if (const auto texture{get_game_texture(sprite)})
			{
				//Bit cursed because it's stored as a vector2, but it is what it is
				auto [x, y]{texture->Size()};
				const ImVec2& ndc_texture_size{convert_coord_types({x, y}, true)};
				image(texture->m_shader, pos, ndc_texture_size * size, color, sprite.m_rotation);
			}
		}
	}

	//I feel these are explained by reading. If you can't figure it out, you shouldn't be using this project
	namespace handlers
	{
		namespace sounds
		{
			void queue(const soundData& sound)
			{
				if (sound.m_active)
				{
					g_pool.add([&]
					{
						AUDIO::PLAY_SOUND_FRONTEND(-1, sound.m_id.data(), sound.m_set.data(), NULL);
					});
				}
			}
		}

		namespace key
		{
			void check_if_pressed(bool& value, int padIdx, int key, int keyboardKey, size_t delay)
			{
				static timer t;
				if (GetForegroundWindow() == pointers::g_hwnd) {
					if (GetAsyncKeyState(keyboardKey) & 1 || PAD::IS_DISABLED_CONTROL_JUST_PRESSED(padIdx, key) && !PAD::IS_USING_KEYBOARD_AND_MOUSE(padIdx)) { value = true; }
					else if (GetAsyncKeyState(keyboardKey) & 0x8000) {
						t.start(delay * 100);
						if (t.ready())
							value = true;
					}
					else { t.reset(); }
				}
			}

			void check_if_pressed(bool& value, int padIdx, int key, int key2, int keyboardKey, size_t delay)
			{
				static timer t;
				if (GetForegroundWindow() == pointers::g_hwnd) {
					if (GetAsyncKeyState(keyboardKey) & 1 || ((PAD::IS_DISABLED_CONTROL_PRESSED(padIdx, key) || PAD::IS_DISABLED_CONTROL_JUST_PRESSED(padIdx, key)) && PAD::IS_DISABLED_CONTROL_JUST_PRESSED(padIdx, key2) && !PAD::IS_USING_KEYBOARD_AND_MOUSE(padIdx))) { value = true; }
					else if (GetAsyncKeyState(keyboardKey) & 0x8000) {
						t.start(delay * 100);
						if (t.ready())
							value = true;
					}
					else { t.reset(); }
				}
			}

			void reset_input_handler()
			{
				m_open_key_pressed = false;
				m_back_key_pressed = false;
				m_enter_key_pressed = false;
				m_up_key_pressed = false;
				m_down_key_pressed = false;
				m_left_key_pressed = false;
				m_right_key_pressed = false;
			}

			void input_check_handler()
			{
				check_if_pressed(m_open_key_pressed, 2, 227, 175, VK_INSERT, m_openDelay);		// RB + R-Arrow
				check_if_pressed(m_back_key_pressed, 2, 194, VK_BACK, m_enterDelay);			// A
				check_if_pressed(m_enter_key_pressed, 2, 191, VK_RETURN, m_backDelay);			// B
				check_if_pressed(m_up_key_pressed, 2, 172, VK_UP, m_verticalDelay);				// U-Arrow
				check_if_pressed(m_down_key_pressed, 2, 173, VK_DOWN, m_verticalDelay);			// D-Arrow
				check_if_pressed(m_left_key_pressed, 2, 174, VK_LEFT, m_horizontalDelay);		// L-Arrow
				check_if_pressed(m_right_key_pressed, 2, 175, VK_RIGHT, m_horizontalDelay);		// R-Arrow
			}

			void actions()
			{
				if (m_open_key_pressed) 
				{
					sounds::queue(g_open ? sounds::g_open : sounds::g_close);
					g_open ^= true;
				}
				if (g_open)
				{
					if (m_enter_key_pressed)
					{
						sounds::queue(sounds::g_enter);
						CURRENT_MENU.action(eActionType::Enter);
					}
					if (m_back_key_pressed)
					{
						sounds::queue(sounds::g_back);
						menu::pop();
					}
					if (m_up_key_pressed)
					{
						sounds::queue(sounds::g_up);
						CURRENT_MENU.action(eActionType::Up);
					}
					if (m_down_key_pressed)
					{
						sounds::queue(sounds::g_down);
						CURRENT_MENU.action(eActionType::Down);
					}
					if (m_left_key_pressed)
					{
						sounds::queue(sounds::g_left);
						CURRENT_MENU.action(eActionType::Left);
					}
					if (m_right_key_pressed)
					{
						sounds::queue(sounds::g_right);
						CURRENT_MENU.action(eActionType::Right);
					}
				}
			}
		}
	}
}
