#include "hooking/hooking.h"
#include <fiber/manager.h>

bool hooks::c_run_gta_thread(uint32_t ops_to_execute)
{
	g_manager.on_game_tick();
	return g_hooking->m_cRunGtaThread.get_original<decltype(&c_run_gta_thread)>()(ops_to_execute);
}