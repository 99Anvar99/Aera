#pragma once
#include "base_option.h"

namespace ui
{
	template <typename t>
	class vectorOption : public baseOption
	{
	public:
		vectorOption(std::string name, std::string description, std::vector<t> value, i64& pos, bool onChange = false,
		             fnptr<void()> action = nullptr) : baseOption(name, description, action), m_value(value),
		                                               m_pos(&pos), m_onChange(onChange)
		{
		}

		vectorOption(std::string name, std::vector<t> value, i64& pos, bool onChange = false,
		             fnptr<void()> action = nullptr) : vectorOption(name, std::string(), value, pos, onChange, action)
		{
		}

		void draw(bool selected) override
		{
			baseOption::draw(selected);
			g_base -= g_options.m_size;
			std::stringstream str{};
			str << m_value[*m_pos] << " [ " << *m_pos + 1 << " / " << m_value.size() << " ]";
			drawing::text(
				g_options.m_font,
				str.str(),
				{
					g_pos.x,
					g_base + g_options.m_size / 2.f - drawing::get_text_height(
						g_options.m_font, g_options.m_textSize) / 2.f
				},
				g_options.m_textSize,
				g_options.text(selected),
				eJustify::Right,
				g_options.m_padding,
				g_width
			);
			str.clear();
			g_base += g_options.m_size;
		}

		u32 type() override
		{
			return static_cast<u32>(eOptionTypes::Vector);
		}

		void action(eActionType type) override
		{
			bool run{};
			i64& pos{*m_pos};
			switch (type)
			{
			case eActionType::Left:
				{
					run = true;
					if (pos > 0)
					{
						pos--;
					}
					else
					{
						pos = static_cast<i64>(m_value.size() - 1);
					}
				}
				break;
			case eActionType::Right:
				{
					run = true;
					if (!m_value.empty())
					{
						pos = (pos + 1) % m_value.size();
					}
				}
				break;
			}
			if (m_onChange && run)
			{
				invoke();
			}
			baseOption::action(type);
		}

	private:
		std::vector<t> m_value{};
		i64* m_pos{};
		bool m_onChange{};
	};
}
