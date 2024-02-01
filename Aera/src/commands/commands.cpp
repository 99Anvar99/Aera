#include "pch/pch.h"
#include "commands.h"
#include "fiber/fiber.h"
#include "util/util.h"
#include "util/clipboard.h"
#include "features.h"
#include "gui/gui.h"
#include "renderer/renderer.h"

namespace commands
{
	void copy_text(stringCommand* command)
	{
		util::clipboard clipboard{command->get(0).string};
		clipboard.set();
	}

	void copy_sc_string(stringCommand* command)
	{
		g_engine.primitiveExecute("copyText {}", util::network::socialclub::getString(command->get(0).string));
	}

	void print_cliboard(actionCommand* command)
	{
		util::clipboard clipboard{};
		clipboard.get();
		LOG_DIRECT(White, "Clipboard", "{}", clipboard.str())
	}

	void use_direct_match_result(toggleCommand* command)
	{
		g_engine.m_useDirectMatchResults = command->get(0).toggle;
	}

	void auto_complete_commands(toggleCommand* command)
	{
		g_engine.m_autoComplete = command->get(0).toggle;
	}

	void use_first_command_on_multiple_results(toggleCommand* command)
	{
		g_engine.m_useFirstResultOnTooManyResults = command->get(0).toggle;
	}

	void print(stringCommand* command)
	{
		std::string str{command->m_context.substr(command->m_context.find_first_of(' ') + 1)};
		LOG(Info, str)
	}

	void force_quit_to_sp(actionCommand* command)
	{
		NETWORK::SHUTDOWN_AND_LOAD_MOST_RECENT_SAVE();
	}

	void restart_game(actionCommand* command)
	{
		MISC::RESTART_GAME();
	}

	void unload(actionCommand* command)
	{
		g_running = false;
	}

	void exit(actionCommand* command)
	{
		g_running = false;
		g_exit_with_clean_up = true;
	}

	void init()
	{
		g_manager.add(stringCommand("copyText", "Copy Text", "Copies text to clipboard", copy_text));
		g_manager.add(stringCommand("copyScString", "Copy Socialclub String",
		                            "Copies a string from socialclub.dll to clipboard", copy_sc_string));
		g_manager.add(
			actionCommand("printClipboard", "Print Clipboard", "Prints your clipboard to log", print_cliboard));
		g_manager.add(toggleCommand("useDirectMatchResults", "Use Direct Match Results",
		                            "When an command is a direct match, it will use that when possible",
		                            use_direct_match_result));
		g_manager.add(toggleCommand("autoCompleteCommands", "Auto Complete Commands",
		                            "When a command is still being typed and there is no other results, it will auto complete the command",
		                            auto_complete_commands));
		g_manager.add(toggleCommand("useFirstCommandOnMultipleResults", "Use The First Command On Multiple Results",
		                            "When an command has multiple results, it will use the closet resembling command",
		                            use_first_command_on_multiple_results));
		g_manager.add(stringCommand("print", "Print", "Prints a string", print));
		g_manager.add(actionCommand("forceQuitToSp", "Force Quit To Story Mode", "Forcefully quits to SP",
		                            force_quit_to_sp));
		g_manager.add(actionCommand("restartGame", "Restart Game", "Restarts your game", restart_game));
		g_manager.add(actionCommand("unload", "Unload", "Removes " BRAND " from the game", unload));
		g_manager.add(actionCommand("exit", "Exit", "Exit the game", exit));
		features::init();
		g_manager.init();
	}

	void on_init()
	{
		features::on_init();
		"useDirectMatchResults"_TC->get(0).toggle = true;
		"autoCompleteCommands"_TC->get(0).toggle = true;
	}

	void on_tick()
	{
		init();
		on_init();
		while (true)
		{
			features::on_tick();
			g_manager.tick();
			fiber::current()->sleep();
		}
	}
}
