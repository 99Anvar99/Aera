#include "classdefs.h"
#include "classes.h"
#include "memory/pointers.h"

uint64_t rage::datBitBuffer::WriteBitsSingle(uint32_t value, int32_t bits)
{
	return pointers::g_writeBitsSingle(m_data, value, bits, m_bitsRead + m_bitOffset);
}

uint32_t rage::datBitBuffer::ReadBitsSingle(uint32_t numBits)
{
	if ((m_flagBits & 2) || m_bitsRead + numBits > GetMaxDataLength())
	{
		return 0;
	}
	const uint32_t bufPos = m_bitsRead + m_bitOffset;
	const uint32_t initialBitOffset{bufPos & 7};
	uint8_t* const start{&m_data[bufPos / 8]};
	uint8_t* const next{&start[1]};
	int32_t result{(start[0] << initialBitOffset) & 0xFF};
	for (uint32_t i{}; i < ((numBits - 1) / 8); ++i)
	{
		result <<= 8;
		result |= next[i] << initialBitOffset;
	}
	if (initialBitOffset)
		result |= next[0] >> (8 - initialBitOffset);
	AddNumberOfBits(numBits);
	return result >> ((8 - numBits) % 8);
}

bool rage::datBitBuffer::WriteArray(void* array, int32_t size)
{
	return pointers::g_writeBitbufArray(this, array, size, 0);
}

bool rage::datBitBuffer::ReadArray(void* array, int32_t size)
{
	return pointers::g_readBitbufArray(this, array, size, 0);
}

int CGameScriptHandlerNetComponent::get_participant_index(CNetGamePlayer* player)
{
	if (player == (*pointers::g_networkPlayerMgr)->m_local_net_player)
		return m_local_participant_index;
	if (m_num_participants <= 1)
		return -1;
	for (decltype(m_num_participants) i{}; i != m_num_participants - 1; ++i)
	{
		if (m_participants[i] && m_participants[i]->m_net_game_player == player)
			return m_participants[i]->m_participant_index;
	}
	return -1;
}

bool CGameScriptHandlerNetComponent::force_host()
{
	if (auto& cNetworkPlayerMgr = *pointers::g_networkPlayerMgr; cNetworkPlayerMgr)
	{
		for (int32_t i{}; !is_local_player_host(); ++i)
		{
			if (i > 200)
				return false;

			send_host_migration_event(cNetworkPlayerMgr->m_local_net_player);
			send_host_migration_event(cNetworkPlayerMgr->m_local_net_player);
		}
		return is_local_player_host();
	}
	return false;
}

void CNetComplaintMgr::Add(uint64_t PeerAddress, bool UseAll, bool SpoofAddress)
{
	auto net_mgr = (*pointers::g_networkPlayerMgr);
	if (HasComplaint(PeerAddress) || PeerAddress == net_mgr->m_local_net_player->m_player_info->m_gamer_info.m_peer_address)
		return;
	auto old_token = m_peer_address;
	if (m_peer_address == old_token && SpoofAddress)
		m_peer_address = PeerAddress;
	if (UseAll)
	{
		for (auto player : net_mgr->m_player_list)
		{
			if (auto gamer_info = player->m_player_info->m_gamer_info; player && player->is_valid())
			{
				if (gamer_info.m_peer_address != PeerAddress)
					m_peer_addresses_complained[m_num_addresses_complained++] = gamer_info.m_peer_address;
			}
		}
	}
	m_peer_addresses_complained[m_num_addresses_complained++] = PeerAddress;
	if (m_peer_address != old_token && SpoofAddress)
		m_peer_address = old_token;
}
