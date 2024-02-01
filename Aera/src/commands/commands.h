#pragma once
#include "manager/manager.h"
#include "engine/engine.h"

namespace commands
{
	extern void copy_text(stringCommand* command);
	extern void copy_sc_string(stringCommand* command);
	extern void print_cliboard(actionCommand* command);
	extern void use_direct_match_result(toggleCommand* command);
	extern void auto_complete_commands(toggleCommand* command);
	extern void use_first_command_on_multiple_results(toggleCommand* command);
	extern void print(stringCommand* command);
	extern void force_quit_to_sp(actionCommand* command);
	extern void restart_game(actionCommand* command);
	extern void unload(actionCommand* command);
	extern void exit(actionCommand* command);
	extern void init();
	extern void on_init();
	extern void on_tick();

	inline bool g_exit_with_clean_up = false;
}
