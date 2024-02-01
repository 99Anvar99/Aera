#pragma once
#include "abstract_option.h"
#include "gui/gui.h"
#include "gui/interface.h"
#include "commands/manager/types.h"

namespace ui
{
	class baseOption : public abstractOption
	{
	public:
		baseOption(std::string name, std::string description, std::function<void()> action) :
			abstractOption(name, description), m_action(action)
		{
		}

		void draw(bool selected) override
		{
			drawing::rectangle({g_pos.x, g_base + (g_options.m_size / 2.f)}, {g_width, g_options.m_size},
			                   g_options.m_color, true);
			const auto scrollPosition{g_base + (g_options.m_size / 2.f)};
			static lerping_float scroll(scrollPosition, g_options.m_scrollSpeed / 10.f);
			if (selected)
			{
				drawing::rectangle({
					                   g_pos.x,
					                   scroll.set_target(scrollPosition).update(g_options.m_scrollSpeed / 10.f).
					                          get_current()
				                   }, {g_width, g_options.m_size}, g_options.m_selectedColor);
			}
			drawing::text(
				g_options.m_font,
				m_name,
				{
					g_pos.x,
					g_base + (g_options.m_size / 2.f) - (drawing::get_text_size(
						g_options.m_font, g_options.m_textSize, m_name).y / 2.f)
				},
				g_options.m_textSize,
				g_options.text(selected),
				g_options.m_textJustify,
				g_options.m_padding,
				g_width
			);
			g_base += g_options.m_size;
		}

		void action(eActionType type) override
		{
			switch (type)
			{
			case eActionType::Enter:
				{
					invoke();
				}
				break;
			}
		}

		void invoke()
		{
			if (m_action)
			{
				m_action();
			}
		}

	private:
		std::function<void()> m_action{};
	};
}
