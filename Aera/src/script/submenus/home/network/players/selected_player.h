#pragma once
#include "pch/pch.h"
#include "gui/menu_handler.h"
#include "gui/types/option.h"
#include "gui/types/toggle_option.h"
#include "gui/types/submenu_option.h"
#include "gui/types/number_option.h"
#include "gui/types/break_option.h"
#include "commands/features.h"

namespace ui::submenus::players
{
	class selectedPlayerSubmenu
	{
		static void init(submenu& submenu);

	public:
		static menuInstance<selectedPlayerSubmenu>* get()
		{
			return menuInstance<selectedPlayerSubmenu>::get("Selected Player", &init);
		}
	};
}
