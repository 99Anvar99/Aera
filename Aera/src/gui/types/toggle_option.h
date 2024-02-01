#pragma once
#include "base_option.h"

namespace ui
{
	class toggleOption : public baseOption
	{
	public:
		toggleOption(std::string name, std::string description, bool& toggle,
		             fnptr<void()> action = nullptr) : baseOption(name, description, action), m_toggle(&toggle)
		{
		}

		toggleOption(std::string name, bool& toggle, fnptr<void()> action = nullptr) : toggleOption(
			name, std::string(), toggle, action)
		{
		}

		toggleOption(commands::toggleCommand* cmd) : baseOption(cmd->m_name, cmd->m_description, {}),
		                                             m_toggle(&cmd->get(0).toggle)
		{
		}

		void draw(bool selected) override
		{
			baseOption::draw(selected);
			g_base -= g_options.m_size;
			drawing::image(
				g_options.m_toggle,
				{g_pos.x + (g_width / g_options.m_padding), g_base + (g_options.m_size / 2.f)},
				g_options.m_toggleSize,
				g_options.toggle(*m_toggle)
			);
			g_base += g_options.m_size;
		}

		u32 type() override
		{
			return static_cast<u32>(eOptionTypes::Toggle);
		}

		void action(eActionType type) override
		{
			switch (type)
			{
			case eActionType::Enter:
				{
					*m_toggle ^= true;
				}
				break;
			}
			baseOption::action(type);
		}

	private:
		bool* m_toggle{};
	};
}
