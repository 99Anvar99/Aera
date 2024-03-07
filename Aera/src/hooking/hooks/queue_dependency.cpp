#include "core/logger.h"
#include "hooking/hooking.h"
#include "rage/classdefs.h"
#include "rage/classes.h"
#include "util/util.h"

struct ac_verifier
{
	virtual ~ac_verifier() = 0;
	virtual bool run() = 0;
	rage::Obf32 m_last_time; // 0x8 
	rage::Obf32 m_delay; // 0x18
};

bool is_unwanted_dependency(int64_t cb)
{
	int64_t f1 = *reinterpret_cast<int64_t*>(cb + 0x60);
	int64_t f2 = *reinterpret_cast<int64_t*>(cb + 0x100);
	int64_t f3 = *reinterpret_cast<int64_t*>(cb + 0x1A0);

	if (!util::inModuleRegion("GTA5.exe", f1) || !util::inModuleRegion("GTA5.exe", f2) || !util::inModuleRegion("GTA5.exe", f3))
		return false;

	if (*reinterpret_cast<uint8_t*>(f1) != 0xE9)
		return false;

	return true;
}

static bool nullsub()
{
	return true; // returning false would cause the dependency to requeue
}

int hooks::queue_dependency(void* a1, int a2, int64_t dependency)
{
	if (is_unwanted_dependency(dependency))
	{
		LOG(Info, "Blocking AC Verifier: {:0x}", *reinterpret_cast<int64_t*>(dependency + 0x60) - reinterpret_cast<
			int64_t>(GetModuleHandleA(nullptr)));
		const auto verifier = reinterpret_cast<ac_verifier*>(dependency - 0x30);
		verifier->m_delay = INT_MAX; // makes it so these won't queue in the future
		*reinterpret_cast<void**>(dependency + 0x60) = nullsub;
		*reinterpret_cast<void**>(dependency + 0x100) = nullsub;
		*reinterpret_cast<void**>(dependency + 0x1A0) = nullsub;
	}

	return g_hooking->m_queue_dependency.get_original<decltype(&queue_dependency)>()(a1, a2, dependency);
}
