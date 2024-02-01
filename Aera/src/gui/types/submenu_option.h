#pragma once
#include "base_option.h"

namespace ui
{
	class submenuOption : public baseOption
	{
	public:
		submenuOption(const std::string& name, const std::string& description, submenu* submenu,
		              fnptr<void()> action = {}) :
			baseOption(name, description, action), m_submenu(submenu)
		{
		}

		submenuOption(const std::string& description, submenu* submenu, fnptr<void()> action = {}) :
			submenuOption(submenu->m_name, description, submenu, action)
		{
		}

		submenuOption(submenu* submenu, fnptr<void()> action = {}) :
			submenuOption({}, submenu, action)
		{
		}

		void draw(bool selected) override
		{
			baseOption::draw(selected);
			g_base -= g_options.m_size;
			drawing::image(
				g_options.m_arrow,
				{g_pos.x + g_width / g_options.m_padding, g_base + g_options.m_size / 2.f},
				g_options.m_arrowSize,
				g_options.arrow(selected)
			);
			g_base += g_options.m_size;
		}

		void action(eActionType type) override
		{
			switch (type)
			{
			case eActionType::Enter:
				{
					menu::push(m_submenu);
				}
				break;
			default: ;
			}
			baseOption::action(type);
		}

		u32 type() override
		{
			return static_cast<u32>(eOptionTypes::Submenu);
		}

	private:
		submenu* m_submenu{};
	};
}
