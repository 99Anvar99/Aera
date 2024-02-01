#include "script.h"
#include "gui/interface.h"
#include "fiber/fiber.h"
#include "fiber/pool.h"
#include "script/submenus/home.h"

namespace script
{
	void handle_keys()
	{
		if (ui::g_open)
		{
			PAD::DISABLE_CONTROL_ACTION(0, ControlPhone, TRUE);
		}
	}

	void on_tick()
	{
		while (commands::g_manager.getCommands().empty()) { fiber::current()->sleep(100ms); }
		ui::menu::push(ui::submenus::homeSubmenu::get());

		while (true)
		{
			handle_keys();
			fiber::current()->sleep();
		}
	}
}
