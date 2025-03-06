#pragma once
#include "pch/pch.h"
#include "abstract_option.h"

enum
{
	max_visible_options = 13
};

namespace ui
{
	class submenu
	{
	public:
		explicit submenu(std::string name = {}, const fnptr<void(submenu&)> action = {}) : m_name(std::move(name)),
			m_action(action)
		{
		}

		template <typename T>
		void add(const T& option)
		{
			m_options.emplace_back(std::make_shared<T>(option));
		}

		void action(const eActionType type)
		{
			switch (type)
			{
			case eActionType::Up:
				if (m_current > 0)
				{
					u64 offset = m_options.at(m_current - 1)->type() == static_cast<u32>(eOptionTypes::Break) ? 2 : 1;
					m_current -= offset;
				}
				else
				{
					m_current = m_options.size() - 1;
				}
				break;
			case eActionType::Down:
				if (m_current < m_options.size() - 1)
				{
					u64 offset = m_options.at(m_current + 1)->type() == static_cast<u32>(eOptionTypes::Break) ? 2 : 1;
					m_current += offset;
				}
				else
				{
					m_current = m_options.at(0)->type() == static_cast<u32>(eOptionTypes::Break) ? 1 : 0;
				}
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
			for (u64 i = m_scrollPosition; i < std::min(m_scrollPosition + max_visible_options, m_options.size()); ++i)
			{
				m_options[i]->draw(m_current == i);
			}
			if (m_current >= m_options.size())
				m_current = 0;
		}

		std::string m_name;
		std::vector<std::shared_ptr<abstractOption>> m_options;
		u64 m_current{};
		u64 m_scrollPosition{};
		fnptr<void(submenu&)> m_action;

	private:
		void adjust_scroll_position()
		{
			// When scrolling up, ensure there's a buffer of 3 options above the current selection
			if (m_current < m_scrollPosition + 2)
			{
				m_scrollPosition = m_current > 2 ? m_current - 2 : 0;
			}
			// When scrolling down, ensure there's a buffer of 3 options below the current selection
			else if (m_current > m_scrollPosition + max_visible_options - 4)
			// Adjusted to -4 to correctly handle buffer
			{
				auto potential_new_scroll_position = m_current - max_visible_options + 3;
				m_scrollPosition = potential_new_scroll_position < m_options.size() - max_visible_options
					                   ? potential_new_scroll_position
					                   : std::max(m_options.size() - max_visible_options, static_cast<size_t>(0));
			}
		}
	};
}
