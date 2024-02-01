#include "features.h"
#include "fiber/pool.h"
#include "hooking/hooking.h"
#include "rage/lists.h"
#include "renderer/renderer.h"

namespace commands::features
{
	namespace self
	{
		u32 m_last_god_mode_bits = 0;
		u32 god_mode_bits = 0;

		void god_mode(toggleCommand* command)
		{
			if (cPed == nullptr)
			{
				LOG(Warn, "Cped is nullptr")
				return;
			}
			if ("godMode"_TC->get(0).toggle && !disable_godmode)
			{
				god_mode_bits |= 0x100;
				// Calculate changed bits
				const u32 changed_bits = god_mode_bits ^ m_last_god_mode_bits;
				// Calculate combined bits (enabled bits or changed bits)
				const u32 enabled_or_changed_bits = god_mode_bits | changed_bits;
				// Calculate unchanged bits by clearing changed or enabled bits
				const u32 unchanged_bits = cPed->m_damage_bits & ~enabled_or_changed_bits;
				// Set the character's damage bits with god mode bits
				cPed->m_damage_bits = unchanged_bits | god_mode_bits;
				disable_godmode = true;
			}
			if (cPed != nullptr && !"godMode"_TC->get(0).toggle && disable_godmode)
			{
				cPed->m_damage_bits = NULL;
				disable_godmode = false;
			}
		}

		void beast_jump(toggleCommand* command)
		{
		}

		void graceful_landing(toggleCommand* command)
		{
		}
	}

	const std::vector<std::string> g_textures = {
		"commonmenu"
	};

	void request_textures()
	{
		g_pool.add([]
		{
			for (const auto& texture : g_textures)
			{
				while (!GRAPHICS::HAS_STREAMED_TEXTURE_DICT_LOADED(texture.c_str()))
				{
					GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT(texture.c_str(), TRUE);
					LOG(Registers, "Requesting in game textures: {}", texture.c_str())
					fiber::current()->sleep();
				}
			}
		});
	}

	void unload_textures()
	{
		g_pool.add([]
		{
			for (const auto& textures : g_textures)
			{
				if (GRAPHICS::HAS_STREAMED_TEXTURE_DICT_LOADED(textures.c_str()))
				{
					GRAPHICS::SET_STREAMED_TEXTURE_DICT_AS_NO_LONGER_NEEDED(textures.c_str());
					LOG(Registers, "Unloading in game textures: {}", textures.c_str())
				}
				else
				{
					LOG(Info, "Unloading without removing any in game textures")
				}
			}
		});
	}

	void init()
	{
		g_manager.add(toggleCommand("godMode", "GodMode", self::god_mode));
		g_manager.add(toggleCommand("gracefulLanding", "Graceful Landing", self::graceful_landing));
		g_manager.add(toggleCommand("beastJump", "Beast Jump", self::beast_jump));
	}

	void uninit()
	{
		const auto ped{PLAYER::PLAYER_PED_ID()};
		ENTITY::SET_ENTITY_COLLISION(ped, TRUE, TRUE);
		ENTITY::RESET_ENTITY_ALPHA(ped);
		ENTITY::SET_ENTITY_VISIBLE(ped, TRUE, FALSE);
		ENTITY::SET_ENTITY_HAS_GRAVITY(ped, TRUE);
		PLAYER::SET_MAX_WANTED_LEVEL(5);
		PLAYER::SET_POLICE_RADAR_BLIPS(TRUE);
		const auto player{PLAYER::PLAYER_ID()};
		PLAYER::RESET_WANTED_LEVEL_HIDDEN_ESCAPE_TIME(player);
		unload_textures();
	}

	void on_init()
	{
		request_textures();
		//These need to be after init because the values aren't created yet
	}

	void on_tick()
	{
		cPed = util::classes::getPed();
		cPedWeaponManager = cPed->m_weapon_manager;
		cWeaponInfo = cPedWeaponManager->m_weapon_info;
		cAmmoInfo = cWeaponInfo->m_ammo_info;
		cVehicle = cPed->m_vehicle;
		cPlayerInfo = util::classes::getPlayerInfo();
	}
}
