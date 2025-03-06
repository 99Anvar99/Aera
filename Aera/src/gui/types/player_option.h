#pragma once
#include "util/player_mgr.h"
#include "script/submenus/home/network/players/selected_player.h"

namespace ui
{
	class playerOption : public baseOption
	{
	public:
		playerOption(const util::network::player& player, fnptr<void()> action = {}) :
			baseOption(player.m_name, {}, action), m_submenu(submenus::players::selectedPlayerSubmenu::get()),
			m_player(player)
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
			if (type == eActionType::Enter)
			{
				m_submenu->m_name = m_player.m_name;
				g_selectedPlayer = m_player.m_index;
				menu::push(m_submenu);
			}
			baseOption::action(type);
		}

		u32 type() override
		{
			return static_cast<u32>(eOptionTypes::Player);
		}

	private:
		submenu* m_submenu{};
		util::network::player m_player{};
	};
}
