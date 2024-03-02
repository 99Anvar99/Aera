#include "players.h"

#include <ranges>

#include "players/selected_player.h"

namespace ui::submenus
{
	void playersSubmenu::init(submenu& submenu)
	{
		for (auto& player : util::network::g_manager | std::views::values)
		{
			if (player)
			{
				submenu.add(playerOption(player));
			}
		}
	}
}
