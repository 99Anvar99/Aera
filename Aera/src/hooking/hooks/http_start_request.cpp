#include "core/logger.h"
#include "hooking/hooking.h"

bool hooks::http_start_request(void* _this, const char* uri)
{
	if (strstr(uri, "Bonus"))
	{
		uri = "https://0.0.0.0/";
		LOG(Warn, "R* report has slipped through the cracks, Redirecting the report to: [{}]", uri);
	}
	return g_hooking->m_http_start_request.get_original<decltype(&http_start_request)>()(_this, uri);
}