#include "hooking/hooking.h"
#include "commands/features.h"

void* hooks::c_task_fall_constructor(u64 a1, u32 flags)
{
	if ("gracefulLanding"_TC->get(0).toggle)
		flags |= static_cast<u32>(eTaskFlags::GracefulLanding);
	return g_hooking->m_cTaskFallConstructor.get_original<decltype(&c_task_jump_constructor)>()(a1, flags);
}
