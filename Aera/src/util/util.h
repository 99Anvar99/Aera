#pragma once
#include "pch/pch.h"
#include "memory/pointers.h"
#include "rage/classes.h"
#include "rage/commands/list.h"
#include "fiber/fiber.h"
#include "commands/math.h"
#include "base64.h"

namespace util
{
	namespace classes
	{
		inline CPedFactory* getPedFactory()
		{
			return *pointers::g_pedFactory;
		}

		inline CReplayInterface* getInterface()
		{
			return *pointers::g_replayInterface;
		}

		inline CVehicleInterface* getVehicleInterface()
		{
			return getInterface()->m_vehicle_interface;
		}

		inline CPedInterface* getPedInterface()
		{
			return getInterface()->m_ped_interface;
		}

		inline CPickupInterface* getPickupInterface()
		{
			return getInterface()->m_pickup_interface;
		}

		inline CObjectInterface* getObjectInterface()
		{
			return getInterface()->m_object_interface;
		}

		inline CPed* getPed()
		{
			return getPedFactory()->m_local_ped;
		}

		inline CPlayerInfo* getPlayerInfo()
		{
			return getPed()->m_player_info;
		}

		inline rage::rlGamerInfo* getRlGamerInfo()
		{
			return &getPlayerInfo()->m_gamer_info;
		}

		inline rage::atArray<GtaThread*>& getThreads()
		{
			return *pointers::g_gtaThreads;
		}

		inline rage::scrProgram* getScrProgram(u32 hash)
		{
			return pointers::g_scrProgramTable->find_script(hash);
		}

		inline GtaThread* getGtaThread(u32 hash)
		{
			for (GtaThread* thread : getThreads())
			{
				if (thread->m_script_hash == hash)
				{
					return thread;
				}
			}
			return nullptr;
		}

		inline GtaThread* getGtaThread(const std::string& str)
		{
			if (GtaThread* thread{getGtaThread(stoi(str))})
			{
				return thread;
			}
			if (GtaThread* thread{getGtaThread(stoul(str))})
			{
				return thread;
			}
			std::vector<GtaThread*> results{};
			for (GtaThread* thread : getThreads())
			{
				if (str.find(thread->m_name) != std::string::npos)
				{
					results.push_back(thread);
				}
			}
			if (!results.empty())
			{
				return results[0];
			}
			return nullptr;
		}

		inline void runUnderThread(GtaThread* thread, const std::function<void()>& callback)
		{
			if (!callback)
				return;
			rage::tlsContext* context{rage::tlsContext::get()};
			if (!thread || !thread->m_serialised.m_thread_id)
				return;
			rage::scrThread* originalThread{context->m_script_thread};
			context->m_script_thread = thread;
			context->m_is_script_thread_active = true;
			callback();
			context->m_script_thread = originalThread;
			context->m_is_script_thread_active = originalThread != nullptr;
		}

		inline CGameScriptHandlerNetComponent* getScriptHandlerNetComponet(GtaThread* thr)
		{
			return thr->m_net_component;
		}

		inline rage::CDynamicEntity* getEntityFromSGUID(Entity sguid)
		{
			return pointers::g_handleToPointer(sguid);
		}

		inline Entity getSGUIDFromEntity(rage::CEntity* entity)
		{
			Entity sguid{-1};
			for (rage::fwExtensionContainer* container{entity->m_extension_container}; container; container->m_next)
			{
				if (rage::fwExtension* entry{container->m_entry})
				{
					if (i32 id{static_cast<i32>(entry->get_id())}; id && (id <= sguid || sguid == -1))
					{
						sguid = id;
					}
				}
			}
			return sguid;
		}
	}

	namespace network
	{
		inline CNetworkObjectMgr* getObjectMgr()
		{
			return *pointers::g_networkObjectMgr;
		}

		inline CNetwork* get()
		{
			return *pointers::g_network;
		}

		namespace friends
		{
			inline FriendRegistry* getRegistry()
			{
				return pointers::g_friendRegistry;
			}

			inline FriendList* getList()
			{
				return getRegistry()->m_friend_list;
			}

			inline FriendInfo* getFriends()
			{
				return getList()->m_friends;
			}

			inline u32 getFriendCount()
			{
				return getRegistry()->m_friend_count;
			}

			inline void iterateFriends(std::function<void(u32, bool, FriendInfo*)> cb)
			{
				for (u32 i{}; i != getFriendCount(); ++i)
				{
					if (auto fr{&getFriends()[i]}; fr)
					{
						cb(i, fr->m_is_joinable, fr);
					}
				}
			}
		}

		namespace socialclub
		{
			inline ScInfo* getInfo()
			{
				return pointers::g_scInfo;
			}

			inline std::string getTicket()
			{
				return getInfo()->m_ticket;
			}

			inline std::string authorizationHeader()
			{
				return "Authorization: SCAUTH val=\"" + getTicket() + "\"";
			}

			inline ScGame getInfo(cc* id)
			{
				i32 index{
					pointers::g_scGetGameInfoIndex(id, pointers::g_scGameInfo->GetGamesAddress(),
					                               pointers::g_scGameInfo->m_id)
				};
				u64 address{pointers::g_scGameInfo->GetGamesAddress() + (index * 0x148i64)};
				ScGame game;
				memcpy(game.pad_0000, reinterpret_cast<void*>(address), sizeof(game.pad_0000));
				game.m_string = reinterpret_cast<char*>(address + 0x40);
				memcpy(game.pad_0048, reinterpret_cast<void*>(address), sizeof(game.pad_0048));
				return game;
			}

			inline const char* getString(const char* id)
			{
				ScGame table{getInfo(id)};
				return table.m_string;
			}
		}

		namespace session
		{
			inline rage::snSession* get()
			{
				return network::get()->m_game_session_ptr;
			}

			inline rage::snSession* getTransition()
			{
				return network::get()->m_transition_session_ptr;
			}

			namespace peer
			{
				inline rage::snPeer* getViaConnectionId(uint8_t conId)
				{
					return get()->m_peers[conId];
				}
			}
		}

		inline std::string base64Handle(u64 rid)
		{
			u8 payloadData[16]{};
			for (u64 i{}; i != 4; ++i)
			{
				payloadData[i] = static_cast<u8>((rid >> i) * 8ui64);
			}
			payloadData[8] = 3ui8;
			std::string payload{};
			payload.resize(COUNT(payloadData));
			memcpy(payload.data(), payloadData, payload.size());
			return base64Encode(payload);
		}

		inline bool isHost(u8 index)
		{
			auto player{(*pointers::g_networkPlayerMgr)->m_player_list[index]};
			return player && player->is_valid() && player->is_host();
		}

		inline CNetGamePlayer* getScriptHostNetGamePlayer()
		{
			if (GtaThread* thr{classes::getGtaThread("freemode"_joaat)})
			{
				if (CGameScriptHandlerNetComponent* netComponet{classes::getScriptHandlerNetComponet(thr)})
				{
					return netComponet->get_host();
				}
			}
			return nullptr;
		}

		inline bool deserialiseNetMessage(enum eNetMessage& msg, class rage::datBitBuffer& buffer)
		{
			uint32_t pos;
			uint32_t magic;
			uint32_t length;
			uint32_t extended{};
			if ((buffer.m_flagBits & 2) != 0 || (buffer.m_flagBits & 1) == 0
				    ? (pos = buffer.m_curBit)
				    : (pos = buffer.m_maxBit),
				buffer.m_bitsRead + 15 > pos || !buffer.ReadDword(&magic, 14) || magic != 0x3246 || !buffer.ReadDword(
					&extended, 1))
			{
				msg = eNetMessage::MsgInvalid;
				return false;
			}
			length = extended ? 16 : 8;
			if ((buffer.m_flagBits & 1) == 0 ? (pos = buffer.m_curBit) : (pos = buffer.m_maxBit),
				length + buffer.m_bitsRead <= pos && buffer.ReadDword(reinterpret_cast<uint32_t*>(&msg), length))
				return true;
			return false;
		}
	}

	namespace natives
	{
		inline bool request_model(const u32& hash, high_resolution_clock::duration timeout = 3s)
		{
			if (!STREAMING::IS_MODEL_VALID(hash) || !STREAMING::IS_MODEL_IN_CDIMAGE(hash))
				return false;

			const auto end = high_resolution_clock::now() + timeout;
			while (high_resolution_clock::now() < end)
			{
				if (STREAMING::HAS_MODEL_LOADED(hash))
					return true;

				STREAMING::REQUEST_MODEL(hash);

				fiber::current()->sleep(100ms);
			}

			return false;
		}

		inline bool network_has_control_of_entity(const rage::netObject* net_object)
		{
			return !net_object || !net_object->m_next_owner_id && net_object->m_control_id == -1;
		}

		inline bool request_control(const Entity ent, const int timeout = 300)
		{
			if (!*pointers::g_isSessionActive)
				return true;

			const auto hnd = pointers::g_handleToPointer(ent);
			if (!hnd || !hnd->m_net_object || !*pointers::g_isSessionActive)
				return false;

			if (network_has_control_of_entity(hnd->m_net_object))
				return true;

			for (int i = 0; i < timeout; i++)
			{
				pointers::g_requestControl(hnd->m_net_object);
				if (network_has_control_of_entity(hnd->m_net_object))
					return true;

				if (timeout != 0)
					fiber::current()->sleep();
			}
			return false;
		}

		inline bool forcefullyTakeControl(Entity ent)
		{
			if (*pointers::g_isSessionActive)
			{
				const CNetworkPlayerMgr* player_mgr{*pointers::g_networkPlayerMgr};
				const rage::CDynamicEntity* entity{classes::getEntityFromSGUID(ent)};
				request_control(ent, 0);
				rage::netObject* net_obj{entity->m_net_object};
				const auto obj_mgr = *pointers::g_networkObjectMgr;
				obj_mgr->ChangeOwner(net_obj, player_mgr->m_local_net_player, 0); //Yoink
				return true;
			}
			return false;
		}

		inline void delete_selected_entity(Entity& ent, bool force)
		{
			if (!ENTITY::DOES_ENTITY_EXIST(ent))
			{
				return;
			}

			if (!force && !request_control(ent))
			{
				return;
			}

			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(ent, 7000.f, 7000.f, 15.f, FALSE, FALSE, FALSE);

			if (!ENTITY::IS_ENTITY_A_MISSION_ENTITY(ent))
			{
				ENTITY::SET_ENTITY_AS_MISSION_ENTITY(ent, TRUE, TRUE);
			}

			ENTITY::DELETE_ENTITY(&ent);
		}
	}

	namespace files
	{
		inline fs::path getPath(const fs::path& nonRelative)
		{
			fs::path relative{std::getenv("appdata")};
			relative /= BRAND;
			relative /= nonRelative;
			return relative;
		}

		inline void destory(std::ofstream& file)
		{
			file.clear();
			file = {};
			file.close();
		}

		inline std::ifstream input(fs::path nonRelative)
		{
			return std::ifstream(getPath(nonRelative));
		}

		inline std::ofstream output(fs::path nonRelative)
		{
			return std::ofstream(getPath(nonRelative));
		}

		inline std::ofstream initWithData(fs::path relative, const std::string& data, bool endLine = true)
		{
			std::ofstream stream{output(relative)};
			stream << data;
			if (endLine)
			{
				stream << std::endl;
			}
			return std::move(stream);
		}

		inline std::string read(std::ifstream& file)
		{
			return {(std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()};
		}
	}

	inline void async(const std::function<void()>& callback)
	{
		std::thread(callback).detach();
	}

	inline void delayedThread(bool& running, milliseconds ms, const std::function<void()>& callback)
	{
		async([&]
		{
			while (running)
			{
				if (callback)
					callback();
				std::this_thread::sleep_for(ms);
			}
		});
	}

	inline bool inModuleRegion(cc* module, u64 address)
	{
		if (!address)
			return false;
		static HMODULE hmod{GetModuleHandleA(module ? module : nullptr)};
		static u64 moduleBase{};
		static u64 moduleSize{};
		if (!moduleBase || !moduleSize)
		{
			MODULEINFO info{};
			if (!K32GetModuleInformation(GetCurrentProcess(), hmod, &info, sizeof(info)))
			{
				LOG(Fatal, "GetModuleInformation failed!");
				return true;
			}
			moduleBase = reinterpret_cast<u64>(hmod);
			moduleSize = static_cast<u64>(info.SizeOfImage);
		}
		return address > moduleBase && address < moduleBase + moduleSize;
	}

	inline bool checkIns(cc* module, u64 address, u8 ins)
	{
		if (!inModuleRegion(module, address))
		{
			return false;
		}
		return *reinterpret_cast<u8*>(address) == ins;
	}

	inline bool pressed(i8 key)
	{
		if (GetForegroundWindow() == pointers::g_hwnd)
		{
			if (GetAsyncKeyState(key) & 0x1)
			{
				return true;
			}
		}
		return false;
	}

	inline bool onPress(i8 key, const std::function<void()>& cb = {})
	{
		if (pressed(key))
		{
			if (cb)
			{
				cb();
			}
			return true;
		}
		return false;
	}

	inline void iteratorFilesInPath(const fs::path& path, const std::string& ext,
	                                std::function<void(fs::path, const std::string&)> cb)
	{
		if (exists(path))
		{
			fs::directory_iterator iterator{path.string()};
			for (auto&& entry : iterator)
			{
				if (entry.is_regular_file())
				{
					fs::path path{entry.path()};
					if (path.has_filename() && path.extension() == ext)
					{
						cb(path, path.filename().string());
					}
				}
			}
		}
	}

	template <typename t>
	u64 getPoolObjects(i32 type, i32* arr, i32 arrSize)
	{
		std::vector<uint64_t> objects{};
		t* inf{};
		switch (type)
		{
		case 0:
			{
				inf = static_cast<t*>(classes::getVehicleInterface());
			}
			break;
		case 1:
			{
				inf = static_cast<t*>(classes::getPedInterface());
			}
			break;
		case 2:
			{
				inf = static_cast<t*>(classes::getObjectInterface());
			}
			break;
		case 3:
			{
				inf = static_cast<t*>(classes::getPickupInterface());
			}
			break;
		default: ;
		}
		for (int32_t i{}; i != inf->m_count; ++i)
		{
			if (arrSize == i)
				break;
			objects.push_back(inf->m_list->addr(i));
		}
		for (int32_t i{}; i != objects.size(); ++i)
		{
			arr[i] = classes::getSGUIDFromEntity(reinterpret_cast<rage::CEntity*>(objects[i]));
		}
		return objects.size();
	}

	inline std::wstring strToWstr(const std::string& str)
	{
		return fs::path(str).wstring();
	}

	inline std::string time(const std::string& format)
	{
		char timeBuf[256]{};
		i64 timeSinceEpoch{std::time(nullptr)};
		tm localtime{};
		localtime_s(&localtime, &timeSinceEpoch);
		strftime(timeBuf, sizeof(timeBuf), format.c_str(), &localtime);
		return timeBuf;
	}

	inline constexpr cc* g_advertisementStrings[]{
		"qq",
		"www.",
		".gg",
		". gg",
		".c",
		". c",
		"http",
		"/menu",
		"money/",
		"money\\",
		"--->",
		"shopgta5",
		"<b>",
		"P888",
		"gtacash",
		"trustpilot",
		"\xE5\xBE\xAE\xE4\xBF\xA1", //"wechat" - Chinese
		"<font s",
		"sellix.io",
		"plano inicial", //"initial plan" - Spanish
		"rep +",
		"l55.me",
		"\xE5\xBA\x97", //"shop" - Chinese
		"\xE9\x92\xB1", //"money" - Chinese
		"\xE5\x88\xB7", //"make(money)" - Chinese
		"\xE8\x90\x9D\xE8\x8E\x89", //"cute girl" - Chinese
		"\xE5\xA6\x88", //"mother" - Chinese
		"\xE7\xBE\x8E\xE5\xA5\xB3", //"sexy girl" - Chinese
		"\xE5\xBC\xBA\xE5\xA5\xB8", //"rape" - Chinese
		"\xE8\x90\x9D", //"loli" - Chinese
		"\xE6\x8C\x82", //"hack" - Chinese
		"\xE5\x85\x83", //Yen sign
		//Known advertisers
		"wavy",
		"krutka",
		"itzgoated",
		"warlord",
		"doit#",
		"orangeMmango",
		"faynx",
		"thecashlounge"
	};

	inline bool isSpamMessage(const std::string& message)
	{
		for (auto& string : g_advertisementStrings)
		{
			if (string_to_lower(message).find(string) != std::string::npos)
				return true;
		}
		return false;
	}

	inline bool isInVehicle(CPed* ped, CVehicle* vehicle)
	{
		if (!ped || !vehicle)
			return false;
		if (vehicle->m_driver == ped)
			return true;
		for (u8 i{}; i != 15; ++i)
		{
			if (vehicle->m_passengers[i] == ped)
				return true;
		}
		return false;
	}

	//Not compile time but I'm fucking sick of issues so I'm using what works
	inline std::vector g_badEndpoints{
		"SubmitCompressed",
		"SubmitRealTime"
	};

	inline bool badEndpoint(const std::string& endpoint)
	{
		for (auto& e : g_badEndpoints)
		{
			if (endpoint.find(e) != std::string::npos)
			{
				return true;
			}
		}
		return false;
	}

	class raycast
	{
	public:
		bool check(float distance)
		{
			BOOL hit{};
			Vector3 camCoords{CAM::GET_GAMEPLAY_CAM_COORD()};
			Vector3 camRotation{CAM::GET_GAMEPLAY_CAM_ROT(2)};
			Vector3 dir{math::rotation_to_direction(camRotation)};
			Vector3 dist{camCoords + dir * distance};
			i32 ray{
				SHAPETEST::START_EXPENSIVE_SYNCHRONOUS_SHAPE_TEST_LOS_PROBE(
					camCoords.x, camCoords.y, camCoords.z, dist.x, dist.y, dist.z, -1, ent, 7)
			};
			SHAPETEST::GET_SHAPE_TEST_RESULT(ray, &hit, &end, &surface, &ent);
			return hit;
		}

		Entity entity()
		{
			return ent;
		}

		Vector3& coords()
		{
			return end;
		}

	private:
		Vector3 end{};
		Vector3 surface{};
		Entity ent{};
	};

	enum class eBlipHandleType : i8
	{
		Closest,
		First,
		Next
	};

	class blip
	{
	public:
		blip(i8 sprite, eBlipHandleType handleType) : m_sprite(sprite)
		{
			switch (handleType)
			{
			case eBlipHandleType::Closest:
				{
					m_handle = HUD::GET_CLOSEST_BLIP_INFO_ID(m_sprite);
				}
				break;
			case eBlipHandleType::First:
				{
					m_handle = HUD::GET_FIRST_BLIP_INFO_ID(m_sprite);
				}
				break;
			case eBlipHandleType::Next:
				{
					m_handle = HUD::GET_NEXT_BLIP_INFO_ID(m_sprite);
				}
				break;
			}
			m_type = HUD::GET_BLIP_INFO_ID_TYPE(handle());
			coords() = HUD::GET_BLIP_COORDS(handle());
			alpha() = HUD::GET_BLIP_ALPHA(handle());
			rotation() = HUD::GET_BLIP_ROTATION(handle());
			color() = HUD::GET_BLIP_COLOUR(handle());
			hudColor() = HUD::GET_BLIP_HUD_COLOUR(handle());
		}

		void setBlipCoords(const Vector3& value)
		{
			coords() = value;
			HUD::SET_BLIP_COORDS(handle(), coords().x, coords().y, coords().z);
		}

		void setAlpha(i32 value)
		{
			alpha() = value;
			HUD::SET_BLIP_ALPHA(handle(), alpha());
		}

		void setScale(float value)
		{
			scale() = value;
			HUD::SET_BLIP_SCALE(handle(), scale());
		}

		void setColor(i32 value)
		{
			color() = value;
			HUD::SET_BLIP_COLOUR(handle(), color());
		}

		void remove()
		{
			HUD::REMOVE_BLIP(&handle());
		}

		bool onMinimap()
		{
			return HUD::IS_BLIP_ON_MINIMAP(handle());
		}

		bool exists()
		{
			return HUD::DOES_BLIP_EXIST(handle());
		}

		Blip& handle()
		{
			return m_handle;
		}

		Vector3& coords()
		{
			return m_coords;
		}

		i32& alpha()
		{
			return m_alpha;
		}

		float& scale()
		{
			return m_scale;
		}

		i32& rotation()
		{
			return m_rotation;
		}

		i32& color()
		{
			return m_color;
		}

		i32& hudColor()
		{
			return m_hudColor;
		}

	private:
		Blip m_handle{};
		i8 m_sprite{};
		i32 m_type{};
		Vector3 m_coords{};
		i32 m_alpha{};
		float m_scale{};
		i32 m_rotation{};
		i32 m_color{};
		i32 m_hudColor{};
	};
}
