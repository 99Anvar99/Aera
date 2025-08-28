#pragma once
#include "pch/pch.h"
#include "abstract_option.h"

namespace ui
{
    inline constexpr std::size_t max_visible_options = 13;

    class submenu
    {
    public:
        explicit submenu(std::string name = {}, fnptr<void(submenu&)> action = nullptr)
            : m_name(std::move(name)), m_action(action)
        {}

        template <typename T>
        void add(const T& option)
        {
            m_options.emplace_back(std::make_shared<T>(option));
        }

        void action(const eActionType type)
        {
            if (m_options.empty())
                return;

            switch (type)
            {
                case eActionType::Up:
                    navigate_up();
                    break;

                case eActionType::Down:
                    navigate_down();
                    break;

                default:
                    break;
            }

            adjust_scroll_position();
            m_options[m_current]->action(type);
        }

        void once()
        {
            if (m_action)
                m_action(*this);
        }

        void handle(const std::function<void()>& no_options)
        {
            if (m_options.empty())
            {
                no_options();
                return;
            }

            const auto end = std::min(m_scrollPosition + max_visible_options, m_options.size());
            for (std::size_t i = m_scrollPosition; i < end; ++i)
                m_options[i]->draw(m_current == i);

            // Safety: clamp current index in case options were removed dynamically
            if (m_current >= m_options.size())
                m_current = 0;
        }

        std::string m_name;
        std::vector<std::shared_ptr<abstractOption>> m_options;
        std::size_t m_current{};
        std::size_t m_scrollPosition{};
        fnptr<void(submenu&)> m_action;

    private:
        void navigate_up()
        {
            if (m_current == 0)
            {
                m_current = m_options.size() - 1;
                return;
            }

            const auto prevType = m_options[m_current - 1]->type();
            m_current -= (prevType == static_cast<u32>(eOptionTypes::Break)) ? 2 : 1;
        }

        void navigate_down()
        {
            if (m_current >= m_options.size() - 1)
            {
                m_current = (m_options[0]->type() == static_cast<u32>(eOptionTypes::Break)) ? 1 : 0;
                return;
            }

            const auto nextType = m_options[m_current + 1]->type();
            m_current += (nextType == static_cast<u32>(eOptionTypes::Break)) ? 2 : 1;
        }

        void adjust_scroll_position()
        {
            // keep a buffer of 2 above
            if (m_current < m_scrollPosition + 2)
            {
                m_scrollPosition = (m_current > 2) ? m_current - 2 : 0;
            }
            // keep a buffer of 3 below
            else if (m_current > m_scrollPosition + max_visible_options - 4)
            {
                const auto potential = m_current + 1 > max_visible_options
                                         ? m_current - max_visible_options + 3
                                         : 0;

                m_scrollPosition = (potential < m_options.size() - max_visible_options)
                                     ? potential
                                     : std::max(m_options.size() > max_visible_options
                                                    ? m_options.size() - max_visible_options
                                                    : 0ull,
                                                0ull);
            }
        }
    };
}