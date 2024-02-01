#pragma once
#include "base_option.h"
#include "commands/math.h"
#include "fiber/pool.h"
#include "rage/commands/list.h"

namespace ui
{
	template <typename T>
	class numberOption final : public baseOption
	{
	public:
		numberOption(const std::string& name, const std::string& description, T& value, T min, T max, T step = T(1),
		             bool enableEdit = false, const bool onChange = false, fnptr<void()> action = nullptr) :
			baseOption(name, description, action), m_value(&value), m_min(min), m_max(max), m_step(step),
			m_onChange(onChange), m_enableEdit(enableEdit)
		{
		}

		numberOption(std::string name, T& value, T min, T max, T step = T(1), bool enableEdit = false,
		             bool onChange = false, fnptr<void()> action = nullptr) :
			numberOption(name, std::string(), value, min, max, step, enableEdit, onChange, action)
		{
		}

		void draw(const bool selected) override
		{
			baseOption::draw(selected);
			g_base -= g_options.m_size;
			drawing::text(
				g_options.m_font,
				selected ? "< " + remove_zeros(to_fixed(*m_value, 2)) + " >" : remove_zeros(to_fixed(*m_value, 2)),
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
			g_base += g_options.m_size;
		}

		u32 type() override
		{
			return static_cast<u32>(eOptionTypes::Number);
		}

		void action(const eActionType type) override
		{
			bool run = false;
			switch (type)
			{
			case eActionType::Left:
				{
					if (*m_value > m_min)
					{
						T new_value = *m_value - m_step;
						*m_value = math::are_numbers_equal(m_min, new_value) ? m_min : std::max(m_min, new_value);
						run = true;
					}
					else
					{
						*m_value = m_max;
					}
				}
				break;
			case eActionType::Right:
				{
					if (*m_value < m_max)
					{
						T new_value = *m_value + m_step;
						*m_value = math::are_numbers_equal(m_max, new_value) ? m_max : std::min(m_max, new_value);
						run = true;
					}
					else
					{
						*m_value = m_min;
					}
				}
				break;
			case eActionType::Enter:
				{
					if (m_enableEdit)
					{
						g_pool.add([&]
						{
							g_open = false;
							// Disable all control actions to prevent unwanted input
							PAD::DISABLE_ALL_CONTROL_ACTIONS(0x0);
							// Display on-screen keyboard with a prompt
							MISC::DISPLAY_ONSCREEN_KEYBOARD(0x6, "FMMC_KEY_TIP8", "", "", "", "", "", 0xF);
							// Wait for the user to finish entering input
							while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0x0)
							{
								fiber::current()->sleep();
							}
							// Check if the user canceled the input
							if (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0x2)
							{
								g_open = true;
								return;
							}
							const std::string& user_input = MISC::GET_ONSCREEN_KEYBOARD_RESULT();
							// Check if the user provided valid input
							if (user_input.empty())
							{
								g_open = true;
								return;
							}
							// Check if input doesn't contain a number
							if (!contains_an_number(user_input) || contains_a_character(user_input))
							{
								g_open = true;
								return;
							}

							if (is_integer(user_input))
							{
								if (static_cast<T>(std::stoi(user_input)) > m_max)
								{
									*m_value = m_max;
								}
								else if (static_cast<T>(std::stoi(user_input)) < m_min)
								{
									*m_value = m_min;
								}
								else
								{
									*m_value = static_cast<T>(std::stoi(user_input));
								}
							}
							else if (is_float(user_input))
							{
								if (static_cast<T>(std::stof(user_input)) > m_max)
								{
									*m_value = m_max;
								}
								else if (static_cast<T>(std::stof(user_input)) < m_min)
								{
									*m_value = m_min;
								}
								else
								{
									*m_value = static_cast<T>(std::stof(user_input));
								}
							}

							run = true;

							g_open = true;
						});
					}
				}
				break;
			default: ;
			}
			if (m_onChange && run)
			{
				invoke();
			}
			baseOption::action(type);
		}

	private:
		T* m_value{};
		T m_min{};
		T m_max{};
		T m_step{};
		bool m_onChange{};
		bool m_enableEdit{};
	};
}
