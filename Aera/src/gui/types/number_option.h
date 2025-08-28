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
        numberOption(const std::string& name, const std::string& description,
                     T& value, T min, T max, T step = T(1),
                     bool enableEdit = false, bool onChange = false,
                     fnptr<void()> action = nullptr)
            : baseOption(name, description, action),
              m_value(&value),
              m_min(min),
              m_max(max),
              m_step(step),
              m_onChange(onChange),
              m_enableEdit(enableEdit)
        {}

        numberOption(std::string name, T& value, T min, T max, T step = T(1),
                     bool enableEdit = false, bool onChange = false,
                     fnptr<void()> action = nullptr)
            : numberOption(std::move(name), std::string(), value, min, max, step, enableEdit, onChange, action)
        {}

        void draw(const bool selected) override
        {
            baseOption::draw(selected);

            g_base -= g_options.m_size;

            const std::string valueText = remove_zeros(to_fixed(*m_value, 2));
            const std::string display   = selected ? "< " + valueText + " >" : valueText;

            drawing::text(
                g_options.m_font,
                display,
                {
                    g_pos.x,
                    g_base + g_options.m_size / 2.f -
                    drawing::get_text_height(g_options.m_font, g_options.m_textSize) / 2.f
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
                    run = decrement();
                    break;

                case eActionType::Right:
                    run = increment();
                    break;

                case eActionType::Enter:
                    if (m_enableEdit)
                        openKeyboard(run);
                    break;

                default:
                    break;
            }

            if (m_onChange && run)
                invoke();

            baseOption::action(type);
        }

    private:
        bool decrement()
        {
            if (*m_value > m_min)
            {
                const T newVal = *m_value - m_step;
                *m_value = math::are_numbers_equal(m_min, newVal) ? m_min : std::max(m_min, newVal);
                return true;
            }

            *m_value = m_max; // wrap around
            return false;
        }

        bool increment()
        {
            if (*m_value < m_max)
            {
                const T newVal = *m_value + m_step;
                *m_value = math::are_numbers_equal(m_max, newVal) ? m_max : std::min(m_max, newVal);
                return true;
            }

            *m_value = m_min; // wrap around
            return false;
        }

        void openKeyboard(bool& run)
        {
            g_pool.add([this, &run]
            {
                g_open = false;
                PAD::DISABLE_ALL_CONTROL_ACTIONS(0);

                MISC::DISPLAY_ONSCREEN_KEYBOARD(6, "FMMC_KEY_TIP8", "", "", "", "", "", 15);

                while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0)
                    fiber::current()->sleep();

                if (MISC::UPDATE_ONSCREEN_KEYBOARD() == 2) // canceled
                {
                    g_open = true;
                    return;
                }

                const std::string userInput = MISC::GET_ONSCREEN_KEYBOARD_RESULT();
                if (userInput.empty() || !contains_an_number(userInput) || contains_a_character(userInput))
                {
                    g_open = true;
                    return;
                }

                try
                {
                    if (is_integer(userInput))
                        setValue(static_cast<T>(std::stoi(userInput)));
                    else if (is_float(userInput))
                        setValue(static_cast<T>(std::stof(userInput)));

                    run = true;
                }
                catch (const std::exception&)
                {
                    // invalid input, ignore
                }

                g_open = true;
            });
        }

        void setValue(T val)
        {
            if (val > m_max)      *m_value = m_max;
            else if (val < m_min) *m_value = m_min;
            else                  *m_value = val;
        }

        T*   m_value{};
        T    m_min{};
        T    m_max{};
        T    m_step{};
        bool m_onChange{};
        bool m_enableEdit{};
    };
}