#include "hooking/hooking.h"
#include "commands/features.h"

void* hooks::c_task_jump_constructor(u64 a1, u32 flags)
{
	if ("beastJump"_TC->get(0).toggle)
		flags |= static_cast<u32>(eTaskFlags::BeastJumpWithSuper);
	return g_hooking->m_cTaskJumpConstructor.get_original<decltype(&c_task_jump_constructor)>()(a1, flags);
}
