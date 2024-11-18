#include "memory/pointers.h"
#include "core/logger.h"
#include "scanner.h"

namespace pointers
{
	void scan_all()
	{
		g_loadingScreenState = scan("LSS", "83 3D ? ? ? ? ? 75 ? 8B 43").lea().add(1).as<decltype(g_loadingScreenState)>();
		g_scrThreadInit = scan("STI", "83 89 40 01 00 00 FF 83").as<decltype(g_scrThreadInit)>();
		g_scrThreadTick = scan("STT", "80 B9 4E 01 00 00 00").sub(0xF).as<decltype(g_scrThreadTick)>();
		g_scrThreadKill = scan("STK", "48 89 5C 24 08 57 48 83 EC 20 48 83 B9 18 01").as<decltype(g_scrThreadKill)>();
		g_frame_count = scan("FC", "8B 15 ? ? ? ? 41 FF CF").lea().as<decltype(g_frame_count)>();
		g_handleToPointer = scan("HTP", "83 F9 FF 74 31 4C").as<decltype(g_handleToPointer)>();
		g_pointerToHandle = scan("PTH", "48 8D 1C F8 48 8B CB").add(7).call().as<decltype(g_pointerToHandle)>();
		g_readBitbufArray = scan("RBA", "48 89 5C 24 08 57 48 83 EC 30 41 8B F8 4C").as<decltype(g_readBitbufArray)>();
		g_writeBitbufArray = scan("WBA", "48 89 5C 24 08 57 48 83 EC 30 F6 41 1C").as<decltype(g_writeBitbufArray)>();
		g_readBitsSingle = scan("RBS", "48 8D 54 24 68 41 C1 E1 03").add(0xF).call().as<decltype(g_readBitsSingle)>();
		g_writeBitsSingle = scan("WBS", "48 89 5C 24 08 44 8B DA 83").as<decltype(g_writeBitsSingle)>();
		g_gxtLabels = scan("GL", "48 8D 0D ? ? ? ? E8 ? ? ? ? 8B 0D ? ? ? ? 48 8B 5C 24 30").mov().as<decltype(g_gxtLabels)>();
		g_getGxtLabelFromTable = scan("GGLFT", "E8 ? ? ? ? 48 8D 4B 0B").call().as<decltype(g_getGxtLabelFromTable)>();
		g_getJoaatedGxtLabelFromTable = scan("GJGLFT", "48 83 EC 28 E8 ? ? ? ? 48 85 C0 75 34").as<decltype(g_getJoaatedGxtLabelFromTable)>();
		g_cTaskJumpConstructor = scan("CTJC", "48 89 5C 24 08 89 54 24 10 57 48 83 EC 30 0F").as<decltype(g_cTaskJumpConstructor)>();
		g_cTaskFallConstructor = scan("CTFC", "48 89 5C 24 08 89 54 24 10 57 48 83 EC 20 48 8B D9 E8").as<decltype(g_cTaskFallConstructor)>();
		g_scGetGameInfoIndex = scan("SGGII", "E8 ? ? ? ? 85 C0 78 27 3B 47 14 7D 1E 48 98 48 8B D6 48 69 C8",{"socialclub.dll"}).call().as<decltype(g_scGetGameInfoIndex)>();
		g_joinBySessionInfo = scan("JBSI", "89 6C 24 ? 4C 89 74 24 ? E8 ? ? ? ? EB").mov().as<decltype(g_joinBySessionInfo)>();
		g_textureStore = scan("TS", "48 8D 0D ? ? ? ? E8 ? ? ? ? 8B 45 EC").mov().as<decltype(g_textureStore)>();
		g_scGameInfo = scan("SGI", "48 8D 05 ? ? ? ? 48 03 F8 44 8B 47 14 48 8D 57 20 E8 ? ? ? ? 85",{"socialclub.dll"}).mov().as<decltype(g_scGameInfo)>();
		g_friendRegistry = scan("FR", "41 8B F4 3B C5").sub(0xB).lea().as<decltype(g_friendRegistry)>();
		g_scInfo = scan("SI", "48 8B D3 48 8D 4C 24 ? 48 69 D2").sub(4).rip().as<decltype(g_scInfo)>();
		g_networkPlayerMgr = scan("NPM", "48 8B 0D ? ? ? ? 8A D3 48 8B 01 FF 50 ? 4C 8B 07").mov().as<decltype(g_networkPlayerMgr)>();
		g_networkObjectMgr = scan("NOM", "48 8B 0D ? ? ? ? 45 33 C0 E8 ? ? ? ? 33 FF 4C 8B F0").mov().as<decltype(g_networkObjectMgr)>();
		g_network = scan("N", "48 8B 0D ? ? ? ? 48 8B D7 E8 ? ? ? ? 84 C0 75 17").mov().as<decltype(g_network)>();
		g_isSessionActive = scan("ISA", "40 38 35 ? ? ? ? 75 0E 4C 8B C3").cmp().as<decltype(g_isSessionActive)>();
		g_requestControl = scan("RC", "E8 ? ? ? ? EB 3E 48 8B D3").call().as<decltype(g_requestControl)>();
		g_pedFactory = scan("PF", "48 8B 05 ? ? ? ? 48 8B 48 08 48 85 C9 74 52").mov().as<decltype(g_pedFactory)>();
		g_replayInterface = scan("RI", "0F B7 44 24 ? 66 89 44 4E").add(0x1C).mov().as<decltype(g_replayInterface)>();
		g_httpStartRequest = scan("HSR", "48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 48 8B D9 48 81 C1 28 05").as<decltype(g_httpStartRequest)>();
		g_scriptHandlerMgr = scan("SHM", "74 17 48 8B C8 E8 ? ? ? ? 48 8D 0D").add(0xA).mov().as<decltype(g_scriptHandlerMgr)>();
		g_scrProgramTable = scan("SPT", "48 8B 1D ? ? ? ? 41 83 F8 FF").mov().as<decltype(g_scrProgramTable)>();
		g_getNativeHandler = scan("GNH", "48 8D 0D ? ? ? ? 48 8B 14 FA").add(12).rip().as<decltype(g_getNativeHandler)>();
		g_nativeRegistrationTable = scan("NRT", "48 8D 0D ? ? ? ? 48 8B 14 FA").mov().as<decltype(g_nativeRegistrationTable)>();
		g_swapChain = scan("SC", "48 8B 0D ? ? ? ? 48 8B 01 44 8D 43").mov().as<decltype(g_swapChain)>();
		g_hashTable = scan("MT", "4C 03 05 ? ? ? ? EB 03").mov().as<decltype(g_hashTable)>();
		g_gtaThreads = scan("GT", "F5 8B FD 73").add(5).mov().as<decltype(g_gtaThreads)>();
		g_globals = scan("G", "48 8B 8D B0 00 00 00 4C 8D 4D 08").add(0xB).mov().as<decltype(g_globals)>();
		g_threadId = scan("TI", "8B 15 ? ? ? ? 48 8B 05 ? ? ? ? FF C2 89 15 ? ? ? ? 48 8B 0C F8").lea().as<decltype(g_threadId)>();
		g_threadCount = scan("TC", "FF 0D ? ? ? ? 48 8B D9").lea().as<decltype(g_threadCount)>();
		g_hwnd = FindWindowA("grcWindow", nullptr);
		LOG(Info, "{}/{} pointers found. ({} failed)", g_foundSigCount, g_totalSigCount, g_failedSigCount);
	}
}
