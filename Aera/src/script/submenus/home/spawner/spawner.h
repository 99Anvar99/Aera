#pragma once
#include "pch/pch.h"
#include "gui/menu_handler.h"
#include "gui/types/option.h"
#include "gui/types/toggle_option.h"
#include "gui/types/submenu_option.h"
#include "gui/types/number_option.h"
#include "gui/types/break_option.h"
#include "commands/features.h"

namespace ui::submenus
{
	class spawnerSubmenu
	{
		static void init(submenu& submenu);

	public:
		static menuInstance<spawnerSubmenu>* get()
		{
			return menuInstance<spawnerSubmenu>::get("Spawner", &init);
		}
	};

	class homeSpawnerVehicle
	{
		static void init(submenu& submenu);

	public:
		static menuInstance<homeSpawnerVehicle>* get()
		{
			return menuInstance<homeSpawnerVehicle>::get("Vehicles", &init);
		}
	};

	class selectedVehicleSpawnerSubmenmu
	{
		static void init(submenu& submenu);

	public:
		static menuInstance<selectedVehicleSpawnerSubmenmu>* get()
		{
			return menuInstance<selectedVehicleSpawnerSubmenmu>::get("InvalidClass", &init);
		}
	};

	class finalselectedVehicleSpawnerSubmenmu
	{
		static void init(submenu& submenu);

	public:
		static menuInstance<finalselectedVehicleSpawnerSubmenmu>* get()
		{
			return menuInstance<finalselectedVehicleSpawnerSubmenmu>::get("InvalidVehicle", &init);
		}
	};
}
