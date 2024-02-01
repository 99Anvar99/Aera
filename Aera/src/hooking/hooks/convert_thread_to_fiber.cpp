#include "hooking/hooking.h"

LPVOID hooks::convert_thread_to_fiber(LPVOID param)
{
	if (IsThreadAFiber())
		return GetCurrentFiber();
	return g_hooking->m_convertThreadToFiber.get_original<decltype(&convert_thread_to_fiber)>()(param);
}
