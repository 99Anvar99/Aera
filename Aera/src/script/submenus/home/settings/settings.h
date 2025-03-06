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
	class settingsSubmenu
	{
		static void init(submenu& submenu);

	public:
		static menuInstance<settingsSubmenu>* get()
		{
			return menuInstance<settingsSubmenu>::get("Settings", &init);
		}
	};

	//Header
	class headerSubmenu
	{
		static void init(submenu& submenu);

	public:
		static menuInstance<headerSubmenu>* get()
		{
			return menuInstance<headerSubmenu>::get("Header", &init);
		}
	};

	//subtitle
	class subtitleSubmenu
	{
		static void init(submenu& submenu);

	public:
		static menuInstance<subtitleSubmenu>* get()
		{
			return menuInstance<subtitleSubmenu>::get("Subtitle", &init);
		}
	};

	//options
	class optionsSubmenu
	{
		static void init(submenu& submenu);

	public:
		static menuInstance<optionsSubmenu>* get()
		{
			return menuInstance<optionsSubmenu>::get("Options", &init);
		}
	};

	//footer
	class footerSubmenu
	{
		static void init(submenu& submenu);

	public:
		static menuInstance<footerSubmenu>* get()
		{
			return menuInstance<footerSubmenu>::get("Footer", &init);
		}
	};

	//description
	class descriptionSubmenu
	{
		static void init(submenu& submenu);

	public:
		static menuInstance<descriptionSubmenu>* get()
		{
			return menuInstance<descriptionSubmenu>::get("Description", &init);
		}
	};
}
