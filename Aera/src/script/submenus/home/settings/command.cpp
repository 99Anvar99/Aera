#include "command.h"

namespace ui::submenus::settings
{
	void commandSubmenu::init(submenu& submenu)
	{
		submenu.add(toggleOption("useDirectMatchResults"_TC));
		submenu.add(toggleOption("autoCompleteCommands"_TC));
		submenu.add(toggleOption("useFirstCommandOnMultipleResults"_TC));
	}
}
