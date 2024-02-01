#pragma once
#include "pch/pch.h"
#include "abstract_option.h"
#define MAX_VISIBLE_OPTIONS 13

namespace ui
{
	class submenu
	{
	public:
		submenu(std::string name = {}, fnptr<void(submenu&)> action = {}) : m_name(name), m_action(action)
		{
		}

		template <typename t>
		void add(t option)
		{
			m_options.emplace_back(new t(option));
		}

		void action(eActionType type)
		{
			switch (type)
			{
			case eActionType::Up:
				{
					if (m_current > 0)
					{
						u64 offset{1};
						if (m_options.at(m_current - 1)->type() == static_cast<u32>(eOptionTypes::Break))
						{
							offset += 1;
						}
						m_current -= offset;
					}
					else
					{
						m_current = m_options.size() - 1;
					}
				}
				break;
			case eActionType::Down:
				{
					if (m_current < m_options.size() - 1)
					{
						u64 offset{1};
						if (m_options.at(m_current + 1)->type() == static_cast<u32>(eOptionTypes::Break))
						{
							offset += 1;
						}
						m_current += offset;
					}
					else
					{
						u64 offset{};
						if (m_options.at(0)->type() == static_cast<u32>(eOptionTypes::Break))
						{
							offset += 1;
						}
						m_current = offset;
					}
				}
				break;
			}
			if (m_current < m_scrollPosition)
			{
				m_scrollPosition = m_current;
			}
			if (m_current > m_scrollPosition + MAX_VISIBLE_OPTIONS - 1)
			{
				m_scrollPosition = m_current - MAX_VISIBLE_OPTIONS + 1;
			}
			m_options[m_current]->action(type);
		}

		void once()
		{
			if (m_action)
				m_action(*this);
		}

		void handle(std::function<void()> noOptions)
		{
			if (m_options.empty())
				noOptions();
			for (u64 i{m_scrollPosition}; i < m_scrollPosition + MAX_VISIBLE_OPTIONS; ++i)
			{
				if (i < m_options.size())
				{
					auto&& opt{*m_options[i]};
					opt.draw(m_current == i);
				}
			}
			if (m_current >= m_options.size())
				m_current = 0;
		}

		std::string m_name{};
		std::vector<std::shared_ptr<abstractOption>> m_options{};
		u64 m_current{};
		u64 m_scrollPosition{};
		fnptr<void(submenu&)> m_action{};
	};
}
