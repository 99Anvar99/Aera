#include "invoker.h"
#include "core/logger.h"
#include "memory/pointers.h"

void native_invoker::fix_vectors()
{
	rage::scrThread::Info* info_instance_ptr{};
	info_instance_ptr->copy_referenced_parameters_out();
}

void native_invoker::cache_handlers()
{
	if (m_are_handlers_cached)
		return;

	for (size_t i = 0; i < g_crossmap.size(); i++)
	{
		m_handlers[i] = pointers::g_getNativeHandler(
			pointers::g_nativeRegistrationTable, g_crossmap[i]);
	}

	LOG(Info, "Cached {} handlers", g_crossmap.size())

	m_are_handlers_cached = true;
}
