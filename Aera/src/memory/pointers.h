#pragma once
#include "rage/classes.h"
#include "util/disable_anti_cheat_skeleton.h"

namespace pointers
{
	extern void scan_all();

	namespace types
	{
		using scrThreadInit = void(*)(rage::scrThread* thread);
		using scrThreadTick = rage::eThreadState(*)(rage::scrThread* thread, u32 opsToExecute);
		using scrThreadKill = void(*)(rage::scrThread* thread);
		using pointerToHandle = Entity(*)(rage::CEntity* ptr);
		using handleToPointer = rage::CDynamicEntity* (*)(Entity);
		using readBitbufArray = bool(*)(rage::datBitBuffer* Buffer, PVOID Read, i32 Bits, i32 Unk);
		using writeBitbufArray = bool(*)(rage::datBitBuffer* Buffer, PVOID Read, i32 Bits, i32 Unk);
		using readBitsSingle = bool(*)(u8* Data, u32* Value, i32 Bits, i32 Start);
		using writeBitsSingle = i64(*)(u8* Data, u32 Value, i32 Bits, i32 Start);
		using cTaskJumpConstructor = void* (*)(u64 _This, u32 Flags);
		using cTaskFallConstructor = void* (*)(u64 _This, u32 Flags);
		using scGetGameInfoIndex = int(*)(const char* StringId, u64 Unk, u32 GameId);
		using joinBySessionInfo = bool(*)(CNetwork* Network, rage::rlSessionInfo* Info, i32 Unk, i32 Flags, rage::rlGamerHandle* Handles, i32 HandleCount);
		using getGamerTaskResult = bool(*)(i32 ProfileIndex, rage::rlGamerHandle* pHandles, i32 Count, rage::rlSessionByGamerTaskResult* pResult, i32 Unk, bool* pSuccess, rage::rlTaskStatus* pStatus);
		using getNativeHandler = rage::scrNativeHandler(*)(rage::scrNativeRegistrationTable* registration_table, rage::scrNativeHash hash);
		using get_gxt_label_from_table_t = cc * (*)(void* This, cc* label);
		using get_joaated_gxt_label_from_table_t = cc * (*)(void* This, u32 hash);
		using request_control = void(*)(rage::netObject* net_object);
	}

	inline types::scrThreadInit g_scrThreadInit{};
	inline types::scrThreadTick g_scrThreadTick{};
	inline types::scrThreadKill g_scrThreadKill{};
	inline types::handleToPointer g_handleToPointer{};
	inline types::pointerToHandle g_pointerToHandle{};
	inline types::readBitbufArray g_readBitbufArray{};
	inline types::writeBitbufArray g_writeBitbufArray{};
	inline types::readBitsSingle g_readBitsSingle{};
	inline types::writeBitsSingle g_writeBitsSingle{};
	inline types::cTaskJumpConstructor g_cTaskJumpConstructor{};
	inline types::cTaskFallConstructor g_cTaskFallConstructor{};
	inline types::scGetGameInfoIndex g_scGetGameInfoIndex{};
	inline types::joinBySessionInfo g_joinBySessionInfo{};
	inline types::getGamerTaskResult g_getGamerTaskResult{};
	inline types::getNativeHandler g_getNativeHandler{};
	inline types::get_gxt_label_from_table_t g_getGxtLabelFromTable{};
	inline types::get_joaated_gxt_label_from_table_t g_getJoaatedGxtLabelFromTable{};
	inline types::request_control g_requestControl{};

	inline void* g_gxtLabels{};

	inline bool* g_isSessionActive{};
	inline uint32_t* g_frame_count{};
	inline rage::grcTextureStore* g_textureStore{};
	inline ScGameInfo* g_scGameInfo{};
	inline FriendRegistry* g_friendRegistry{};
	inline ScInfo* g_scInfo{};
	inline CNetworkPlayerMgr** g_networkPlayerMgr{};
	inline CNetworkObjectMgr** g_networkObjectMgr{};
	inline CNetwork** g_network{};
	inline CPedFactory** g_pedFactory{};
	inline CReplayInterface** g_replayInterface{};
	inline PVOID g_queueDependency{};
	inline PVOID g_httpStartRequest{};
	inline CGameScriptHandlerMgr* g_scriptHandlerMgr{};
	inline rage::scrProgramTable* g_scrProgramTable{};
	inline rage::scrNativeRegistrationTable* g_nativeRegistrationTable{};
	inline IDXGISwapChain** g_swapChain{};
	inline PresenceData** g_presenceData{};
	inline HashTable<rage::CBaseModelInfo*>* g_hashTable{};
	inline rage::atArray<GtaThread*>* g_gtaThreads{};
	inline rage::scrValue** g_globals{};
	inline eLoadingScreenState* g_loadingScreenState{};
	inline uint32_t* g_threadId{};
	inline uint32_t* g_threadCount{};
	inline HWND g_hwnd{};
}
