#include "network.h"
#include "players.h"

namespace ui::submenus
{
	void networkSubmenu::init(submenu& submenu)
	{
		submenu.add(option("Players", []
		{
			// Temporary solution for updating the player list
			const auto player_sub = playersSubmenu::get();
			player_sub->m_options.clear();
			player_sub->once();

			menu::push(player_sub);
		}));
	}
}
