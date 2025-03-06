#include "self.h"

#include "gui/types/vector_option.h"

namespace ui::submenus
{
	bool toggle;
	int value = 1;

	i64 pos = 0;
	std::vector vector_numbers = {"One", "Two", "Three"};

	void selfSubmenu::init(submenu& submenu)
	{
		submenu.add(toggleOption("godMode"_TC)); //Called from commands
		submenu.add(toggleOption("beastJump"_TC));
		submenu.add(toggleOption("gracefulLanding"_TC));
		submenu.add(breakOption("Demo"));
		submenu.add(option("Regular Option", [] { LOG(Info, "You pressed regular option!") }));
		submenu.add(toggleOption("Toggle Option", "Description", toggle));
		submenu.add(numberOption("Number Option", "Description", value, 1, 10, 1, true));
		submenu.add(vectorOption("Vector Option", "Description", vector_numbers, pos));
	}
}
