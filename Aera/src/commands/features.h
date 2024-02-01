#pragma once
#include "rage/classes.h"
#include "manager/manager.h"

namespace commands::features
{
	inline CPed* cPed{};
	inline CPedWeaponManager* cPedWeaponManager{};
	inline CWeaponInfo* cWeaponInfo{};
	inline CAmmoInfo* cAmmoInfo{};
	inline CVehicle* cVehicle{};
	inline CPlayerInfo* cPlayerInfo{};

	namespace self
	{
		inline bool disable_godmode = false;
		extern void god_mode(toggleCommand* command);
		extern void beast_jump(toggleCommand* command);
		extern void graceful_landing(toggleCommand* command);
	}

	extern void init();
	extern void uninit();
	extern void on_init();
	extern void on_tick();
}
