#include "pch.h"
#include "core/logger.h"

void stackWalker::OnOutput(LPCSTR sz_text)
{
	const std::string text(sz_text);
	LOG(Stackwalker, "{}", text.substr(0, text.size() - 1))
}

void stackWalker::OnLoadModule(LPCSTR img, LPCSTR mod, DWORD64 baseAddr, DWORD size, DWORD result, LPCSTR symType,
                               LPCSTR pdbName, ULONGLONG fileVersion)
{
	//StackWalker::OnLoadModule(img, mod, baseAddr, size, result, symType, pdbName, fileVersion);
}

void stackWalker::OnSymInit(LPCSTR szSearchPath, DWORD symOptions, LPCSTR szUserName)
{
	//StackWalker::OnSymInit(szSearchPath, symOptions, szUserName);
}

void stackWalker::OnDbgHelpErr(LPCSTR szFuncName, DWORD gle, DWORD64 addr)
{
	LOG(Stackwalker, "Error ({}) in {} at 0x{:X}", gle, szFuncName, addr)
}


void stackWalker::OnCallstackEntry(CallstackEntryType eType, CallstackEntry& entry)
{
    HMODULE game_hmod = GetModuleHandleA(nullptr);
    
    const char* module_type = entry.moduleName == "GTA5" ? "exe" : "dll";
    u64 base_offset = entry.moduleName == "GTA5" ? reinterpret_cast<u64>(game_hmod) : 0;
    u64 adjusted_offset = entry.offset - base_offset;

    std::string moduleNameStr = entry.moduleName;
    std::string offsetStr = std::format("{}.{}+0x{:X}", moduleNameStr, module_type, adjusted_offset);
    #ifdef DEBUG
	if (entry.lineFileName[0])
	{
		if (entry.name[0])
		{
			LOG(Stackwalker, "{} L{}: {} ({})", entry.lineFileName, entry.lineNumber, entry.name, offsetStr);
		}
	}
	else
	{
		if (entry.name[0])
		{
			LOG(Stackwalker, "{} ({})", entry.name, offsetStr);
		}
		else
		{
			LOG(Stackwalker, "{}", offsetStr)
		}
	}
#else
	if (!entry.lineFileName[0]) {
		if (entry.name[0]) {
			LOG(Stackwalker, "{} ({})", entry.name, offsetStr);
		}
		else {
			LOG(Stackwalker, "{}", offsetStr);
		}
	}
#endif
}

