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
	private:
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
	private:
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
	private:
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
	private:
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
	private:
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
	private:
		static void init(submenu& submenu);

	public:
		static menuInstance<descriptionSubmenu>* get()
		{
			return menuInstance<descriptionSubmenu>::get("Description", &init);
		}
	};
}
