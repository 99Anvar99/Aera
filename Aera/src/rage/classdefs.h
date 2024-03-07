#pragma once
#include "pch/framework.h"
#include "joaat.h"
#include "enums.h"
#include <cassert>
#define LODWORD(l)           ((DWORD)(((DWORD_PTR)(l)) & 0xffffff))
#define HIDWORD(l)           ((DWORD)((((DWORD_PTR)(l)) >> 16) & 0xffffff))
#define SLODWORD(l)          ((signed long)(((DWORD_PTR)(l)) & 0xffffffff))
#define SHIDWORD(l)          ((signed long)((((DWORD_PTR)(l)) >> 16) & 0xffffffff))
#define DEFINE_AT_RTTI(T) private: \
	virtual T* GetIdentifier() { return nullptr; }; \
	virtual T* GetIdentifier_2() { return nullptr; }; \
	virtual uint32_t GetTypeHash() { return NULL; }; \
	virtual T* GetIfIsOfType(T* vft) { return vft; }; \
	virtual void IsOfType(T* vft) {}; \
	virtual void IsOfTypeObject(T* object) {}; \
	public:

class CNavigation;
class CNetShopTransaction;
class CNetShopTransactionBasket;
class CNetworkShoppingMgr;
class UIElement;
class UIElementConstant;
class UIIndex;
class UIElementInt;
class UIElementShader;
class TimecycleKeyframeData;
class CHeaders;
class CHttpRequest;
class CGameScriptId;
class CGameScriptObjInfo;
class NodeCommonDataOperations;
class CProjectBaseSyncDataNode;
class CSyncDataNodeFrequent;
class CSyncDataNodeInfrequent;
class CSectorPositionDataNode;
class CSectorDataNode;
class CTrainGameStateDataNode;
class CObjectCreationDataNode;
class CPickupCreationDataNode;
class CDoorCreationDataNode;
class CPedCreationDataNode;
class CVehicleCreationDataNode;
class CPlayerCreationDataNode;
class CPlayerGameStateDataNode;
class CPedGameStateDataNode;
class CPhysicalAttachDataNode;
class CPedAttachDataNode;
class CPedComponents;
class CPlayerAppearanceDataNode;
class CVehicleProximityMigrationDataNode;
class CObject;
class Vector2;
class Vector3;
class Vector4;
class FriendInfo;
class FriendList;
class FriendRegistry;
class CNonPhysicalPlayerData;
class CHandlingObject;
class CBaseSubHandlingData;
class CAdvancedData;
class CCarHandlingData;
class CHandlingData;
class CVehicleModelInfo;
class CVehicle;
class CItemInfo;
class CAmmoInfo;
class CAimingInfo;
class CForceInfo;
class CWeaponInfo;
class CPedWeaponManager;
class CPedFactory;
class CPedModelInfo;
class CPedBoneInfo;
class CPedIntelligence;
class CPedInventory;
class CPed;
class CPlayerInfo;
class ClanData;
class CNetGamePlayer;
class CNetworkPlayerMgr;
class GtaThread;
class CNetworkObjectMgr;
class HashNode;
template <typename T>
class HashTable;
class CPickup;
class CPickupHandle;
class CPickupList;
class CPickupInterface;
class CObjectHandle;
class CObjectList;
class CObjectInterface;
class CPedHandle;
class CPedList;
class CPedInterface;
class CVehicleHandle;
class CVehicleList;
class CVehicleInterface;
class CReplayInterface;
class CGameScriptHandlerMgr;
class CScriptParticipant;
class CGameScriptHandlerNetComponent;
class CScriptedGameEvent;
class CNetworkIncrementStatEvent;
class ScInfo;
class ScGame;
class ScGameInfo;
class MetricSessionMigrated;
class NetworkGameConfig;
class NetworkGameFilterMatchmakingComponent;
class MatchmakingAttributes;
class NetworkGameFilter;
class SessionInfoBackup;
class MatchmakingSessionResult;
class CNetRemoteComplaint;
class CNetComplaintMgr;
class PlayerNameMapNode;
class JoiningPlayerNameMap;
class CNetBlacklistNode;
class CNetBlacklist;
class RemotePlayerData;
class InvitedGamer;
class InvitedGamers;
class CNetwork;
class CMsgTextMessage;
class CMsgTextMessage2;
class PresenceData;
class CPlayerListMenu;

namespace rage
{
	template <typename T>
	class atRTTI;
	struct vector2;
	struct vector3;
	struct vector4;
	union matrix34;
	union matrix44;
	class scrVector;
	class netLoggingInterface;
	class nonPhysicalPlayerDataBase;
	union netAddress;
	class netSocketAddress;
	class rlGamerHandle;
	class rlPeerInfo;
	class rlGamerInfo;
	class netPlayer;
	class netPlayerMgrBase;
	class fwDrawData;
	class pgBase;
	class datBase;
	template <typename T, typename Base = datBase>
	class atDNode;
	template <typename Node>
	class atDList;
	template <typename T>
	class fwRefAwareBaseImpl;
	class fwRefAwareBase;
	class fwExtension;
	class fwExtensionContainer;
	class fwExtensibleBase;
	class fwArchetypeDef;
	class fwArchetype;
	class CBaseModelInfo;
	class fwDynamicEntityComponent;
	class crmtRequestPose;
	class crmtRequestIk;
	class crFrameFilter;
	class fwAudEntity;
	class phBoundBase;
	class phBound;
	class phBoundCapsule;
	class phBoundComposite;
	class phArchetype;
	class phArchetypePhys;
	class phArchetypeDamp;
	class fwEntity;
	class datBitBuffer;
	class netGameEvent;
	template <typename T>
	class atArray;
	enum class eThreadState : uint32_t;
	class tlsContext;
	union scrValue;
	class scrThread;
	inline constexpr u32 scrPageShift = 14;
	inline constexpr u32 scrPageSize = (1 << scrPageShift);
	inline constexpr u32 scrPageMask = (scrPageSize - 1);

	inline constexpr u32 scrStringShift = 14;
	inline constexpr u32 scrStringSize = (1 << scrStringShift);
	inline constexpr u32 scrStringMask = (scrStringSize - 1);
	class scrProgram;
	class scrProgramTableEntry;
	class scrProgramTable;
	class scriptHandlerObject;
	class sysMemAllocator;
	class scriptIdBase;
	class scriptId;
	class scriptResource;
	class scriptHandler;
	class scriptHandlerNetComponent;
	class rlSessionInfo;
	class rlSessionByGamerTaskResult;
	class rlPresenceEventInviteAccepted;
	class netSyncTree;
	class netObject;
	class netObjectMgrBase;
	class CEntityDrawHandler;
	class CEntity;
	class CDynamicEntity;
	class CPhysical;
	class scriptHandlerMgr;
	using scrNativeHash = uint64_t;
	class scrNativeRegistration;
	class scrNativeRegistrationTable;
	class rlJSON;
	class JSONNode;
	class rlMetric;
	template <typename t>
	class ObfVar;
	using Obf16 = ObfVar<unsigned short>;
    using Obf32 = ObfVar<unsigned int>;
	class rlSessionDetail;
	class rlMatchmakingFindResult;
	class netGamePlayerData;
	class netConnectionManager;
	class netPeerAddress;
	class netQueuedMessage;
	class netMessageQueue;
	class netPackedMessage;
	class netPackedMessageQueue;
	class netConnectionQueue;
	class netConnectionPeer;
	class netEventMgr;
	class snPlayer;
	class snPeer;
	class rlRemoteGamer;
	class rlTaskStatus;
	class snSession;
	class snMsgRemoveGamersFromSessionCmd;
	class snMsgAddGamerToSessionCmd;
	enum InitFunctionType : int;
	class InitFunctionData;
	class InitFunctionEntry;
	class InitFunctionList;
	class gameSkeletonUpdateBase;
	class UpdateFunctionList;
	class gameSkeleton;
	enum class grcLockFlags;
	class grcLockedTexture;
	class grcTexture;
	class grcRenderTarget;
	class grcTextureFactory;
	template <typename T>
	class pgDictionary;
	template <typename T>
	class pgDictionaryPool;
	class grcTextureStore;
	class netTextMsg;
	class netSyncNodeBase;
	class netSyncDataNode;
	class netSyncData;
	class sysModule;
	class parStructure;
}
