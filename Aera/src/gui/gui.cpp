#include "pch/pch.h"
#include "gui.h"
#include "interface.h"
#include "types/option.h"
#include "util/imageLoaderHelpers.h"

namespace ui
{
	void tooltip::draw() const
	{
		if (!m_enabled)
			return;
		float pos{0.09f};
		constexpr float spacing{0.03f};
		const std::string& key_bind{"Insert or RB + Right"};
		drawing::text(
			m_font,
			m_text,
			{0.5f, pos},
			m_textSize,
			m_textColor,
			m_textJustify,
			2.1f,
			-1.f
		);
		drawing::text(
			m_font,
			key_bind,
			{0.5f, pos + spacing},
			m_textSize,
			m_textColor,
			m_textJustify,
			2.1f,
			-1.f
		);
	}

	void header::draw()
	{
		if (!m_enabled)
			return;
		if (!image_loader::m_has_header_loaded)
		{
			drawing::rectangle({g_pos.x, g_base + m_size / 2.f}, {g_width, m_size}, m_color);
			drawing::text(
				m_font,
				m_text,
				{g_pos.x, g_base + m_size / 2.f - drawing::get_text_size(m_font, m_textSize, m_text).y / 2.f},
				m_textSize,
				m_textColor,
				m_textJustify,
				2.1f,
				g_width
			);	
		}
		g_base += m_size;
	}

	void subtitle::draw()
	{
		if (!m_enabled)
			return;
		if (m_text != CURRENT_MENU.m_name)
			m_text = CURRENT_MENU.m_name;
		u64 num_options_calc{OPTION_COUNT};
		u64 selected_option_calc{OPTION_INDEX + 1};
		for (u64 i{}; i != num_options_calc; ++i)
		{
			if (CURRENT_MENU.m_options.at(i)->type() == static_cast<u32>(eOptionTypes::Break))
			{
				if (i < OPTION_INDEX + 1)
				{
					selected_option_calc--;
				}
				num_options_calc--;
			}
		}
		const std::string& option_text{std::format("{} / {}", selected_option_calc, num_options_calc)};
		//All hail uniform alignment.
		drawing::rectangle({g_pos.x, g_base + m_size / 2.f}, {g_width, m_size}, m_color);
		drawing::text(
			m_font,
			string_to_upper(CURRENT_MENU.m_name),
			{g_pos.x, g_base + m_size / 2.f - drawing::get_text_size(m_font, m_textSize, m_text).y / 2.f},
			m_textSize,
			m_textColor,
			m_textJustify,
			m_padding,
			g_width
		);
		drawing::text(
			m_optionFont,
			option_text,
			{
				g_pos.x,
				g_base + m_size / 2.f - drawing::get_text_size(m_optionFont, m_optionTextSize, option_text).y / 2.f
			},
			m_optionTextSize,
			m_optionTextColor,
			m_optionTextJustify,
			m_padding,
			g_width
		);
		g_base += m_size;
	}

	void options::draw()
	{
		CURRENT_MENU.handle([]
		{
			CURRENT_MENU.add(option("No options."));
		});
	}

	void footer::draw()
	{
		if (!m_enabled)
			return;
		float size{m_spriteSize};
		if (IS_STACK_VALID && default_sprite())
		{
			if (OPTION_INDEX == 0 || OPTION_INDEX + 1 == OPTION_COUNT)
			{
				constexpr float scale_factor = 1.f;
				m_sprite.m_texture = "arrowright";
				m_sprite.m_rotation = OPTION_INDEX == 0 ? 360.f : -180.f;
				size *= scale_factor;
			}
			else
			{
				m_sprite.m_texture = "shop_arrows_upanddown";
				m_sprite.m_rotation = 0.f;
				size *= 0.7f;
			}
		}
		drawing::rectangle({g_pos.x, g_base + m_size / 2.f}, {g_width, m_size}, m_color);
		drawing::image(m_sprite, {g_pos.x, g_base + m_size / 2.f}, {size, size}, m_spriteColor);
		g_base += m_size;
	}

	void description::draw()
	{
		if (!m_enabled)
			return;
		if (VALID_OPTION)
			m_text = CURRENT_OPTION.m_description;
		if (m_text.empty())
			return;
		g_base += m_height;
		const float wrap{g_width - 0.015f};
		const i32 line_count{drawing::get_line_count(m_text, m_font, m_textSize, wrap)};
		float size{
			(drawing::get_text_height(m_font, m_textSize, wrap) + 0.005f) * static_cast<float>(line_count) + (
				line_count <= 1 ? 0.01f : 0.005f)
		};
		drawing::rectangle({g_pos.x, g_base + size / 2.f}, {g_width, size}, m_color);
		drawing::text(
			m_font,
			m_text,
			{g_pos.x, g_base + m_size / 2.f - drawing::get_text_size(m_font, m_textSize, m_text).y / 2.f},
			m_textSize,
			m_textColor,
			m_textJustify,
			m_padding,
			wrap
		);
		g_base += size;
	}

	void draw()
	{
		if (IS_STACK_VALID)
		{
			ImGui::SetNextWindowPos(ImVec2(0, 0));
			ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
			if (ImGui::Begin("UI", nullptr, 935))
			{
				handlers::key::reset_input_handler();
				handlers::key::input_check_handler();
				handlers::key::actions();
				if (g_open)
				{
					g_base = g_pos.y;
					g_header.draw();
					g_subtitle.draw();
					g_options.draw();
					g_footer.draw();
					g_description.draw();
				}
				else
				{
					g_tooltip.draw();
				}
				ImGui::End();
			}
		}
		if (g_push_menu)
		{
			g_menus.push(g_menu);
			g_push_menu = false;
		}
	}
}
