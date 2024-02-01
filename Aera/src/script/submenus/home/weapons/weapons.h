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
	class weaponsSubmenu
	{
	private:
		static void init(submenu& submenu);

	public:
		static menuInstance<weaponsSubmenu>* get()
		{
			return menuInstance<weaponsSubmenu>::get("Weapons", &init);
		}
	};
}
