#include "selected_player.h"
#include "util/player_mgr.h"

namespace ui::submenus::players
{
	void selectedPlayerSubmenu::init(submenu& submenu)
	{
		submenu.add(option("Test", []
		{
			auto [m_index, m_host, m_name, m_pos, m_heading, m_ped, m_vehicle, m_vehicleModelInfo, m_navigation,
				m_playerInfo, m_netGamePlayer, m_snPlayer, m_snPeer, m_gamerInfo, m_address, m_ip, m_handle, m_rid,
				m_peerId, m_peerAddress, m_platformData, m_port, m_data]{util::network::g_manager.selected()};
			const rage::vector3 pos{m_pos};
			commands::features::cPed->set_position(pos);
		}));
	}
}
