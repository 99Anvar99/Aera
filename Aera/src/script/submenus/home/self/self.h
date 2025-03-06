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
	class selfSubmenu
	{
		static void init(submenu& submenu);

	public:
		static menuInstance<selfSubmenu>* get()
		{
			return menuInstance<selfSubmenu>::get("Self", &init);
		}
	};
}
