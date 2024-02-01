#include "players.h"
#include "players/selected_player.h"

namespace ui::submenus
{
	void playersSubmenu::init(submenu& submenu)
	{
		for (auto& e : util::network::g_manager)
		{
			if (auto& p{e.second})
			{
				submenu.add(playerOption(p));
			}
		}
	}
}
