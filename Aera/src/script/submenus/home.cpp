#include "home.h"
#include "home/miscellaneous/miscellaneous.h"
#include "home/network/network.h"
#include "home/protections/protections.h"
#include "home/recovery/recovery.h"
#include "home/self/self.h"
#include "home/settings/settings.h"
#include "home/spawner/spawner.h"
#include "home/teleport/teleport.h"
#include "home/vehicle/vehicle.h"
#include "home/weapons/weapons.h"

namespace ui::submenus
{
	void homeSubmenu::init(submenu& submenu)
	{
		submenu.add(submenuOption(selfSubmenu::get()));
		submenu.add(submenuOption(protectionsSubmenu::get()));
		submenu.add(submenuOption(networkSubmenu::get()));
		submenu.add(submenuOption(weaponsSubmenu::get()));
		submenu.add(submenuOption(vehicleSubmenu::get()));
		submenu.add(submenuOption(teleportSubmenu::get()));
		submenu.add(submenuOption(spawnerSubmenu::get()));
		submenu.add(submenuOption(recoverySubmenu::get()));
		submenu.add(submenuOption(miscellaneousSubmenu::get()));
		submenu.add(submenuOption(settingsSubmenu::get()));
	}
}
