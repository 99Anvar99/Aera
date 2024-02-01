#pragma once
#include "base_option.h"

namespace ui
{
	class breakOption : public baseOption
	{
	public:
		breakOption(std::string name) :
			baseOption(name, {}, {})
		{
		}

		u32 type() override
		{
			return static_cast<u32>(eOptionTypes::Break);
		}

		void draw(bool selected) override
		{
			drawing::rectangle({g_pos.x, g_base + (g_options.m_size / 2.f)}, {g_width, g_options.m_size},
			                   g_options.m_color, true);
			drawing::text(
				g_options.m_breakFont,
				m_name,
				{
					g_pos.x,
					g_base + (g_options.m_size / 2.f) - (drawing::get_text_height(
						g_options.m_breakFont, g_options.m_breakTextSize) / 2.f)
				},
				g_options.m_breakTextSize,
				g_options.m_breakTextColor,
				g_options.m_breakTextJustify,
				g_options.m_padding,
				g_width
			);
			g_base += g_options.m_size;
		}
	};
}
