#pragma once
#include "classdefs.h"
#include "commands/types.h"

class CMoveObjectPooledObject;
class Vector2;
class Vector3;
class Vector4;

namespace rage
{
	template <typename T>
	class atRTTI
	{
		DEFINE_AT_RTTI(T)
	};

	struct vector2
	{
		float x, y;
		Vector2 serialize();
	};

	struct vector3
	{
		float x, y, z;
		Vector2 serialize_v2();
		Vector3 serialize();
	};

	struct vector4
	{
		float x, y, z, w;
		Vector4 serialize();
	};

	template <typename T>
	struct cvector2
	{
		T x, y;
	};

	template <typename T>
	struct cvector3
	{
		T x, y, z;
	};

	template <typename T>
	struct cvector4
	{
		T x, y, z, w;
	};

	union matrix34
	{
		float data[3][4];

		struct
		{
			struct
			{
				float x, y, z, w;
			} rows[3];
		};
	};

	union matrix44
	{
		float data[4][4];

		struct
		{
			struct
			{
				float x, y, z, w;
			} rows[4];
		};
	};
}

/*class CNavigation {
public:
	char pad_0000[32]; //0x0000
	float m_heading; //0x0020
	float m_heading2; //0x0024
	char pad_0028[8]; //0x0028
	rage::vector3 m_rotation; //0x0030
	char pad_003C[20]; //0x003C
	rage::vector3 m_position; //0x0054
}; //Size: 0x0060
static_assert(sizeof(CNavigation) == 0x5C);*/
class CNavigation
{
public:
	char pad_0000[16]; //0x0000
	class rage::phArchetypeDamp* m_damp; //0x0010
	char pad_0018[8]; //0x0018
	rage::matrix44 m_transformation_matrix; //0x0020

	rage::vector3& get_rotation()
	{
		return reinterpret_cast<rage::vector3&>(m_transformation_matrix.rows[2]);
	}

	rage::vector3& get_position()
	{
		return reinterpret_cast<rage::vector3&>(m_transformation_matrix.rows[3]);
	}

	float& get_heading()
	{
		return m_transformation_matrix.data[0][0];
	}

	float& get_heading2()
	{
		return m_transformation_matrix.data[0][1];
	}

	void model_to_world(const rage::vector3& model_coords, rage::vector3& world_coords)
	{
		world_coords.x = model_coords.x * m_transformation_matrix.data[0][0] + model_coords.y * m_transformation_matrix.
			data[1][0] + model_coords.z * m_transformation_matrix.data[2][0] + m_transformation_matrix.data[3][0];
		world_coords.y = model_coords.x * m_transformation_matrix.data[0][1] + model_coords.y * m_transformation_matrix.
			data[1][1] + model_coords.z * m_transformation_matrix.data[2][1] + m_transformation_matrix.data[3][1];
		world_coords.z = model_coords.x * m_transformation_matrix.data[0][2] + model_coords.y * m_transformation_matrix.
			data[1][2] + model_coords.z * m_transformation_matrix.data[2][2] + m_transformation_matrix.data[3][2];
	}
}; //Size: 0x0060
static_assert(sizeof(CNavigation) == 0x60);

namespace rage
{
#pragma optimize("", off)
	__declspec(align(16)) class scrVector
	{
	public:
		scrVector()
		{
		}

		scrVector(float x, float y, float z) : x(x), y(y), z(z)
		{
		}

		float x{}, y{}, z{};
	};
#pragma optimize("", on)
	class netLoggingInterface
	{
	public:
	};

	class nonPhysicalPlayerDataBase
	{
	public:
		virtual ~nonPhysicalPlayerDataBase() = default; //0 (0x00)
		virtual void unk_008() = 0; //1 (0x08)
		virtual void unk_010() = 0; //2 (0x10)
		virtual void unk_018() = 0; //3 (0x18)
		virtual void log(netLoggingInterface* logger) = 0; //4 (0x20)
	};

	union netAddress
	{
		uint32_t m_packed; //0x0000
		struct
		{
			uint8_t m_field4; //0x0000
			uint8_t m_field3; //0x0001
			uint8_t m_field2; //0x0002
			uint8_t m_field1; //0x0003
		};
	};

	static_assert(sizeof(netAddress) == 0x04);

	class netSocketAddress
	{
	public:
		union netAddress m_ip; //0x0000
		uint16_t m_port; //0x0004
	};

	static_assert(sizeof(netSocketAddress) == 0x08);
#pragma pack(push, 8)
	class rlGamerHandle
	{
	public:
		rlGamerHandle(uint64_t rockstar_id, uint8_t platform) : m_rockstar_id(rockstar_id), m_platform(platform),
		                                                        m_flag(0)
		{
		}

		rlGamerHandle(uint64_t rockstar_id) : rlGamerHandle(0, 3)
		{
		}

		rlGamerHandle() : rlGamerHandle(0)
		{
		}

	public:
		uint64_t m_rockstar_id; //0x0000
		uint8_t m_platform; //0x0008
		uint8_t m_flag; //0x0009
	public:
		bool deserialize(datBitBuffer& buf);
		bool serialize(datBitBuffer& buf);
	}; //Size: 0x0010
	static_assert(sizeof(rlGamerHandle) == 0x10);
#pragma pack(pop)
#pragma pack(push, 8)
	class rlPeerInfo
	{
	public:
		char m_certificate[96]; //0x0000
		uint64_t m_peer_id; //0x0060
		class rlGamerHandle m_gamer_handle; //0x0068
		char m_aes_key[32]; //0x0078
		uint8_t unk_0098; //0x0098
		class netSocketAddress m_unk_address; //0x0099
		class netSocketAddress m_public_address; //0x00A9
		class netSocketAddress m_relay_address; //0x00A1
		class netSocketAddress m_local_address; //0x00B1
		uint32_t unk_00B9; //0x00B9
	}; //Size: 0x00C0
	static_assert(sizeof(rlPeerInfo) == 0xC0);
#pragma pack(pop)
#pragma pack(push, 8)
	class rlGamerInfo : public rlPeerInfo
	{
	public:
		uint64_t m_peer_address; //0x00C0
		union
		{
			uint64_t m_platform_data; //0x00C8
			rlGamerHandle m_unk_gamer_handle; //0x00D0
			//As of ov1.67, these aren't always in all instances.
			//Some classes contain m_platform_data, and vice versa
		};

		uint32_t m_ros_privilege; //0x00E0
		char m_name[17]; //0x00E4
	}; //Size: 0x00F0
	static_assert(sizeof(rlGamerInfo) == 0xF0);
#pragma pack(pop)
#pragma pack(push, 8)
	class netPlayer
	{
	public:
		virtual void* _0x00();
		virtual void* _0x08();
		virtual uint32_t _0x10();
		virtual netPlayer* _0x18(void*);
		virtual bool _0x20(void*);
		virtual bool _0x28(void**);
		virtual void destructor();
		virtual void reset();
		virtual bool is_valid();
		virtual const char* get_name();
		virtual void _0x50();
		virtual bool is_host();
		virtual rlGamerInfo* get_net_data();
		virtual void _0x68();

		char pad_0008[8]; //0x0008
		CNonPhysicalPlayerData* m_non_physical_player; //0x0010
		uint32_t m_msg_id; //0x0018
		char pad_001C[4]; //0x001C
		uint8_t m_active_id; //0x0020
		uint8_t m_player_id; //0x0021
		char pad_0022[3]; //0x0022
		uint16_t m_complaints; //0x0026
		char pad_0027[17]; //0x0028
		class CNetGamePlayer* m_unk_net_player_list[10]; //0x0040
		char pad_0090[4]; //0x0090
		uint64_t pad_0098; //0x0098
	}; //Size: 0x00A0
	static_assert(sizeof(netPlayer) == 0xA0);
#pragma pack(pop)
#pragma pack(push, 8)
	class netPlayerMgrBase
	{
	public:
		virtual ~netPlayerMgrBase();
		virtual void Initialize();
		virtual void Shutdown();
		virtual void unk_0018();
		virtual class CNetGamePlayer* RawAddPlayer(void* a1, void* a2, void* a3, class rlGamerInfo* gamer_info,
		                                           class CNonPhysicalPlayerData* non_physical_player_data);
		virtual void RemovePlayer(class CNetGamePlayer* net_game_player);
		virtual void UpdatePlayerListsForPlayer(class CNetGamePlayer* net_game_player);
		virtual class CNetGamePlayer* AddPlayer(void* a1, void* a2, void* a3, class rlGamerInfo* gamer_info,
		                                        class CNonPhysicalPlayerData* non_physical_player_data);

		char pad_0008[8]; //0x0008
		uint64_t* m_network_bandwidth_manager; //0x0010
		char pad_0018[216]; //0x0018
		class CNetGamePlayer* m_local_net_player; //0x00E8
		char pad_00F0[144]; //0x00F0
		class CNetGamePlayer* m_player_list[32]; //0x0180
		uint16_t m_player_limit; //0x0280
		char pad_0282[10]; //0x0282
		uint16_t m_player_count; //0x028C
		char pad_0290[1618]; //0x0290
	}; //Size: 0x08E8
	static_assert(sizeof(netPlayerMgrBase) == 0x8E8);
#pragma pack(pop)
#pragma pack(push, 4)
	class fwDrawData
	{
	public:
		uint64_t unk_000; //0x0000
		uint64_t unk_008; //0x0008
		char pad_0010[8]; //0x0010
		uint32_t dword18; //0x0018
		uint32_t unk_001C; //0x0028
		uint64_t unk_0020; //0x0028
		uint32_t unk_0028; //0x0028
	}; //Size: 0x002C
	static_assert(sizeof(fwDrawData) == 0x2C);
#pragma pack(pop)
	class pgBase
	{
	public:
		virtual ~pgBase() = default;

	private:
		void* m_pgunk;
	};

	class datBase
	{
	public:
		virtual ~datBase() = default;
	};

	class pgBase;

	template <typename T, typename Base>
	class atDNode : public Base
	{
	public:
		T m_data;
		atDNode<T, Base>* m_next;
	};

	template <typename Node>
	class atDList
	{
	public:
		Node* m_head;
		Node* m_tail;
	};

	template <typename T>
	class fwRefAwareBaseImpl : public T
	{
	private:
		void* m_ref; //0x0008
	};

	class fwRefAwareBase : public fwRefAwareBaseImpl<datBase>
	{
	public:
	};

	static_assert(sizeof(fwRefAwareBase) == 0x10);

	class fwExtension
	{
	public:
		virtual ~fwExtension() = default;
		virtual void unk_008() = 0;
		virtual void unk_010() = 0;
		virtual uint32_t get_id() = 0;
	}; //Size: 0x0008
	static_assert(sizeof(fwExtension) == 0x8);

	class fwExtensionContainer
	{
	public:
		fwExtension* m_entry; //0x0000
		fwExtensionContainer* m_next; //0x0008
	}; //Size: 0x0010
	static_assert(sizeof(fwExtensionContainer) == 0x10);

	class fwExtensibleBase : public fwRefAwareBase
	{
	public:
		virtual bool is_of_type(uint32_t hash) = 0;
		virtual const uint32_t& get_type() = 0;
		fwExtensionContainer* m_extension_container; //0x0010
		void* m_extensible_unk; //0x0018
		template <typename T>
		bool is_of_type()
		{
			static auto name = (typeid(T).name()) + 6; //Skip "class "
			static auto name_hash = joaat(name);
			return is_of_type(name_hash);
		}
	}; //Size: 0x0020
	static_assert(sizeof(fwExtensibleBase) == 0x20);
#pragma pack(push, 8)
	class fwArchetypeDef
	{
	public:
		virtual ~fwArchetypeDef() = 0;
		virtual int GetTypeIdentifier() = 0;

		float m_lod_dist; //0x0008
		uint32_t m_flags; //0x000C
		uint32_t m_special_attribute; //0x0010
		char pad_0014[12]; //0x0014
		vector4 m_bounding_box_min; //0x0020
		vector4 m_bounding_box_max; //0x0030
		vector4 m_bounding_sphere_center; //0x0040
		float m_bounding_sphere_radius; //0x0050
		float m_hd_texture_dist; //0x0054
		uint32_t m_name_hash; //0x0058
		uint32_t m_texture_dictionary; //0x005C
		uint32_t m_clip_dictionary_hash; //0x0060
		uint32_t m_drawable_dictionary_hash; //0x0064
		uint32_t m_physics_dictionary_hash; //0x0068
		enum eAssetType : uint32_t
		{
			Uninitialized = 0,
			Fragment = 1,
			Drawable = 2,
			DrawableDictionary = 3,
			Assetless = 4,
		} m_asset_type; //0x006C
		uint32_t m_asset_name_hash; //0x0070
		uint64_t* m_extensions; //0x0078
		uint16_t unk_0080; //0x0080
		char pad_0082[12]; //0x0082
	}; //Size: 0x0090
	static_assert(sizeof(fwArchetypeDef) == 0x90);
#pragma pack(pop)
#pragma pack(push, 8)
	class fwArchetype : public datBase
	{
	public:
		virtual void Initialize() = 0;
		virtual void InitializeFromArchetypeDef(uint32_t mapTypeStoreIdx, fwArchetypeDef* archetypeDef, bool) = 0;
		virtual fwEntity* CreateEntity() = 0;

		char pad_0008[16]; //0x0008
		uint32_t m_hash; //0x0018
		char unk_001C[4]; //0x001C
		vector3 m_bounding_sphere_center; //0x0020
		float m_bounding_sphere_radius; //0x002C
		vector3 m_aabbMin; //0x0030
		float m_lod_dist; //0x003C
		vector3 m_aabbMax; //0x0040
		float m_hd_texture_dist; //0x004C
		uint32_t m_flags; //0x0050
		char unk_0054[4]; //0x0054
		uint64_t unk_0058; //0x0058
		char unk_0060[4]; //0x0060
		uint32_t m_asset_index; //0x0064
		uint16_t unk_0068; //0x0068
		uint16_t unk_006A; //0x006A
	};

	static_assert(sizeof(fwArchetype) == 0x70);
#pragma pack(pop)
#pragma pack(push, 8)
	class CBaseModelInfo : public fwArchetype
	{
	public:
		char pad_0070[8]; //0x0070
		uint64_t unk_0078; //0x0078
		uint64_t unk_0080; //0x0080
		char pad_0088[8]; //0x0088
		uint64_t unk_0090; //0x0090
		char pad_0098[5]; //0x0098
		uint8_t m_model_type; //0x009D
		char pad_009E[6]; //0x009E
		uint64_t unk_00A8; //0x00A8
		uint8_t get_type()
		{
			return m_model_type & 0xFF;
		}

		bool is_type(eModelType type)
		{
			return get_type() == static_cast<uint8_t>(type);
		}

		bool of_any_type(std::vector<eModelType> types)
		{
			for (eModelType& type : types)
			{
				if (is_type(type))
				{
					return true;
				}
			}
			return false;
		}
	}; //Size: 0x00B0
	static_assert(sizeof(CBaseModelInfo) == 0xB0);
#pragma pack(pop)
	class phBoundBase : public pgBase
	{
	public:
	}; //Size: 0x0010
	static_assert(sizeof(phBoundBase) == 0x10);
#pragma pack(push, 4)
	class phBound : public phBoundBase
	{
	public:
		eBoundType m_type; //0x0010
		uint8_t m_flags; //0x0011
		uint16_t m_part_index; //0x0012
		float m_radius_around_centroid; //0x0014
		char pad_0018[8]; //0x0018
		vector4 m_bounding_box_max_xyz_margin_w; //0x0020
		vector4 m_bounding_box_min_xyz_ref_count_w; //0x0030
		vector4 m_centroid_offset_xyz_material_id_0_w; //0x0040
		vector4 m_cg_offset_xyz_material_id_1_w; //0x0050
		vector4 m_volume_distribution; //0x0060
	}; //Size: 0x0070
	static_assert(sizeof(phBound) == 0x70);
#pragma pack(pop)
#pragma pack(push, 1)
	class phBoundCapsule : public phBound
	{
	public:
		float m_capsule_half_height; //0x0070
		uint64_t unk_0074; //0x0074
		uint32_t unk_007C; //0x007C
	}; //Size: 0x0080
	static_assert(sizeof(phBoundCapsule) == 0x80);
#pragma pack(pop)
#pragma pack(push, 8)
	class phBoundComposite : public phBound
	{
	public:
		phBound** m_bounds; //0x0070
		matrix34* m_current_matrices; //0x0078
		matrix34* m_last_matrices; //0x0080
		vector3* unk_0088; //0x0088
		uint32_t* m_type_and_include_flags; //0x0090
		uint32_t* m_owned_type_and_include_flags; //0x0098
		uint16_t m_max_num_bounds; //0x00A0
		uint16_t m_num_bounds; //0x00A2
		char pad_00A4[4]; //0x00A4
		void* unk_00A8; //0x00A8
	}; //Size: 0x00B0
	static_assert(sizeof(phBoundComposite) == 0xB0);
#pragma pack(pop)
	class phArchetype
	{
	public:
		char pad_0000[32]; //0x0000
		class phBound* m_bound; //0x0020
		char pad_0028[16]; //0x0028
	}; //Size: 0x0038
	static_assert(sizeof(phArchetype) == 0x38);

	class phArchetypePhys : public phArchetype
	{
	public:
		char pad_0038[28]; //0x0028
		float m_water_collision; //0x0054
		char pad_0058[40]; //0x0058
	}; //Size: 0x0080
	static_assert(sizeof(phArchetypePhys) == 0x80);

	class phArchetypeDamp : public phArchetypePhys
	{
	public:
		char pad_0080[96]; //0x0080
	}; //Size: 0x00E0
	static_assert(sizeof(phArchetypeDamp) == 0xE0);
#pragma pack(push, 1)
	class fwEntity : public fwExtensibleBase
	{
	public:
		DEFINE_AT_RTTI(fwEntity)
		virtual void* _0x38(void*, void*) = 0;
		virtual void AddExtension(void* extension) = 0; //0x40
		virtual void _0x48() = 0; //not implemented
		virtual void _0x50() = 0; //only implemented by CEntityBatch
		virtual void _0x58() = 0;
		virtual void SetModelInfo(std::uint16_t* model_index) = 0; //0x60
		virtual void _0x68(int, vector4*) = 0;
		virtual void* _0x70(int) = 0;
		virtual CNavigation* GetNavigation() = 0; //0x78
		virtual CMoveObjectPooledObject* CreateMoveObject() = 0; //0x80
		virtual std::uint32_t* GetType() = 0; //0x88
		virtual void _0x90() = 0;
		virtual float _0x98() = 0;
		virtual bool TryRequestInverseKinematics(crmtRequestPose* pose, crmtRequestIk* ik) = 0;
		//0xA0 implemented only by CPed
		virtual bool TryRequestFacialAnims(void*) = 0; //0xA8 implemented only by CPed
		virtual void* _0xB0() = 0;
		virtual std::uint8_t _0xB8() = 0; //implemented only by CPed
		virtual crFrameFilter* GetFrameFilter() = 0; //0xC0
		virtual fwAudEntity* GetEntityAudio() = 0; //0xC8
		virtual void _0xD0() = 0;
		virtual void SetTransform(matrix44* matrix, bool update_pos) = 0; //0xD8
		virtual void SetTransform2(matrix44* matrix, bool update_pos) = 0; //0xE0
		virtual void SetPosition(vector4* pos, bool update_pos) = 0; //0xE8
		virtual void SetHeading(float heading, bool update_pos) = 0; //0xF0
		virtual void SetEntityTypeFlags() = 0; //0xF8
		virtual void _0x100() = 0; //not implemented
		virtual void UpdatePhysics(CNavigation* navigation) = 0; //0x108
		virtual void UpdatePhysics2(CNavigation* navigation) = 0; //0x110
		virtual void UpdatePosition() = 0; //0x118

		class CBaseModelInfo* m_model_info; //0x0020
		uint8_t m_entity_type; //0x0028
		char unk_0029; //0x0029
		uint16_t unk_002A; //0x002A
		uint32_t m_entity_flags; //0x002D
		class CNavigation* m_navigation; //0x0030
		uint16_t unk_0038; //0x0038
		uint16_t unk_003A; //0x003A
		uint32_t unk_003C; //0x003C
		class fwDynamicEntityComponent* m_dynamic_entity_component; //0x0040 (stores attachments and stuff)
		class fwDrawData* m_draw_data; //0x0048
		class fwDynamicEntityComponent* unk_0050; //0x0050
		uint64_t unk_0058; //0x0058
		matrix44 m_transformation_matrix; //0x0060
		fwEntity* m_render_focus_entity; //0x00A0
		uint32_t m_render_focus_distance; //0x00A8
		uint32_t m_flags_2; //0x00AC
		uint32_t m_shadow_flags; //0x00B0
		uint32_t m_lod; //0x00B4
		uint8_t byteB8; //0x00B8

		phBound* getPhBound()
		{
			return m_navigation->m_damp->m_bound;
		}

		phBoundComposite* getPhBoundComposite()
		{
			return reinterpret_cast<phBoundComposite*>(getPhBound());
		}

		void set_entity_flag(eEntityFlags flag, bool value)
		{
			value ? m_entity_flags |= static_cast<uint32_t>(flag) : m_entity_flags &= ~static_cast<uint32_t>(flag);
		}

		float& get_heading()
		{
			return m_navigation->m_transformation_matrix.data[0][0];
		}

		void set_heading(float v)
		{
			get_heading() = v;
		}

		vector3& get_position()
		{
			return reinterpret_cast<vector3&>(m_transformation_matrix.rows[3]);
		}

		vector4& get_position_full()
		{
			return reinterpret_cast<vector4&>(m_transformation_matrix.rows[3]);
		}

		void model_to_world(const vector3& model_coords, vector3& world_coords)
		{
			world_coords.x = model_coords.x * m_transformation_matrix.data[0][0] + model_coords.y *
				m_transformation_matrix.data[1][0] + model_coords.z * m_transformation_matrix.data[2][0] +
				m_transformation_matrix.data[3][0];
			world_coords.y = model_coords.x * m_transformation_matrix.data[0][1] + model_coords.y *
				m_transformation_matrix.data[1][1] + model_coords.z * m_transformation_matrix.data[2][1] +
				m_transformation_matrix.data[3][1];
			world_coords.z = model_coords.x * m_transformation_matrix.data[0][2] + model_coords.y *
				m_transformation_matrix.data[1][2] + model_coords.z * m_transformation_matrix.data[2][2] +
				m_transformation_matrix.data[3][2];
		}
	}; //Size: 0x00B9
	static_assert(sizeof(fwEntity) == 0xB9);
#pragma pack(pop)
	class datBitBuffer
	{
	public:
		datBitBuffer(uint8_t* data, uint32_t size, bool flagBitsToWrite = false)
		{
			m_data = data;
			m_bitOffset = 0;
			m_maxBit = size * 8;
			m_bitsRead = 0;
			m_curBit = 0;
			m_highestBitsRead = 0;
			m_flagBits = 0;
			if (flagBitsToWrite)
			{
				m_flagBits &= 0xFCu;
				m_flagBits &= 0xFDu;
				m_flagBits |= 0xFCu;
			}
			else
			{
				m_flagBits &= ~2u;
				m_flagBits |= 1u;
			}
		}

		datBitBuffer(void* data, uint32_t size, bool flagBitsToWrite = false) : datBitBuffer(
			static_cast<uint8_t*>(data), size, flagBitsToWrite)
		{
		}

		datBitBuffer(bool flagBitsToWrite = false) : datBitBuffer(static_cast<uint8_t*>(nullptr), NULL, flagBitsToWrite)
		{
		}

		bool IsFlagSet(uint8_t flag)
		{
			return (m_flagBits & flag) != NULL;
		}

		uint32_t GetPosition()
		{
			return m_bitsRead;
		}

		uint32_t GetMaxPossibleBit()
		{
			return (m_highestBitsRead + 7) >> 3;
		}

		bool Seek(uint32_t bits)
		{
			if (bits >= 0)
			{
				uint32_t length{IsFlagSet(1) ? m_maxBit : m_curBit};
				if (bits <= length)
				{
					m_bitsRead = bits;
					return true;
				}
			}
			return false;
		}

		int64_t SeekCur(uint32_t bits)
		{
			m_bitsRead += bits;
			if (m_bitsRead > m_curBit)
				m_curBit = m_bitsRead;
			return m_bitsRead;
		}

		int64_t AddNumberOfBits(uint32_t bits)
		{
			m_bitsRead += bits;
			if (m_bitsRead > m_highestBitsRead)
				m_highestBitsRead = m_bitsRead;
			return m_bitsRead;
		}

		uint32_t GetDataLength()
		{
			uint32_t leftoverBit{(m_curBit % 8u) ? 1u : 0u};
			return (m_curBit / 8u) + leftoverBit;
		}

		uint32_t GetMaxDataLength()
		{
			return (m_flagBits & 1) != NULL ? m_maxBit : m_curBit;
		}

		bool EnsureBitData(uint32_t bits)
		{
			if (IsFlagSet(2) || m_bitsRead + bits > GetMaxDataLength())
				return false;
			return true;
		}

		u64 WriteBitsSingle(uint32_t value, int32_t bits);
		uint32_t ReadBitsSingle(uint32_t numBits);

		bool WriteIntegrityPassed()
		{
			WriteBool(false);
			WriteDword(0xCDEF, 0x10);
			return true;
		}

		bool CheckIntegrity()
		{
			bool shouldCheck{};
			bool success{ReadBool(&shouldCheck)};
			if (!shouldCheck)
				return success;
			uint32_t sig{};
			if (ReadDword(&sig, 0x10))
				return sig == 0xCDEF;
			return false;
		}

		template <typename t>
		bool Write(t value, uint32_t bits = 0)
		{
			uint32_t bitsToWrite{bits ? bits : sizeof(t)};
			bool res{WriteBitsSingle(static_cast<uint32_t>(value), bitsToWrite) ? true : false};
			SeekCur(bitsToWrite);
			return res;
		}

		template <typename t>
		void WriteSigned(t value, int32_t bits)
		{
			int32_t sign{value < 0};
			uint32_t signEx{sign ? 0xFFFFFFFF : 0};
			uint32_t d{value ^ signEx};
			Write<int32_t>(1, sign);
			Write<int32_t>(bits - 1, d);
		}

		void WriteFloat(int32_t length, float divisor, float value)
		{
			int32_t max{(1 << length) - 1};
			int32_t integer{static_cast<int32_t>((value / divisor) * max)};
			Write<int32_t>(length, integer);
		}

		void WriteSignedFloat(int32_t length, float divisor, float value)
		{
			int32_t max{(1 << (length - 1)) - 1};
			int32_t integer{static_cast<int32_t>((value / divisor) * max)};
			WriteSigned<int32_t>(length, integer);
		}

		template <typename t>
		t Read(uint32_t bits)
		{
			if (bits <= 32)
			{
				return t(ReadBitsSingle(bits));
			}
			uint32_t Low{ReadBitsSingle(bits - 32u)};
			uint32_t High{ReadBitsSingle(32u)};
			return t(High | (static_cast<uint64_t>(Low) << 32));
		}

		template <typename t>
		t ReadSigned(uint32_t bits)
		{
			int32_t sign{Read<int32_t>(1)};
			int32_t data{Read<int32_t>(bits - 1)};
			return t(static_cast<int64_t>(sign) + (data ^ -sign));
		}

		float ReadFloat(int32_t length, float divisor)
		{
			int32_t integer{Read<int32_t>(length)};
			float max{static_cast<float>((1 << length) - 1)};
			return (static_cast<float>(integer) / max) * divisor;
		}

		float ReadSignedFloat(int32_t length, float divisor)
		{
			int32_t integer{ReadSigned<int32_t>(length)};
			float max{static_cast<float>((1 << (length - 1)) - 1)};
			return (static_cast<float>(integer) / max) * divisor;
		}

		bool ReadPeerId(uint64_t* value)
		{
			if (!EnsureBitData(0x20))
				return false;
			*value = Read<uint64_t>(0x20);
			return true;
		}

		bool WriteString(char* string, uint32_t length)
		{
			uint32_t len{std::min(length, static_cast<uint32_t>(strlen(string)) + 1)};
			bool extended{len > 127};
			WriteBool(extended);
			WriteDword(len, extended ? 15 : 7);
			WriteArray(string, len * 8);
			return true;
		}

		template <uint32_t length>
		bool WriteString(char (&string)[length])
		{
			return WriteString(string, length);
		}

		bool ReadString(char* string, uint32_t length)
		{
			bool extended{Read<bool>(1)};
			uint32_t len{Read<uint32_t>(extended ? 15 : 7)};
			ReadArray(string, len * 8);
			if (string[len - 1] != '\0')
				return false;
			return true;
		}

		template <uint32_t length>
		bool ReadString(char (&string)[length])
		{
			return ReadString(string, length);
		}

		bool ReadBool(bool* boolean)
		{
			if (!EnsureBitData(1))
				return false;
			*boolean = Read<uint8_t>(1);
			return true;
		}

		bool WriteBool(bool value)
		{
			return Write<bool>(value);
		}

		bool ReadByte(uint8_t* integer, int bits)
		{
			if (!EnsureBitData(bits))
				return false;
			*integer = Read<uint8_t>(bits);
			return true;
		}

		bool WriteByte(uint8_t value, int bits)
		{
			return Write<uint8_t>(value, bits);
		}

		bool ReadWord(uint16_t* integer, int bits)
		{
			if (!EnsureBitData(bits))
				return false;
			*integer = Read<uint16_t>(bits);
			return true;
		}

		bool WriteWord(uint16_t value, int bits)
		{
			return Write<uint16_t>(value, bits);
		}

		bool ReadDword(uint32_t* integer, int bits)
		{
			if (!EnsureBitData(bits))
				return false;
			*integer = Read<uint32_t>(bits);
			return true;
		}

		bool WriteDword(uint32_t value, int bits)
		{
			return Write<uint32_t>(value, bits);
		}

		bool ReadInt32(int32_t* integer, int bits)
		{
			if (!EnsureBitData(bits))
				return false;
			*integer = ReadSigned<int32_t>(bits);
			return true;
		}

		bool ReadQword(uint64_t* integer, int bits)
		{
			if (!EnsureBitData(bits))
				return false;
			*integer = Read<uint64_t>(bits);
			return true;
		}

		bool WriteQword(uint64_t value, int bits)
		{
			if (bits <= 32)
			{
				if (IsFlagSet(1))
					return false;
				if ((m_bitsRead + bits) <= m_maxBit)
				{
					if (!IsFlagSet(2))
					{
						Write<uint64_t>(value, bits);
					}
					return true;
				}
				return false;
			}
			if (IsFlagSet(1))
				return false;
			if ((m_bitsRead + 32) > m_maxBit)
				return false;
			if (!IsFlagSet(2))
			{
				Write<uint64_t>(value, 32);
			}
			if (IsFlagSet(1) || (bits - 32) + m_bitsRead > m_maxBit)
			{
				return false;
			}
			if (!IsFlagSet(2))
			{
				Write<uint32_t>(SHIDWORD(value), bits - 32);
			}
			return true;
		}

		bool ReadInt64(int64_t* integer, int32_t bits)
		{
			if (!EnsureBitData(bits))
				return false;
			*integer = ReadSigned<int64_t>(bits);
			return true;
		}

		bool WriteArray(void* array, int32_t size);
		bool ReadArray(void* array, int32_t size);

	public:
		uint8_t* m_data; //0x0000
		uint32_t m_bitOffset; //0x0008
		uint32_t m_maxBit; //0x000C
		uint32_t m_bitsRead; //0x0010
		uint32_t m_curBit; //0x0014
		uint32_t m_highestBitsRead; //0x0018
		uint8_t m_flagBits; //0x001C
	}; //Size: 0x0020
	static_assert(sizeof(datBitBuffer) == 0x20);

	class netGameEvent
	{
	public:
		virtual ~netGameEvent() = default;
		virtual const char* get_name() { return nullptr; };
		virtual bool is_in_scope(netPlayer* player) { return false; };
		virtual bool time_to_resend(uint32_t time) { return false; };
		virtual bool can_change_scope() { return false; };

		virtual void prepare_data(datBitBuffer* buffer, netPlayer* source_player, netPlayer* target_player)
		{
		};

		virtual void handle_data(datBitBuffer* buffer, netPlayer* source_player, netPlayer* target_player)
		{
		};
		virtual bool decide(netPlayer* source_player, netPlayer* target_player) { return false; };

		virtual void prepare_reply(datBitBuffer* buffer, netPlayer* reply_player)
		{
		};

		virtual void handle_reply(datBitBuffer* buffer, netPlayer* souce_player)
		{
		};

		virtual void prepare_extra_data(datBitBuffer* buffer, bool is_reply, netPlayer* player, netPlayer* player2)
		{
		};

		virtual void handle_extra_data(datBitBuffer* buffer, bool is_reply, netPlayer* player, netPlayer* player2)
		{
		};

		virtual void unk_0060()
		{
		};

		virtual void unk_0068()
		{
		};

		virtual void unk_0070()
		{
		};

		virtual void unk_0078()
		{
		};
		virtual bool operator==(const netGameEvent& other) { return false; };
		virtual bool operator!=(const netGameEvent& other) { return false; };
		virtual bool must_persist() { return false; };
		virtual bool must_persist_when_out_of_scope() { return false; };
		virtual bool has_timed_out() { return false; };

		uint16_t m_id; //0x0008
		bool m_requires_reply; //0x000A
		char pad_000B[5]; //0x000B
		netPlayer* m_source_player; //0x0010
		netPlayer* m_target_player; //0x0018
		uint32_t m_resend_time; //0x0020
		uint16_t unk_0024; //0x0024
		uint8_t unk_0026; //0x0026
		uint8_t unk_0027; //0x0027
		uint32_t unk_0028; //0x0028
		char pad_002C[4]; //0x002C
	}; //Size: 0x0030
	template <typename T>
	class atArray
	{
	public:
		T* begin() { return m_data; }
		T* end() { return m_data + m_size; }
		const T* begin() const { return m_data; }
		const T* end() const { return m_data + m_size; }
		T* data() { return m_data; }
		const T* data() const { return m_data; }
		uint16_t size() const { return m_size; }
		uint16_t capacity() const { return m_capacity; }
		T& operator[](uint16_t index) { return m_data[index]; }
		const T& operator[](uint16_t index) const { return m_data[index]; }

	private:
		T* m_data; //0x0000
		uint16_t m_size; //0x0008
		uint16_t m_capacity; //0x0010
	}; //Size: 0x0010
	static_assert(sizeof(atArray<void*>) == 0x10);

	enum class eThreadState : uint32_t
	{
		running,
		blocked,
		aborted,
		halted,
		reset_instruction_count
	};

#pragma pack(push, 1)
	class tlsContext
	{
	public:
		char gap0[8884];
		uint8_t m_unk_byte; // 0xB4
		char gapB5[3];
		sysMemAllocator* m_allocator; // 0xB8
		sysMemAllocator* m_allocator2; // 0xC0 - Same as 0xB8
		sysMemAllocator* m_allocator3; // 0xC8 - Same as 0xB8
		uint32_t m_console_smth; // 0xD0
		char gapD4[188];
		uint64_t m_unk; // 0x190
		char gap198[1720];
		scrThread* m_script_thread; // 0x858
		bool m_is_script_thread_active; // 0x860

#if _WIN32
		static tlsContext* get()
		{
			return *reinterpret_cast<tlsContext**>(__readgsqword(0x58));
		}
		static tlsContext** get_pointer()
		{
			return reinterpret_cast<tlsContext**>(__readgsqword(0x58));
		}
#endif
	};

	static_assert(sizeof(tlsContext) == 0x2A59);
#pragma pack(pop)

	union scrValue
	{
		int32_t Int;
		uint32_t Uns;
		float Float;
		const char* String;
		scrValue* Reference;
		size_t Any;

		bool operator==(const scrValue& val)
		{
			return Int == val.Int;
		}
	};
#pragma pack(push, 8)
	class scrThread
	{
	public:
		virtual ~scrThread() = default; //0 (0x00)
		virtual eThreadState reset(uint32_t script_hash, void* args, uint32_t arg_count)
		{
			return m_serialised.m_state;
		} //1 (0x08)
		virtual eThreadState run() { return m_serialised.m_state; } //2 (0x10)
		virtual eThreadState tick(uint32_t ops_to_execute) { return m_serialised.m_state; } //3 (0x18)
		virtual void kill()
		{
		} //4 (0x20)
		static scrThread** getPointer()
		{
			auto tls = uint64_t(tlsContext::get());
			return reinterpret_cast<scrThread**>(tls + offsetof(rage::tlsContext, m_script_thread));
		}

		static scrThread* get()
		{
			return tlsContext::get()->m_script_thread;
		}

	public:
#pragma pack(pop)
#pragma optimize("", off)
		class Info
		{
		public:
			Info(scrValue* resultPtr, int parameterCount, scrValue* params) :
				ResultPtr(resultPtr), ParamCount(parameterCount), Params(params), BufferCount(0)
			{
			}

			//Return result, if applicable
			scrValue* ResultPtr;
			//Parameter count
			int ParamCount;
			//Pointer to parameter values
			scrValue* Params;
			int BufferCount;
			scrValue* Orig[4];
			scrVector Buffer[4];

			constexpr void copy_referenced_parameters_out()
			{
				while (BufferCount--)
				{
					Orig[BufferCount][0].Float = Buffer[BufferCount].x;
					Orig[BufferCount][1].Float = Buffer[BufferCount].y;
					Orig[BufferCount][2].Float = Buffer[BufferCount].z;
				}
			}
		};
#pragma optimize("", on)
#pragma pack(push, 8)
		class Serialised
		{
		public:
			uint32_t m_thread_id; //0x0000
			uint32_t m_script_hash; //0x0004
			eThreadState m_state; //0x0008
			uint32_t m_pointer_count; //0x000C
			uint32_t m_frame_pointer; //0x0010
			uint32_t m_stack_pointer; //0x0014
			int32_t m_timer_a; //0x0018
			int32_t m_timer_b; //0x001C
			float m_wait; //0x0020
			int32_t m_min_pc; //0x0024
			int32_t m_max_pc; //0x0028
			char m_tls[36]; //0x002C
			uint32_t m_stack_size; //0x0050
			uint32_t m_catch_pointer_count; //0x0054
			uint32_t m_catch_frame_pointer; //0x0058
			uint32_t m_catch_stack_pointer; //0x005C
			uint32_t m_priority; //0x0060
			uint8_t m_call_depth; //0x0060
			uint8_t unk_0061; //0x0061
			uint16_t unk_0062; //0x0062
			char m_callstack[16]; //0x0064
		} m_serialised; //0x0000
		char unk_0078[48]; //0x00A8
		scrValue* m_stack; //0x00A8
		uint32_t unk_00B0; //0x00B0
		uint32_t m_arg_size; //0x00B4
		uint32_t m_arg_loc; //0x00B8
		uint32_t unk_00BC; //0x00BC
		const char* m_exit_message; //0x00C0
		uint32_t unk_00C8; //0x00C8
		char m_name[64]; //0x00CC
		class scriptHandler* m_handler; //0x010C
		class CGameScriptHandlerNetComponent* m_net_component; //0x0114
	}; //Size: 0x0128
	static_assert(sizeof(scrThread) == 0x128);
#pragma pack(pop)
	using Cmd = void(*)(scrThread::Info*);
#pragma pack(push, 8)
	class scrProgram : public pgBase
	{
	public:
		uint8_t** m_code_blocks; //0x0010
		uint32_t m_hash; //0x0018
		uint32_t m_code_size; //0x001C
		uint32_t m_arg_count; //0x0020
		uint32_t m_local_count; //0x0024
		uint32_t m_global_count; //0x0028
		uint32_t m_native_count; //0x002C
		scrValue* m_local_data; //0x0030
		scrValue** m_global_data; //0x0038
		Cmd* m_natives; //0x0040
		uint32_t m_proc_count; //0x0048
		const char** m_proc_names; //0x0050
		uint32_t m_name_hash; //0x0058
		uint32_t m_ref_count; //0x005C
		const char* m_name; //0x0060
		const char** m_string_heaps; //0x0068
		uint32_t m_string_heap_size; //0x0070
		char m_breakpoints[12]; //0x0074 - rage::atMap<uint8_t>

		bool is_valid() const
		{
			return m_code_size != 0;
		}

		uint32_t get_num_code_pages() const
		{
			return (m_code_size + scrPageMask) >> scrStringShift;
		}

		uint32_t get_string_heap_count() const
		{
			return (m_string_heap_size + scrStringMask) >> scrStringShift;
		}

		uint32_t get_string_heap_chunck_size(uint32_t i) const
		{
			return i == get_string_heap_count() - 1 ? m_string_heap_size - (i << scrStringShift) : scrStringSize;
		}

		uint32_t get_code_page_size(uint32_t i) const
		{
			return i == get_num_code_pages() - 1 ? m_code_size & scrPageMask : scrStringSize;
		}

		uint32_t get_full_code_size() const
		{
			uint32_t numPages{get_num_code_pages()};
			if (numPages == 1)
			{
				--numPages;
			}
			return (numPages * scrStringSize) + (m_code_size & scrPageMask);
		}

		uint64_t get_code_page(uint64_t index) const
		{
			return *reinterpret_cast<uint64_t*>(uintptr_t(this) + offsetof(scrProgram, m_code_blocks)) + (index * 8);
		}

		uint64_t get_string_page(uint64_t index) const
		{
			return *reinterpret_cast<uint64_t*>(uintptr_t(this) + offsetof(scrProgram, m_string_heaps)) + (index * 8);
		}

		uint8_t* get_code_address(uint32_t index) const
		{
			if (index < m_code_size)
				return &m_code_blocks[index >> scrPageShift][index & scrPageMask];
			return nullptr;
		}

		const char* get_string(uint32_t index) const
		{
			if (index < m_string_heap_size)
				return &m_string_heaps[index >> scrStringShift][index & scrStringMask];
			return nullptr;
		}

		uint32_t get_string_page_size(uint32_t i) const
		{
			return i == get_num_code_pages() - 1 ? m_string_heap_size & scrPageMask : scrStringSize;
		}

		Cmd* get_native(Cmd entrypoint)
		{
			for (uint32_t i{}; i != m_native_count; ++i)
			{
				if (m_natives[i] == entrypoint)
				{
					return m_natives + i;
				}
			}
			return nullptr;
		}
	}; //Size: 0x0080
	static_assert(sizeof(scrProgram) == 0x80);
#pragma pack(pop)
	class scrProgramTableEntry
	{
	public:
		scrProgram* m_program; //0x0000
		char pad_0008[4]; //0x0008
		uint32_t m_hash; //0x000C
	}; //Size: 0x0010
	static_assert(sizeof(scrProgramTableEntry) == 0x10);

	class scrProgramTable
	{
	public:
		scrProgramTableEntry* m_data; //0x0000
		char pad_0008[16]; //0x0008
		uint32_t m_size; //0x0018

		scrProgram* find_script(uint32_t hash)
		{
			for (uint32_t i{}; i != m_size; ++i)
			{
				if (m_data[i].m_hash == hash)
				{
					return m_data[i].m_program;
				}
			}
			return nullptr;
		}

		scrProgramTableEntry* begin()
		{
			return m_data;
		}

		scrProgramTableEntry* end()
		{
			return m_data + m_size;
		}
	}; //Size: 0x001C
	class sysMemAllocator : public atRTTI<sysMemAllocator>
	{
	public:
		virtual ~sysMemAllocator() = 0;
		virtual void SetQuitOnFail(bool) = 0;
		virtual void* Allocate(size_t size, size_t align, int subAllocator) = 0;
		virtual void* TryAllocate(size_t size, size_t align, int subAllocator) = 0;
		virtual void Free(void* pointer) = 0;
		virtual void TryFree(void* pointer) = 0;
		virtual void Resize(void* pointer, size_t size) = 0;
		virtual sysMemAllocator* GetAllocator(int allocator) const = 0;
		virtual sysMemAllocator* GetAllocator(int allocator) = 0;
		virtual sysMemAllocator* GetPointerOwner(void* pointer) = 0;
		virtual size_t GetSize(void* pointer) const = 0;
		virtual size_t GetMemoryUsed(int memoryBucket) = 0;
		virtual size_t GetMemoryAvailable() = 0;

	public:
		static sysMemAllocator* UpdateAllocatorValue()
		{
			auto tls = tlsContext::get();
			auto gtaTlsEntry = *reinterpret_cast<sysMemAllocator**>(uintptr_t(tls) + offsetof(
				rage::tlsContext, m_allocator));
			if (!gtaTlsEntry)
				tls->m_allocator = gtaTlsEntry;
			tls->m_allocator2 = gtaTlsEntry;
			return gtaTlsEntry;
		}

		static sysMemAllocator* get()
		{
			auto allocValue = tlsContext::get()->m_allocator;
			if (!allocValue)
				allocValue = UpdateAllocatorValue();
			return allocValue;
		}

		static sysMemAllocator** getPointer()
		{
			auto tls = *(uintptr_t*)__readgsqword(0x58);
			return reinterpret_cast<sysMemAllocator**>(tls + offsetof(rage::tlsContext, m_allocator));
		}

		static sysMemAllocator* getEntry()
		{
			auto tls = *(uintptr_t*)__readgsqword(0x58);
			return *reinterpret_cast<sysMemAllocator**>(tls + offsetof(rage::tlsContext, m_allocator2));
		}

		static sysMemAllocator** getEntryPointer()
		{
			auto tls = *(uintptr_t*)__readgsqword(0x58);
			return reinterpret_cast<sysMemAllocator**>(tls + offsetof(rage::tlsContext, m_allocator2));
		}
	};

	class scriptIdBase
	{
	public:
		virtual ~scriptIdBase() = default;

		virtual void assume_thread_identity(scrThread* thread)
		{
		}

		virtual bool is_valid() { return true; }
		virtual uint32_t* get_hash(uint32_t* out) { return nullptr; }
		virtual uint32_t* get_hash2(uint32_t* out) { return nullptr; }
		virtual const char* get_name() { return ""; }

		virtual void deserialize(datBitBuffer* buffer)
		{
		}

		virtual void serialize(datBitBuffer* buffer)
		{
		}

		virtual uint32_t _0x40() { return 0; }
		virtual uint32_t _0x48() { return 0; }

		virtual void log_information(void* logger)
		{
		}

		virtual void copy_data(scriptIdBase* other)
		{
		}

		virtual bool operator==(scriptIdBase* other) { return this == other; }
		virtual bool _0x68(void*) { return false; }
	}; //Size: 0x0008
	static_assert(sizeof(scriptIdBase) == 0x8);
#pragma pack(push, 1)
	class scriptId : public scriptIdBase
	{
	public:
		uint32_t m_hash; //0x0008
		char m_name[32]; //0x000C
	}; //Size: 0x002C
	static_assert(sizeof(scriptId) == 0x2C);
#pragma pack(pop)
	class scriptResource
	{
	public:
		virtual ~scriptResource() = default;
	};

	class scriptHandler
	{
	public:
		class atDScriptObjectNode : public atDNode<scriptHandlerObject*>
		{
		};

	public:
		virtual ~scriptHandler() = default; // 0 (0x00)
		virtual bool _0x08() = 0; // 1 (0x08)
		virtual void _0x10() = 0; // 2 (0x10)
		virtual void cleanup_objects() = 0; // 3 (0x18)
		virtual scriptId* _0x20() = 0; // 4 (0x20)
		virtual scriptId* get_id() = 0; // 5 (0x28)
		//Returns whether the script handler belongs to a networked script.
		virtual bool is_networked() = 0; // 6 (0x30)
		//Initializes the network component for the script handler.
		virtual void init_net_component() = 0; // 7 (0x38)
		//Deletes the script handler's network component, if it exists.
		virtual void reset_net_component() = 0; // 8 (0x40)
		//Destroys the script handler.
		virtual bool destroy() = 0; // 9 (0x48)
		//Adds the object to the script handler's list of objects.
		virtual void add_object(scriptHandlerObject*, bool is_network, bool is_network_and_scriptcheck) = 0; //10 (0x50)
		//Something related to reservations.
		virtual void _0x58(void*) = 0; //11 (0x58)
		virtual void register_resource(scriptResource*, void*) = 0; //12 (0x60)
		virtual void _0x68() = 0; //13 (0x68)
		virtual void _0x70() = 0; //14 (0x70)
		virtual void _0x78() = 0; //15 (0x78)
		virtual void _0x80() = 0; //16 (0x80)
		virtual void _0x88() = 0; //17 (0x88)
		virtual void _0x90() = 0; //18 (0x90)
		virtual void _0x98() = 0; //19 (0x98)
	public:
		void* unk_0008; //0x0008
		void* unk_0010; //0x0010
		scrThread* m_script_thread; //0x0018
		atDList<atDScriptObjectNode> m_objects; //0x0020
		scriptResource* m_resource_list_head; //0x0030
		scriptResource* m_resource_list_tail; //0x0038
		void* unk_0040; //0x0040
		scriptHandlerNetComponent* m_net_component; //0x0048
		uint32_t unk_0050; //0x0050
		uint32_t unk_0054; //0x0054
		uint32_t unk_0058; //0x0058
		uint32_t unk_005C; //0x005C
	}; //Size: 0x0060
	static_assert(sizeof(scriptHandler) == 0x60);

	class scriptHandlerNetComponent
	{
	public:
		virtual ~scriptHandlerNetComponent() = default;
		virtual bool _0x08(void*) = 0;
		virtual void _0x10(CNetGamePlayer*) = 0; //creates a scriptId?
		virtual void* player_left(CNetGamePlayer* player) = 0;
		virtual void* send_host_migration_event(CNetGamePlayer* player) = 0;
		virtual void* player_joined(void**, void* msg_ctx) = 0;
		virtual void* player_joined_ack(void**, void* msg_ctx) = 0;
		virtual bool _0x38(void*, void*) = 0; //join_script?
		virtual void* _0x40(void*, void*) = 0;
		virtual void* _0x48(void*, void*, void*) = 0;
		virtual void* _0x50(void*, void*) = 0;
		virtual void* _0x58(void*, void*) = 0;
		virtual void* _0x60(void*, void*) = 0;
		virtual void* _0x68(void*, void*) = 0;
		virtual void _0x70(void*, void*) = 0;
		virtual void _0x78(void*, void*) = 0;
		virtual short _0x80(void*, void*) = 0;
		virtual void* _0x88(void*, void*) = 0;
		virtual void* _0x90(void*, void*) = 0; //HOST_MIGRATION_FAILED
		virtual bool _0x98(void*, void*) = 0;
		virtual void* _0xA0(void*, void*) = 0;
		virtual void* _0xA8(void*, void*) = 0;
		virtual short _0xB0(void*, void*) = 0;
		virtual bool register_host_broadcast_data(int* data, int size, char* debugString) = 0;
		virtual bool register_player_broadcast_data(int data, int size, bool sync) = 0;
		virtual bool _0xC8() = 0; //something to do to joining session
		virtual bool _0xD0() = 0;
		virtual bool add_player_to_script(CNetGamePlayer* player, short* outParticipantID, short* outSlot,
		                                  int* outFailReason) = 0;
		virtual bool add_player_to_script_internal(CNetGamePlayer* player, short participantID, short slot) = 0;
		//player aka participant
		virtual bool remove_player_from_script(CNetGamePlayer* player) = 0;
		virtual void* player_left_impl(CNetGamePlayer*, bool) = 0;
		virtual bool do_host_migration(CNetGamePlayer* player, short host_token, bool unk) = 0; //aka _0xF8
		virtual void* leave_from_script() = 0; //calls above function with player = nullptr
		virtual bool _0x108() = 0;
		virtual void* _0x110() = 0;
		virtual bool _0x118() = 0; //related to above function
	public:
		scriptHandler* m_script_handler; //0x08
	};

	class rlSessionInfo
	{
	public:
		uint64_t m_rockstar_id; //0x0000
		uint64_t m_session_token; //0x0008
		class rlPeerInfo m_peer_info; //0x0010
	};

	static_assert(sizeof(rlSessionInfo) == 0xD0);

	class rlSessionByGamerTaskResult
	{
	public:
		rlSessionByGamerTaskResult() : m_gamer_handle(NULL)
		{
		}

		rlGamerHandle m_gamer_handle;
		rlSessionInfo m_session_info;
	};

	class rlPresenceEventInviteAccepted
	{
	public:
		rlPresenceEventInviteAccepted(rlGamerInfo& gamerInfo, rlSessionInfo& sessionInfo, rlGamerHandle& gamerHandle) :
			m_gamer_info(gamerInfo), m_session_info(sessionInfo), m_gamer_handle(gamerHandle)
		{
			m_self = this;
		}

		virtual ~rlPresenceEventInviteAccepted() = default;
		virtual void unk_0008() = 0;

		rlPresenceEventInviteAccepted* m_self; //0x0008
		rlGamerInfo m_gamer_info; //0x0010
		char pad_0100[8]; //0x0100
		rlSessionInfo m_session_info; //0x0108
		rlGamerHandle m_gamer_handle; //0x01D8
	}; //Size: 0x01E8
	static_assert(sizeof(rlPresenceEventInviteAccepted) == 0x1E8);
#pragma pack(push, 1)
	class netSyncTree
	{
	public:
		virtual ~netSyncTree() = default;

		char pad_0008[8]; //0x0008
		netSyncNodeBase* m_next_sync_node; //0x0010
		netSyncNodeBase* m_last_sync_node; //0x0018
		uint32_t m_child_node_count; //0x0020
		uint32_t m_unk_array_count; //0x0024
		char pad_0028[8]; //0x0028
		netSyncNodeBase* m_child_nodes[42]; //0x0030
		uint32_t m_child_node_max_count; //0x0180
		netSyncNodeBase* m_unk_array[32]; //0x0188
		uint32_t m_unk_array_max_count; //0x0288
		char pad_0290[560]; //0x0290
	}; //Size: 0x0030
	static_assert(sizeof(netSyncTree) == 0x4B8);
#pragma pack(pop)
	class netObject : public atRTTI<netObject>
	{
	public:
		int16_t m_object_type; //0x0008
		int16_t m_object_id; //0x000A
		char pad_000C[61]; //0x000C
		int8_t m_owner_id; //0x0049
		int8_t m_control_id; //0x004A
		int8_t m_next_owner_id; //0x004B
		bool m_is_remote; //0x004C
		bool m_wants_to_delete; //0x004D
		char pad_004E[1]; //0x004E
		bool m_should_not_be_deleted; //0x004F
		char pad_0050[32]; //0x0050
		uint32_t m_players_acked; //0x0070
		char pad_0074[116]; //0x0074

		virtual ~netObject() = 0;
		virtual void mov1() = 0;
		virtual void mov2() = 0;
		virtual void m_8() = 0;
		virtual void m_10() = 0;
		virtual void m_18() = 0;
		virtual void* m_20() = 0;
		virtual void m_28() = 0;
		virtual netSyncTree* GetSyncTree() = 0;
		virtual void m_38() = 0;
		virtual void m_40() = 0;
		virtual void m_48() = 0;
		virtual void m_50() = 0;
		virtual void m_58() = 0;
		virtual void m_60() = 0;
		virtual void m_68() = 0;
		virtual void m_70() = 0;
		virtual void m_78() = 0;
		virtual CObject* GetGameObject() = 0;
		virtual void m_88() = 0;
		virtual void m_90() = 0;
		virtual void m_98() = 0;
		virtual int GetObjectFlags() = 0;
		virtual void m_A8() = 0;
		virtual void m_B0() = 0;
		virtual void m_B8() = 0;
		virtual void m_C0() = 0;
		virtual void m_C8() = 0;
		virtual int GetSyncFrequency() = 0;
		virtual void m_D8() = 0;
		virtual void m_E0() = 0;
		virtual void m_E8() = 0;
		virtual void m_F0() = 0;
		virtual void m_F8() = 0;
		virtual void Update() = 0;
		virtual bool m_108_1604() = 0; //added in 1604
		virtual void m_108() = 0;
		virtual void m_110() = 0;
		virtual void m_118() = 0;
		virtual void m_120() = 0;
		virtual void m_128() = 0;
		virtual void m_130() = 0;
		virtual void m_138() = 0;
		virtual void m_140() = 0;
		virtual void m_148() = 0;
		virtual void m_150() = 0;
		virtual bool m_158(void* player, int type, int* outReason) = 0;
		virtual void m_160() = 0;
		virtual bool m_168(int* outReason) = 0;
		virtual void m_170() = 0;
		virtual void m_178() = 0;
		virtual void m_180() = 0;
		virtual void m_188() = 0;
		virtual void m_190() = 0;
		virtual void m_198() = 0;
		virtual void m_1A0() = 0;
		virtual void m_1A8() = 0;
		virtual void m_1B0() = 0;
		virtual void m_1B8() = 0;
		virtual void m_1C0() = 0;
		virtual void m_1C8() = 0;
		virtual void m_1D0() = 0;
		virtual void m_1D8() = 0;
		virtual void m_1E0() = 0;
		virtual void m_1E8() = 0;
		virtual void m_1F0() = 0;
		virtual void m_1F8() = 0;
		virtual void m_200() = 0;
		virtual void m_208() = 0;
		virtual void m_210() = 0;
		virtual void m_218() = 0;
		virtual void m_220() = 0;
		virtual void m_228() = 0;
		virtual void m_230() = 0;
		virtual void m_238() = 0;
		virtual void m_240() = 0;
		virtual void m_248() = 0;
		virtual void m_250() = 0;
		virtual void m_258() = 0;
		virtual void m_260() = 0;
		virtual void m_268() = 0;
		virtual void m_270() = 0;
		virtual void m_278() = 0;
		virtual void m_280() = 0;
		virtual void m_288() = 0;
		virtual void m_290() = 0;
		virtual void m_298() = 0;
		virtual void m_2A0() = 0;
		virtual void m_2A8() = 0;
		virtual void m_2B0() = 0;
		virtual void m_2B8() = 0;
		virtual void m_2C0() = 0;
		virtual void m_2C8() = 0;
		virtual void m_2D0() = 0;
		virtual void m_2D8() = 0;
		virtual void m_2E0() = 0;
		virtual void m_2E8() = 0;
		virtual void m_2F0() = 0;
		virtual void m_2F8() = 0;
		virtual void m_300() = 0;
		virtual void m_308() = 0;
		virtual void m_310() = 0;
		virtual void m_318() = 0;
		virtual void m_320() = 0;
		virtual void UpdatePendingVisibilityChanges() = 0;
	};

	static_assert(sizeof(netObject) == 0xE8);

	class netObjectMgrBase
	{
	public:
		class atDNetObjectNode : public atDNode<netObject*>
		{
		};

	public:
		virtual ~netObjectMgrBase() = default;
		virtual void Initialize() = 0;
		virtual void Shutdown() = 0;
		virtual void Update(bool) = 0;
		virtual void AddEntity(void*, void*) = 0;
		virtual void UnregisterNetworkObject_(void* unk, bool force) = 0;
		virtual void UnregisterNetworkObject(netObject* object, int reason, bool force1, bool force2) = 0;
		virtual void ChangeOwner(netObject* object, CNetGamePlayer* player, u32 migrationType) = 0;
		virtual void HandleJoiningPlayer(CNetGamePlayer* player) = 0;
		virtual void HandleLeavingPlayer(CNetGamePlayer* player) = 0;
		virtual void _0x50(CNetGamePlayer* player) = 0;
		virtual void _0x58() = 0;
		virtual void RegisterNetworkObject(netObject* object) = 0;
		virtual void PackCloneCreate(netObject* object, CNetGamePlayer* player, datBitBuffer* buffer) = 0;
		virtual bool PackCloneRemove(netObject* object, CNetGamePlayer* player, bool) = 0;
		virtual void _0x78(netObject* object, void*) = 0;
		virtual void _0x80() = 0;
		virtual void _0x88() = 0;
		virtual const char* _0x90(int) = 0;
		virtual bool HandleCloneCreate(CNetGamePlayer* source, CNetGamePlayer* target, std::uint16_t object_type,
		                               std::uint16_t object_id, uint16_t object_flags, void*,
		                               std::uint32_t timestamp) = 0;
		virtual void HandleCloneCreateAck(CNetGamePlayer* source, CNetGamePlayer* target, std::uint16_t object_flags,
		                                  uint32_t ack_code) = 0;
		virtual int HandleCloneSync(CNetGamePlayer* source, CNetGamePlayer* target, std::uint16_t object_type,
		                            std::uint16_t object_id, void*, std::uint16_t, std::uint32_t timestamp) = 0;
		virtual void HandleCloneSyncAck(CNetGamePlayer* source, CNetGamePlayer* target, void*, std::uint16_t objectId,
		                                uint32_t ack_code) = 0;
		virtual void HandleCloneRemove(CNetGamePlayer* source, CNetGamePlayer* target, std::uint16_t object_id, int) =
		0;
		virtual void HandleCloneRemoveAck(CNetGamePlayer* source, CNetGamePlayer* target, std::uint16_t object_id,
		                                  uint32_t ack_code) = 0;
		virtual void _0xC8() = 0;

	public:
		atDList<atDNetObjectNode> m_objects_owned_by_player[32];
	};

	class CEntityDrawHandler : public fwDrawData
	{
	public:
	};

	static_assert(sizeof(CEntityDrawHandler) == 0x2C);

	class CEntity : public fwEntity
	{
	public:
		virtual void* _0x120() = 0; //implemented only by CPed
		virtual void UpdatePositionImpl() = 0; //0x128
		virtual void _0x130() = 0;
		virtual void _0x138(void*) = 0;
		virtual void _0x140() = 0;
		virtual void _0x148(int) = 0;
		virtual bool _0x150() = 0;
		virtual CEntityDrawHandler* CreateDrawHandler() = 0; //0x158
		virtual int GetTypeFlags() = 0; //0x160
		virtual int GetTypeFlags2() = 0; //0x168
		virtual bool _0x170() = 0; //implemented only by CPickup
		virtual bool _0x178() = 0;
		virtual void _0x180(bool) = 0;
		virtual bool _0x188() = 0;
		virtual bool _0x190() = 0;
		virtual void ClearDecals() = 0; //0x198
		virtual void GetModelBounds(vector3* bounds) = 0; //0x1A0
		virtual void GetModelBounds2(vector3* bounds) = 0; //0x1A8
		virtual float GetBoundingBoxSize() = 0; //0x1B0
		virtual float _0x1B8(void*) = 0;
		virtual float _0x1C0(void*) = 0;
		virtual vector3* _0x1C8() = 0;
		virtual vector3* GetCameraOffset() = 0; //0x1D0
		virtual void GetCameraBasePosition(vector3* pos) = 0; //0x1D8
		virtual bool _0x1E0() = 0;
		virtual bool Update() = 0; //0x1E8 always returns true
		virtual bool _0x1F0() = 0;
		virtual void Warp(vector3* pos, float heading, bool update_pos) = 0; //0x1F8

		void set_position(vector3 v)
		{
			vector4 v4{v.x, v.y, v.z, 0.f};
			m_transformation_matrix.rows[3].x = v.x;
			m_transformation_matrix.rows[3].y = v.y;
			m_transformation_matrix.rows[3].z = v.z;
			m_navigation->get_position() = v;
			Warp(&v, get_heading(), true);
			SetPosition(&v4, true);
		}

		uint8_t unk_00B9; //0x00B9
		char pad_00BA[6]; //0x00BA
		uint32_t m_flags_3; //0x00C0
		uint32_t m_flags_4; //0x00C4
		uint32_t unk_00C8; //0x00C8
		uint32_t unk_00CC; //0x00CC
	}; //Size: 0x00D0
	static_assert(sizeof(CEntity) == 0xD0);

	class CDynamicEntity : public CEntity
	{
	public:
		class netObject* m_net_object; //0x00D0
		char pad_00D8[16]; //0x00D8
		uint64_t unk_00E8; //0x00E8
	}; //Size: 0x00F0
	static_assert(sizeof(CDynamicEntity) == 0xF0);
#pragma pack(push, 1)
	class CPhysical : public CDynamicEntity
	{
	public:
		char pad_00F0[144]; //0x00F0
		uint64_t unk_0180; //0x0180
		uint32_t m_damage_bits; //0x0188
		uint8_t m_hostility; //0x018C
		char pad_018D[3]; //0x018D
		uint8_t unk_0190; //0x0190
		char pad_0191[3]; //0x0191
		uint32_t unk_0194; //0x0194
		char pad_0198[232]; //0x0198
		float m_health; //0x0280
		float m_maxhealth; //0x0284
		uint64_t unk_0280; //0x0280
		char pad_02B0[72]; //0x0290
		uint64_t unk_02D8; //0x02D8
		uint64_t unk_02E0; //0x02E0
		uint32_t unk_02E8; //0x02E8
	}; //Size: 0x02EC
	static_assert(sizeof(CPhysical) == 0x2EC);
#pragma pack(pop)
	class scriptHandlerMgr
	{
	public:
		virtual ~scriptHandlerMgr();
		virtual bool initialize(); //1
		virtual void _0x10(); //2
		virtual void shutdown(); //3
		virtual void _0x20(); //4
		virtual void _0x28(); //5
		virtual void _0x30(); //6
		virtual void _0x38(scrThread*); //7
		virtual scriptHandler* create_script_handler(); //8
		virtual scriptHandler* get_script_handler(scriptId* id); //9
		virtual void attach_thread(scrThread* thread); //10
		virtual void detach_thread(scrThread* thread); //11
		virtual void on_player_join(netPlayer* player); //12
		virtual void on_player_left(netPlayer* player); //13
		virtual int32_t _0x70();
		virtual void* _0x78();

	public:
		char pad_0008[40]; //0x0008
		bool m_initialized; //0x0030
		bool m_initialized_2; //0x0031
		char pad_0032[14]; //0x0032
		netLoggingInterface* m_logger; //0x0040
	}; //Size: 0x0048
	class scrNativeRegistration
	{
	public:
		scrNativeRegistration* m_next; //0x0000
		scrNativeRegistration* m_previous; //0x0008
		Cmd m_handlers[7]; //0x0010
		uint32_t m_num_entries; //0x0048
		uint32_t m_num_entries_2; //0x004C
		uint64_t m_hashes[7 * 2]; //0x0050

		scrNativeRegistration* get_next_registration()
		{
			uint64_t result{};
			uint64_t next_registration{reinterpret_cast<uint64_t>(&m_next)};
			uint64_t previous_registration{next_registration ^ reinterpret_cast<uint64_t>(m_previous)};
			int8_t* registrations{reinterpret_cast<int8_t*>(&result) - next_registration};
			for (uint8_t i{2}; i; --i)
			{
				*reinterpret_cast<uint32_t*>(&registrations[next_registration]) = static_cast<uint32_t>(
					previous_registration) ^ *reinterpret_cast<uint32_t*>(next_registration);
				next_registration += 4;
			}
			return reinterpret_cast<scrNativeRegistration*>(result);
		}

		uint32_t get_num_entries()
		{
			auto addr = reinterpret_cast<std::uintptr_t>(&m_num_entries);
			return static_cast<uint32_t>(addr ^ m_num_entries ^ m_num_entries_2);
		}

		uint64_t get_hash(uint32_t index)
		{
			uint64_t result{};
			auto native_address{16 * index + reinterpret_cast<uintptr_t>(&m_next) + 0x54};
			auto registrations{reinterpret_cast<char*>(&result) - native_address};
			auto address_index{native_address ^ *reinterpret_cast<uint32_t*>(native_address + 0x8)};
			for (uint8_t i{2}; i; --i)
			{
				*reinterpret_cast<uint32_t*>(&registrations[native_address]) = static_cast<uint32_t>(address_index ^ *
					reinterpret_cast<uint32_t*>(native_address));
				native_address += 4;
			}
			return result;
		}
	}; //Size: 0x00C0
	static_assert(sizeof(scrNativeRegistration) == 0xC0);
#pragma pack(push, 1)
	class scrNativeRegistrationTable
	{
	public:
		scrNativeRegistration* m_entries[255]; //0x0000
		uint32_t m_seed; //0x07F8
		bool m_initialized; //0x07FC

		Cmd get_handler(uint64_t hash)
		{
			uint8_t index{hash & 0xFF};
			for (auto entry{m_entries[index]}; entry; entry = entry->get_next_registration())
			{
				for (uint64_t i{}, end{entry->get_num_entries()}; i < end; ++i)
				{
					if (auto entry_hash = entry->get_hash(static_cast<uint32_t>(i)); entry_hash == hash)
					{
						return entry->m_handlers[i];
					}
				}
			}
			return nullptr;
		}
	}; //Size: 0x07FD
	static_assert(sizeof(scrNativeRegistrationTable) == 0x7FD);
#pragma pack(pop)

	class scrNativeCallContext
	{
	public:
		constexpr void reset()
		{
			m_arg_count = 0;
			m_data_count = 0;
		}

		template <typename T>
		constexpr void push_arg(T&& value)
		{
			static_assert(sizeof(T) <= sizeof(std::uint64_t));
			*reinterpret_cast<std::remove_cv_t<std::remove_reference_t<T>>*>(reinterpret_cast<std::uint64_t*>(m_args) +
				(m_arg_count++)) = std::forward<T>(value);
		}

		template <typename T>
		constexpr T& get_arg(std::size_t index)
		{
			static_assert(sizeof(T) <= sizeof(std::uint64_t));
			return *reinterpret_cast<T*>(reinterpret_cast<std::uint64_t*>(m_args) + index);
		}

		template <typename T>
		constexpr void set_arg(std::size_t index, T&& value)
		{
			static_assert(sizeof(T) <= sizeof(std::uint64_t));
			*reinterpret_cast<std::remove_cv_t<std::remove_reference_t<T>>*>(reinterpret_cast<std::uint64_t*>(m_args) +
				index) = std::forward<T>(value);
		}

		template <typename T>
		constexpr T* get_return_value()
		{
			return reinterpret_cast<T*>(m_return_value);
		}

		template <typename T>
		constexpr void set_return_value(T&& value)
		{
			*reinterpret_cast<std::remove_cv_t<std::remove_reference_t<T>>*>(m_return_value) = std::forward<T>(value);
		}

		void* m_return_value;
		std::uint32_t m_arg_count;
		void* m_args;
		std::int32_t m_data_count;
		std::uint32_t m_data[48];
	};

	static_assert(sizeof(scrNativeCallContext) == 0xE0);

	using scrNativeHash = std::uint64_t;
	using scrNativePair = std::pair<scrNativeHash, scrNativeHash>;
	using scrNativeHandler = void(*)(scrNativeCallContext*);

#pragma pack(push, 1)
	class rlJSON
	{
	public:
		rlJSON(uint32_t length) : m_buffer(new char[length]), m_max_length(length), m_read(TRUE)
		{
		}

		~rlJSON() { delete[] m_buffer; }

	public:
		uint32_t unk_0000{}; //0x0000
		uint32_t unk_0004{}; //0x0004
		char* m_buffer{}; //0x0008
		uint32_t m_current_length{}; //0x0010
		uint32_t m_max_length{}; //0x0014
		uint32_t m_read{}; //0x0018
		uint8_t m_flags{}; //0x001C

		std::string str()
		{
			return m_buffer;
		}

		/*void set(const std::string& str)
		{
			strcpy(m_buffer, str.c_str());
			m_current_length = static_cast<uint32_t>(str.length());
			m_max_length = UINT_MAX;
		}*/

		void clear()
		{
			memset(m_buffer, NULL, m_max_length);
		}
	}; //Size: 0x0020
	static_assert(sizeof(rlJSON) == 0x1D);
#pragma pack(pop)
	class JSONNode
	{
	public:
		char* m_key; //0x0000
		char pad_0008[32]; //0x0008
		class JSONNode* m_sibling; //0x0028
		class JSONNode* m_child; //0x0030
		char* m_value; //0x0038
		char pad_0040[8]; //0x0040
		JSONNode* GetChild(const char* name)
		{
			for (JSONNode* node{m_child}; node; node = node->m_sibling)
			{
				if (!strcmp(name, node->m_key))
				{
					return node;
				}
			}
			return nullptr;
		}
	}; //Size: 0x0048
	static_assert(sizeof(JSONNode) == 0x48);

	class rlMetric
	{
	public:
		virtual ~rlMetric() = default; //Deconstructor
		virtual int metric_b() { return 0; }; //Returns a constant integer like 4, 5, 6
		virtual int metric_a() { return 0; }; //Returns a constant integer like 0
		virtual int unk_0018() { return 0; };
		virtual const char* get_name() { return ""; }; //Short name of the metric
		virtual bool to_json(rlJSON* jsonStream) { return false; }; //Prints the metric out to a JSON stream
		virtual int get_size() { return 0; }; //Size in bytes of derived object (for copying)
		virtual uint32_t get_name_hash() { return 0; }; //Joaat of short name
	public:
		bool using_a()
		{
			return metric_a() == 6;
		}

		bool using_b()
		{
			return metric_b() == 5;
		}

		bool using_c()
		{
			return !using_a() && !using_b();
		}

		bool crc_flag()
		{
			return !(*reinterpret_cast<i8*>(*reinterpret_cast<u64*>(__readgsqword(0x58)) + 0xB4) & 1);
		}
	};

	template <typename T>
    class ObfVar
    {
	    T m_unk1;
        T m_unk2;
        T m_unk3;
        T m_unk4;

    public:
        T getData() const
        {
            T v105 = m_unk4;
            T v28 = m_unk1 & v105;
            T v94 = m_unk2 & ~v105;
            return v28 | v94;
        }

        operator T() const
        {
            return getData();
        }

#if _WIN32
        void setData(T val)
        {
            auto seed = static_cast<T>(time(nullptr)); // Use static_cast to ensure the correct type
            m_unk3 = seed;
            seed = static_cast<T>(time(nullptr));
            m_unk4 = seed;

            T v48 = val & ~seed;
            m_unk1 = seed & val;
            m_unk2 = v48;
        }

        void operator =(T val)
        {
            setData(val);
        }
#endif
    };

	class rlSessionDetail
	{
	public:
		class rlPeerInfo m_peer_info;
		char pad_0060[8]; //0x0060
		class rlSessionInfo m_session_info; //0x0068
		char pad_00D8[14]; //0x00D8
		uint16_t m_session_type; //0x00E6
		char pad_00E8[324]; //0x00E8
		uint32_t m_player_count; //0x022C
		uint32_t m_unk_player_count; //0x0230
		char pad_0234[2]; //0x0234
		int16_t m_unk_pos_x; //0x0236
		int16_t m_unk_pos_y; //0x0238
		int16_t m_unk_pos_z; //0x023A
		uint8_t m_matchmaking_property_ids[32]; //0x023C
		char pad_025C[2]; //0x025C
		uint16_t m_rank; //0x025E
		char pad_0260[1]; //0x0260
		uint8_t m_mental_state; //0x0261
		char pad_0262[21]; //0x0262
		uint8_t m_population_density; //0x0277
		char pad_0278[320]; //0x0278
	}; //Size: 0x03CA
	static_assert(sizeof(rlSessionDetail) == 0x478);

	class rlMatchmakingFindResult
	{
	public:
		class rlSessionDetail m_result_session_details[15]; //0x0000
		char pad_4308[168]; //0x4308
	}; //Size: 0x43B0
	static_assert(sizeof(rlMatchmakingFindResult) == 0x43B0);

	class netGamePlayerData
	{
	public:
		class rlGamerHandle m_handle; //0x0000
		bool m_is_activity_spectator; //0x0010
		char pad_0011[7]; //0x0011
		uint64_t m_crew_id; //0x0018
		uint16_t m_rank; //0x0020
		uint16_t m_debug_unk; //0x0022
		char pad_0024[4]; //0x0024
		uint32_t m_nat_type; //0x0028
		bool m_is_rockstar_dev; //0x002C
		char pad_002D[3]; //0x002D
	}; //Size: 0x0030
	static_assert(sizeof(netGamePlayerData) == 0x30);
#pragma pack(push, 4)
	class netPeerAddress
	{
	public:
		uint32_t m_internal_ip; //0x0000
		uint16_t m_internal_port; //0x0004
		uint32_t m_external_ip; //0x0008
		uint16_t m_external_port; //0x000C
		uint64_t m_peer_id; //0x0010
		uint32_t unk_0018; //0x0018
		uint16_t unk_001C; //0x001C
		uint8_t m_platform; //0x001
	}; //Size: 0x0020
	static_assert(sizeof(netPeerAddress) == 0x20);
#pragma pack(pop)
#pragma pack(push, 1)
	class netQueuedMessage
	{
	public:
		uint8_t* m_data_buffer; //0x0000
		void* unk_0008; //0x0008
		void* unk_0010; //0x0010
		void* unk_0018; //0x0018
		netQueuedMessage* m_next; //0x0020
		void* unk_0028; //0x0028
		char unk_0030[8]; //0x0030
		int m_creation_time; //0x0038
		int m_last_send_time; //0x003C
		int m_resend_count; //0x0040
		char pad_0044[4]; //0x0044
		int unk_0048; //0x0048
		uint16_t unk_004C; //0x004C
		char unk_004E; //0x004E
	}; //Size: 0x004F
	static_assert(sizeof(netQueuedMessage) == 0x4F);
#pragma pack(pop)
#pragma pack(push, 1)
	class netMessageQueue
	{
	public:
		netQueuedMessage* m_first; //0x0000
		netQueuedMessage* m_last; //0x0008
		uint64_t m_count; //0x0010
	}; //Size: 0x0018
#pragma pack(pop)
#pragma pack(push, 1)
	class netPackedMessage
	{
	public:
		uint8_t* m_data_buffer; //0x0000
		sysMemAllocator* m_allocator; //0x0008
		void* unk_0010; //0x0010
		void* unk_0018; //0x0018
	}; //Size: 0x0020
#pragma pack(pop)
	class netPackedMessageQueue
	{
	public:
		netPackedMessage* m_first; //0x0000
		netPackedMessage* m_last; //0x0008
		uint64_t m_count; //0x0010
	};
#pragma pack(push, 1)
	class netConnection
	{
	public:
		class InFrame
		{
		public:
			virtual ~InFrame() = default;
			virtual void Destroy() = 0;
			virtual uint32_t GetId() = 0;
			virtual uint32_t GetPayloadSize() = 0;
			virtual bool IsChanneled() = 0;

			uint32_t m_timestamp; //0x0008
			char pad_0008[52]; //0x000C
			uint32_t m_msg_id; //0x0040
			uint32_t m_connection_identifier; //0x0044
			InFrame* m_this; //0x0048
			uint32_t m_peer_id; //0x0050
			char pad_0058[44]; //0x0058
			uint32_t m_length; //0x0080
			char pad_0084[4]; //0x0084
			uint8_t* m_data; //0x0088
		}; //Size: 0x0090
		static_assert(sizeof(InFrame) == 0x90);
		char pad_0000[8]; //0x0000
		netConnectionPeer* m_connection_peer; //0x0008
		int m_msg_id; //0x0010
		uint32_t m_connection_id; //0x0014
		sysMemAllocator* m_allocator; //0x0018
		uint32_t m_connection_state; //0x0020
		uint32_t m_last_send_time; //0x0024
		uint32_t m_last_receive_time; //0x0028
		uint32_t m_num_failed_messages; //0x002C
		char pad_0030[8]; //0x0030
		uint32_t m_timeout_reason; //0x0038
		uint32_t unk_003C; //0x003C
		uint32_t m_timeout; //0x0040
		uint32_t unk_0044; //0x0044
		uint32_t m_resend_threshold; //0x0048
		char unk_004C[4]; //0x004C
		netMessageQueue m_reliables_resend_queue; //0x0050
		netMessageQueue m_normal_message_queue; //0x0068
		netQueuedMessage* m_unacked_reliable_message_list; //0x0080
		int m_unacked_reliable_message_count; //0x0088
		char pad_008C[36]; //0x008C
		netConnectionManager* m_net_connection_mgr; //0x00B0
		char pad_00B8[8]; //0x00B8
		uint32_t pad_00C0; //0x00C0
		int16_t m_msg_counter; //0x00C4
		int16_t unk_00C6; //0x00C6
		char pad_00C8[2]; //0x00C8
		int16_t m_last_reliable_msg_counter; //0x00CB
		char m_flags; //0x00CC
		char pad_00CD[3]; //0x00CD
		int m_failed_allocation_size; //0x00D0
		int32_t m_failed_allocations; //0x00D4
		netConnection* m_next; //0x00D8
		char pad_00E0[208]; //0x00E0
		int m_flags2; //0x01B0
		char pad_01B4[69]; //0x01B4
	}; //Size: 0x1F9
#pragma pack(pop)
	class snPlayer
	{
	public:
		uint64_t m_msg_id; //0x0000
		class rlGamerInfo m_gamer_info; //0x0008
		char pad_00F8[8]; //0x00F8
	}; //Size: 0x00100
	static_assert(sizeof(snPlayer) == 0x100);

	class snPeer
	{
	public:
		class rlGamerInfo m_gamer_data; //0x0000
		char pad_0098[40]; //0x0098
	}; //Size: 0x0118
	static_assert(sizeof(snPeer) == 0x118);

	class rlRemoteGamer
	{
	public:
		rlGamerHandle m_handle;
		char pad_0010[4]; //0x0010
		uint32_t m_timeout_time; //0x0014
		uint32_t m_time_unk; //0x0018
		char pad_001C[4]; //0x001C
	}; //Size: 0x0020
	static_assert(sizeof(rlRemoteGamer) == 0x20);

	class rlTaskStatus
	{
	public:
		int m_state; //0x0000
		int m_unk; //0x0004
	}; //Size: 0x0008
	static_assert(sizeof(rlTaskStatus) == 0x8);

	class rlSession
	{
	public:
		virtual ~rlSession() = default;

		char pad_0008[248]; //0x0008
		class rlSessionInfo m_session_info; //0x0100
		char pad_01D0[296]; //0x01D0
		uint64_t m_session_id; //0x02F8
		char pad_0300[1136]; //0x0300
	}; //Size: 0x770
	static_assert(sizeof(rlSession) == 0x770);

	class snSession
	{
	public:
		uint64_t m_memory_allocator; //0x0000
		char pad_0008[64]; //0x0008
		netConnectionManager* m_net_connection_mgr; //0x0048
		char pad_0050[48]; //0x0050
		class rlSession m_rline_session; //0x0080
		class snPlayer m_local_player; //0x07F0
		uint64_t m_host_token; //0x08F0
		char pad_08F8[144]; //0x08F8
		class snPeer m_peer_storage[32]; //0x0988
		char pad_2C88[24]; //0x2C88
		class snPeer* m_peers[32]; //0x2CA0
		uint32_t m_peer_count; //0x2DA0
		char pad_2DA4[4]; //0x2DA4
		class snPlayer m_player_storage[32]; //0x2DA8
		char pad_4DA8[24]; //0x4DA8
		class snPlayer* m_players[32]; //0x4DC0
		int32_t m_player_count; //0x4EC0
		char pad_4EC4[4]; //0x4EC4
		class rlRemoteGamer m_remote_gamers[32]; //0x4EC8
		uint32_t m_num_remote_gamers; //0x52C8
		bool m_player_joining; //0x52CC
		char pad_52CD[107]; //0x52CD
		uint32_t m_connection_identifier; //0x5338
		char pad_533C[4]; //0x533C
		uint32_t m_profile_index; //0x5340
		char m_token_key[64]; //0x5344
		char m_id_key[64]; //0x5384
		char m_info_key[64]; //0x53C4
		char m_host_key[64]; //0x5404
		char m_join_key[64]; //0x5444
		char pad_5484[4]; //0x5484

		snPlayer* GetPlayerByPeerAddress(uint64_t PeerAddress)
		{
			for (uint32_t i{}; i != m_player_count; ++i)
			{
				if (snPlayer* SnPlayer = m_players[i]; SnPlayer)
				{
					if (SnPlayer->m_gamer_info.m_peer_address == PeerAddress)
						return SnPlayer;
				}
			}
			return nullptr;
		}

		snPlayer* GetPlayerByPlatformData(uint64_t PlatformData)
		{
			for (uint32_t i{}; i != m_player_count; ++i)
			{
				if (snPlayer* SnPlayer = m_players[i]; SnPlayer)
				{
					if (SnPlayer->m_gamer_info.m_platform_data == PlatformData)
						return SnPlayer;
				}
			}
			return nullptr;
		}

		snPlayer* GetPlayerByMsgId(uint32_t MsgId)
		{
			for (uint32_t i{}; i != m_player_count; ++i)
			{
				if (snPlayer* SnPlayer = m_players[i]; SnPlayer)
				{
					if (SnPlayer->m_msg_id == MsgId)
						return SnPlayer;
				}
			}
			return nullptr;
		}

		bool IsHost()
		{
			return m_local_player.m_gamer_info.m_peer_address == m_host_token;
		}

		rlSessionInfo Info()
		{
			return m_rline_session.m_session_info;
		}

		uint64_t Id()
		{
			return m_rline_session.m_session_id;
		}
	}; //Size: 0x3E70
	static_assert(sizeof(snSession) == 0x5488);

	class snMsgRemoveGamersFromSessionCmd
	{
	public:
		uint64_t m_session_id; //0x0000
		rlGamerHandle m_handles[32]; //0x0008
		int32_t m_unk = -1; //0x208
		uint32_t m_num_handles; //0x20C
	}; //Size: 0x0110
	static_assert(sizeof(snMsgRemoveGamersFromSessionCmd) == 0x210);

	class snMsgAddGamerToSessionCmd
	{
	public:
		uint64_t m_session_id; //0x0000
		class rlGamerInfo m_gamer_info; //0x0008
		class netPeerAddress m_peer_address; //0x00A0
		uint32_t unk_00C0; //0x00C0
		uint32_t m_unk_struct_size; //0x00C4
		char m_unk_struct[512]; //0x00C8
		uint32_t m_num_handles; //0x02C8
		rlGamerHandle m_handles[32]; //0x02C8
	}; //Size: 0x00A8
	enum InitFunctionType : int
	{
		INIT_UNKNOWN = 0,
		INIT_CORE = 1,
		INIT_BEFORE_MAP_LOADED = 2,
		INIT_AFTER_MAP_LOADED = 4,
		INIT_SESSION = 8
	};

	class InitFunctionData
	{
	public:
		virtual ~InitFunctionData() = default;
		virtual void initFunction(int unk) = 0;
		virtual void shutdownFunction(int unk) = 0;

		int m_init_order;
		int m_shutdown_order;
		InitFunctionType m_init_mask;
		InitFunctionType m_shutdown_mask;
		u32 m_func_hash;
	};

	class InitFunctionEntry
	{
	public:
		int m_order;
		atArray<int> m_functions;
		InitFunctionEntry* m_next;
	};

	class InitFunctionList
	{
	public:
		InitFunctionType m_type;
		InitFunctionEntry* m_entries;
		InitFunctionList* m_next;
	};

	class gameSkeletonUpdateBase
	{
	public:
		virtual ~gameSkeletonUpdateBase() = 0;
		virtual u64 Run() = 0;

		bool m_flag;
		float m_unk_float;
		u32 m_hash;
		//Might be different for updateElement
		gameSkeletonUpdateBase* m_next_ptr;
		gameSkeletonUpdateBase* m_child_ptr;
	};

	class UpdateFunctionList
	{
	public:
		i32 m_type;
		gameSkeletonUpdateBase* m_entry;
		UpdateFunctionList* m_next;
	};

	class gameSkeleton
	{
	public:
		virtual ~gameSkeleton() = 0;

		int m_function_order;
		InitFunctionType m_function_type;
		int32_t pad;
		int m_updateType;
		atArray<InitFunctionData> m_init_functions;
		void* pad2;
		char pad3[256];
		InitFunctionList* m_init_function_list;
		void* pad4;
		UpdateFunctionList* m_update_function_list;
	};

	enum class grcLockFlags : int32_t
	{
		Read = 1,
		Write = 2,
		Unknown = 4,
		WriteDiscard = 8,
		NoOverwrite = 16
	};

	class grcLockedTexture
	{
	public:
		int32_t m_level; //0x0000
		void* m_bits; //0x0004
		int32_t m_pitch; //0x000C
		int32_t m_pad; //0x0010
		uint32_t m_width; //0x0014
		uint32_t m_height; //0x0018
		int32_t m_format; //0x001C
		int32_t m_num_sub_levels; //0x0020
	}; //Size: 0x0028
	static_assert(sizeof(grcLockedTexture) == 0x28);

	struct grcTextureStored
	{
		char* m_name;
		ID3D11Resource* m_resource;
		ID3D11ShaderResourceView* m_shader;
		uint16_t m_width;
		uint16_t m_height;
		uint16_t m_depth;
	};

	class grcTexture
	{
	public:
		virtual ~grcTexture() = NULL;
		virtual bool unk_0008() = NULL;
		virtual int unk_0010() = NULL;
		virtual void unk_0018() = NULL;
		virtual int unk_0020() = NULL;
		virtual uint16_t GetWidth() = NULL;
		virtual uint16_t GetHeight() = NULL;
		virtual uint16_t GetDepth() = NULL;
		virtual uint8_t GetLevels() = NULL;
		virtual void unk_0048() = NULL;
		virtual bool unk_0050() = NULL;
		virtual void unk_0058(int64_t) = NULL;
		virtual void unk_0060(void*) = NULL;
		virtual void unk_0068(void*) = NULL;
		virtual void unk_0070() = NULL;
		virtual grcTexture* unk_0078() = NULL;
		virtual grcTexture* unk_0080() = NULL;
		virtual bool unk_0088() = NULL;
		virtual int unk_0090() = NULL;
		virtual int unk_0098() = NULL;
		virtual int unk_00A0() = NULL;
		virtual int unk_00A8() = NULL;
		virtual int unk_00B0() = NULL;
		virtual int unk_00B8() = NULL;
		virtual int unk_00C0() = NULL;
		virtual bool Map(int numSubLevels, int subLevel, grcLockedTexture* lockedTexture, grcLockFlags flags) = NULL;

		char pad_0008[32]; //0x0008
		char* m_name; //0x0028
		uint16_t m_ref_count; //0x0030
		uint8_t m_resource_type; //0x0032
		uint8_t m_layers; //0x0033
		char pad_0034[4]; //0x0034
		ID3D11Resource* m_resource; //0x0038
		uint32_t m_physical_size; //0x0040
		uint32_t m_handle; //0x0044
		uint32_t m_flags; //0x0048
		char pad_004C[4]; //0x004C
		uint16_t m_width; //0x0050
		uint16_t m_height; //0x0052
		uint16_t m_depth; //0x0054
		uint16_t m_mip_stride; //0x0056
		uint32_t m_format; //0x0058
		uint8_t m_image_type; //0x005C
		uint8_t m_mip_count; //0x005D
		uint8_t m_cut_mip_levels; //0x005E
		bool m_is_srgb; //0x005F
		grcTexture* m_previous; //0x0060
		grcTexture* m_next; //0x0068
		void* m_backing; //0x0070
		ID3D11ShaderResourceView* m_shader; //0x0078
		char pad_0080[40]; //0x0080
	public:
		void SetShader(ID3D11ShaderResourceView* shader, ID3D11Resource* resource = nullptr)
		{
			m_shader = shader;
			if (!resource)
			{
				m_shader->GetResource(&resource);
			}
			m_resource = resource;
		}

		void SetSize(vector2 size)
		{
			m_width = static_cast<uint16_t>(size.x);
			m_height = static_cast<uint16_t>(size.y);
		}

		vector2 Size()
		{
			return {static_cast<float>(m_width), static_cast<float>(m_height)};
		}

		void Set(std::string file, ID3D11ShaderResourceView* shader, vector2 size)
		{
			m_name = new char[file.length() + 1];
			//Yes, I know this is never deleted. The game will handle it, so hush...
			strcpy_s(m_name, file.length() + 1, file.c_str());
			SetShader(shader);
			SetSize(size);
			m_depth = 1;
		}

		void Swap(grcTexture* texure, bool retainOrginialSizes = false)
		{
			m_resource = texure->m_resource;
			m_shader = texure->m_shader;
			if (!retainOrginialSizes)
			{
				m_width = texure->m_width;
				m_height = texure->m_height;
				m_depth = texure->m_depth;
			}
		}

		void Swap(grcTextureStored texure, bool retainOrginialSizes = false)
		{
			m_resource = texure.m_resource;
			m_shader = texure.m_shader;
			m_width = texure.m_width;
			m_height = texure.m_height;
			m_depth = texure.m_depth;
		}

		grcTextureStored CreateCopy()
		{
			grcTextureStored data{};
			data.m_name = m_name;
			data.m_resource = m_resource;
			data.m_shader = m_shader;
			data.m_width = m_width;
			data.m_height = m_height;
			data.m_depth = m_depth;
			return data;
		}
	}; //Size: 0x00A8
	static_assert(sizeof(grcTexture) == 0xA8);

	class grcRenderTarget : public grcTexture
	{
	public:
	}; //Size: 0x00A8
	static_assert(sizeof(grcRenderTarget) == 0xA8);

	class grcTextureFactory
	{
	public:
		virtual ~grcTextureFactory() = default;
		virtual grcTexture* unk_0008() = NULL;
		virtual grcTexture* CreateManualTexture() = NULL;
		virtual grcTexture* unk_0018() = NULL;
		virtual grcTexture* CreateTexture(const char* name, int32_t id) = NULL;
		virtual void unk_0028() = NULL;
		virtual void unk_0030() = NULL;
		virtual int TranslateFormatToParamFormat(int32_t format) = NULL;
		virtual void unk_0040() = NULL;
		virtual void unk_0048() = NULL;
		virtual void unk_0050() = NULL;
		virtual void unk_0058() = NULL;
		virtual void unk_0060() = NULL;
		virtual void unk_0068() = NULL;
		virtual void unk_0070() = NULL;
		virtual void unk_0078() = NULL;
		virtual void unk_0080() = NULL;
		virtual void unk_0088() = NULL;
		virtual grcTexture* CreateFromNativeTexture(const char* name, ID3D11Resource* nativeResource, void* a3) = NULL;
		virtual void unk_00A0() = NULL;
		virtual void PushRenderTarget(void* a1, grcRenderTarget* target, void* a3, int a4, bool a5, bool a6) = NULL;
		virtual void PopRenderTarget(void* a1, grcRenderTarget* target) = NULL;

	public:
		char pad_0008[8]; //0x0008
	}; //Size: 0x0010
	static_assert(sizeof(grcTextureFactory) == 0x10);

	template <typename T>
	class pgDictionary
	{
	public:
		virtual ~pgDictionary() = default;
		uint64_t unk_0008; //0x0008
		uint64_t unk_0010; //0x0010
		uint32_t unk_0018; //0x0018
		uint32_t unk_001C; //0x001C
		uint64_t unk_0020; //0x0020
		int16_t m_count; //0x0028
		int16_t unk_002A; //0x002A
		char pad_002C[4]; //0x002C
		T** m_items; //0x0030
		uint32_t unk_0038; //0x0038

		bool AddElement(T* element)
		{
			m_items[m_count] = element;
			m_count++;
			return m_items && m_items[m_count - 1] && IsElementValid(m_count - 1);
		}

		bool IsElementValid(int16_t index)
		{
			if (!m_items)
				return false;
			if (index >= m_count)
				return false;
			return m_items[index];
		}

		T* GetElement(int16_t index)
		{
			if (IsElementValid(index))
			{
				if (m_items[index])
				{
					return m_items[index];
				}
			}
			return nullptr;
		}
	}; //Size: 0x003C
	template <typename T>
	class pgDictionaryPool
	{
	public:
		pgDictionary<T>* m_dictionary; //0x0000
		uint32_t m_capacity; //0x0008
		uint32_t m_dictionary_hash; //0x000C
		char pad_0010[8]; //0x0010
	}; //Size: 0x0018
	class grcTextureStore
	{
	public:
		char pad_0000[56]; //0x0000
		pgDictionaryPool<grcTexture>* m_pool; //0x0038
		uint8_t* m_mask; //0x0040
		uint32_t m_count; //0x0048
		uint32_t m_size; //0x0052
	public:
		auto BitshiftMask(uint32_t index, uint8_t amount) { return m_mask[index] >> amount; }

		bool IsElementValid(uint32_t index)
		{
			if (!m_mask)
				return false;
			if (index >= m_count)
				return false;
			return ~BitshiftMask(index, 7) & 1;
		}

		pgDictionaryPool<grcTexture>* GetElement(uint32_t index)
		{
			if (IsElementValid(index))
			{
				if (m_pool[index].m_dictionary)
				{
					return &m_pool[index];
				}
			}
			return nullptr;
		}

		pgDictionary<grcTexture>* FindDictionary(uint32_t hash)
		{
			for (decltype(m_count) i{m_count}; i; --i)
			{
				if (auto element = GetElement(i); element)
				{
					if (element->m_dictionary_hash == hash)
					{
						return element->m_dictionary;
					}
				}
			}
			return nullptr;
		}

		std::map<uint32_t, pgDictionary<grcTexture>*> GetAllDictionaries()
		{
			std::map<uint32_t, pgDictionary<grcTexture>*> elements{};
			for (decltype(m_count) i{m_count}; i; --i)
			{
				if (auto element{GetElement(i)}; element)
				{
					if (element->m_dictionary_hash)
					{
						elements.insert({element->m_dictionary_hash, element->m_dictionary});
					}
				}
			}
			return elements;
		}

		grcTexture* FindTexture(std::string dictionary, std::string name)
		{
			uint32_t hash{joaat(dictionary)};
			uint32_t textureHash{joaat("platform:/textures/" + dictionary)};
			if (auto pgDictionary{FindDictionary(hash)})
			{
				for (decltype(pgDictionary->m_count) i{pgDictionary->m_count}; i; --i)
				{
					if (auto element{pgDictionary->GetElement(i)}; element && !name.compare(element->m_name))
					{
						return element;
					}
				}
			}
			if (auto pgDictionary{FindDictionary(textureHash)})
			{
				for (decltype(pgDictionary->m_count) i{pgDictionary->m_count}; i; --i)
				{
					if (auto element{pgDictionary->GetElement(i)}; element && !name.compare(element->m_name))
					{
						return element;
					}
				}
			}
			return nullptr;
		}
	}; //Size: 0x0056
	class netTextMsg
	{
	public:
		char m_message[256]; //0x0000
		rlGamerHandle m_handle; //0x0100
	}; //Size: 0x0110
	static_assert(sizeof(netTextMsg) == 0x110);
#pragma pack(push, 8)
	class netSyncNodeBase
	{
	public:
		virtual ~netSyncNodeBase() = default; //0x00
		virtual bool IsDataNode() { return false; }; //0x08
		virtual bool IsParentNode() { return false; }; //0x10
		virtual void MoveCommonDataOpsVFT()
		{
		}; //0x18
		virtual void ClearChildren()
		{
		}; //0x20
		virtual void _0x28(void*, void*, void*, int* out_count)
		{
		}; //0x28
		virtual bool Serialize(int flags, int flags2, void*, datBitBuffer* buffer, int, void*, bool, int*,
		                       int* num_serialized) { return false; } //0x30
		virtual bool Deserialize(int flags, int flags2, datBitBuffer* buffer, void*) { return false; } //0x38
		virtual int CalculateSize(int flags, int flags2, void*) { return 0; } //0x40
		virtual int CalculateSize2(int flags, int flags2, bool) { return 0; } //0x48

		netSyncNodeBase* m_next_sibling; //0x0008
		netSyncNodeBase* m_prev_sibling; //0x0010
		netSyncNodeBase* m_root; //0x0018
		netSyncNodeBase* m_parent; //0x0020
		uint32_t m_flags1; //0x0028
		uint32_t m_flags2; //0x002C
		uint32_t m_flags3; //0x0030
		uint32_t m_pad2; //0x0034
		netSyncNodeBase* m_first_child; //0x0038
	}; //Size: 0x0040
	static_assert(sizeof(netSyncNodeBase) == 0x40);
#pragma pack(pop)
#pragma pack(push, 8)
	class netSyncDataNode : public netSyncNodeBase
	{
	public:
		uint32_t m_flags; //0x40
		uint32_t unk_0044; //0x44
		uint64_t unk_0048; //0x48

		netSyncDataNode* m_parent; //0x50
		uint32_t m_child_count; //0x58
		netSyncDataNode* m_children[8]; //0x5C
		uint8_t m_sync_frequencies[8]; //0x9C
		void* m_common_data_operation_vtable; //0xA8
	};

	static_assert(sizeof(netSyncDataNode) == 0xB0);
#pragma pack(pop)
	class sysModuleVtbl
	{
	public:
		u64 desturctor;
		u64 unk_0008;
		u64 unk_0010;
		u64 unk_0018;
		u64 unk_0020;
		u64 unk_0028;
		u64 unk_0030;
		u64 unk_0038;
		u64 unk_0040;
		u64 unk_0048;
		u64 unk_0050;
		u64 unk_0058;
		u64 function1;
	}; //Size: 0x0008
	class sysModule
	{
	public:
		sysModuleVtbl* m_vtable;
		char pad_0008[152];
	}; //Size: 0x00A0
	class atPool;
	class strStreamingModule;
	class streamingDataEntry;
	class streamingListEntry;
	class strStreamingModuleMgr;

	class atPool
	{
	public:
		int8_t* m_data; //0x0000
		int8_t* m_flags; //0x0008
		uint32_t m_max; //0x0010
		uint32_t m_size; //0x0014
		uint32_t m_count; //0x0018

		template <typename T>
		T* GetAt(int index)
		{
			if (m_flags[index] < 0)
				return nullptr;
			return static_cast<T*>(m_data + (index * m_size));
		}
	}; //Size: 0x0020
	static_assert(sizeof(atPool) == 0x20);

	class strStreamingModule
	{
	public:
		uintptr_t m_vtable; //0x0000
		uint32_t m_base_index; //0x0008
		atArray<char> m_name; //0x0014
		char pad_000C[24]; //0x000C
		atPool m_pool; //0x0038

		std::string get_name()
		{
			return m_name.data();
		}
	}; //Size: 0x0058
	static_assert(sizeof(strStreamingModule) == 0x58);

	class streamingDataEntry
	{
	public:
		uint32_t m_handle; //0x0000
		uint32_t m_flags; //0x0004
	}; //Size: 0x0008
	static_assert(sizeof(streamingDataEntry) == 0x8);

	class streamingListEntry
	{
	public:
		atDList<streamingListEntry> m_list; //0x0000
		uint32_t m_index; //0x0010
	}; //Size: 0x0014
	class strStreamingModuleMgr
	{
	public:
		strStreamingModuleMgr()
		{
		}

		virtual ~strStreamingModuleMgr() = default;
		char pad_0008[16]; //0x0008
		atArray<strStreamingModule*> m_modules; //0x0018

		strStreamingModule* GetModuleFromExtension(cc* Extension);

		std::string GetStreamingNameForIndex(u32 Index)
		{
			for (auto& entry : m_modules)
			{
				if (entry->m_base_index == Index)
				{
					return entry->get_name();
				}
			}
			return "";
		}

		static strStreamingModuleMgr& Get();
	};
}

struct Asset
{
	int Value;

	bool Valid()
	{
		return Value != -1;
	}

	void Set(Asset v)
	{
		Value = v.Value;
	}

	void Set(int v)
	{
		Value = v;
	}
};

class CStreaming
{
public:
	static CStreaming* Get();

	rage::streamingDataEntry* Entry(Asset asset)
	{
		if (asset.Valid())
			return &m_entries[asset.Value];
		return nullptr;
	}

	rage::streamingDataEntry* m_entries; //0x0000
	char pad_0008[16]; //0x0008
	int32_t m_num_entries; //0x0018
	int32_t unk_001C; //0x001C
	char pad_0020[64]; //0x0020
	rage::streamingListEntry* m_request_list_head; //0x0060
	rage::streamingListEntry* m_request_list_tail; //0x0068
	char pad_0070[152]; //0x0070
	uint32_t m_loaded_list_size; //0x0108
	char pad_0010C[168]; //0x0010C
	rage::strStreamingModuleMgr m_module_mgr; //0x01B8
	int32_t m_num_pending_requests; //0x01E0
	int32_t m_num_pending_requests_unk; //0x01E4
	int32_t m_num_pending_requests_preious; //0x01E8
}; //Size: 0x01F0
static_assert(sizeof(CStreaming) == 0x1F0);
#pragma pack(push, 1)
class CNetShopTransaction
{
public:
	uint32_t m_id; //0x0000
	uint32_t m_variation; //0x0004
	uint32_t m_price; //0x0008
	uint32_t m_multiplier; //0x000C
	uint32_t m_value; //0x0010
}; //Size: 0x0014
static_assert(sizeof(CNetShopTransaction) == 0x14);

class CNetShopTransactionBasket
{
public:
	virtual ~CNetShopTransactionBasket() = default;

	uint32_t m_transaction_id; //0x0008
	char pad_000C[12]; //0x000C
	uint32_t m_category; //0x0018
	char pad_001C[8]; //0x001C
	uint32_t m_action; //0x0020
	uint32_t m_target; //0x0024
	char pad_0002C[180]; //0x0002C
	class CNetShopTransaction m_transactions[71]; //0x00E0
	uint32_t m_transaction_count; //0x066C
}; //Size: 0x0670
static_assert(sizeof(CNetShopTransactionBasket) == 0x670);
#pragma pack(pop)
class CNetworkShoppingMgr
{
public:
	virtual ~CNetworkShoppingMgr() = default;

	char pad_0008[24]; //0x0008
	rage::atDList<rage::atDNode<CNetShopTransactionBasket*>> m_transaction_nodes; //0x0020
	char pad_0030[9]; //0x0030
	bool m_is_busy1; //0x0039
	char pad_003A[14]; //0x003A
	bool m_is_busy2; //0x0048
	char pad_0049[7]; //0x0049
	bool m_is_busy3; //0x0050
	char pad_0051[79]; //0x0051
	void* m_inventory_items; //0x00A0
	char pad_00A8[24]; //0x00A8
	char m_gs_token[36]; //0x00C0
	char pad_00E4[28]; //0x00E4
	uint32_t m_transaction_noce_seed; //0x0100
	char pad_0104[44]; //0x0104
	uint32_t m_transaction_count; //0x0130
	char pad_0134[396]; //0x0134
	char* m_balance; //0x02C0

	uint32_t get_transaction_nonce()
	{
		return m_transaction_noce_seed + m_transaction_count;
	}
}; //Size: 0x02C8
static_assert(sizeof(CNetworkShoppingMgr) == 0x2C8);

class UIElement
{
public:
	int m_index; //0x0000
	char pad_0004[12]; //0x0004
	rage::vector4 m_value; //0x0010
	void setValueRGBA(u8 r, u8 g, u8 b, u8 a)
	{
		m_value.x = (r & 0xFF) / 255.f;
		m_value.y = (g & 0xFF) / 255.f;
		m_value.z = (b & 0xFF) / 255.f;
		m_value.w = (a & 0xFF) / 255.f;
	}
}; //Size: 0x0020
static_assert(sizeof(UIElement) == 0x20);

class UIElementConstant
{
public:
	rage::vector4 m_value; //0x0000
	float m_intensity; //0x0010
	float m_position; //0x0014
	char pad_0018[4]; //0x0018
	int m_index; //0x001C
	void setValueRGBA(u8 r, u8 g, u8 b, u8 a)
	{
		m_value.x = (r & 0xFF) / 255.f;
		m_value.y = (g & 0xFF) / 255.f;
		m_value.z = (b & 0xFF) / 255.f;
		m_value.w = (a & 0xFF) / 255.f;
	}
}; //Size: 0x0020
static_assert(sizeof(UIElementConstant) == 0x20);

class UIIndex
{
public:
	int m_index; //0x0000
}; //Size: 0x0004
static_assert(sizeof(UIIndex) == 0x4);

class UIElementInt
{
public:
	UIIndex m_index; //0x0000
	float m_value; //0x0004
}; //Size: 0x0008
static_assert(sizeof(UIElementInt) == 0x8);

class UIElementShader
{
public:
	uint64_t m_raw_element; //0x0000
	rage::grcTexture* m_texture; //0x0008
	void setShader(ID3D11ShaderResourceView* shader, ID3D11Resource* resource = nullptr)
	{
		m_texture->SetShader(shader, resource);
	}
}; //Size: 0x0010
static_assert(sizeof(UIElementShader) == 0x10);

class TimecycleKeyframeData
{
public:
	char pad_0000[16]; //0x0000
	UIElement m_azimuth_east_color; //0x0010
	float m_azimuth_east_intensity; //0x0030
	char pad_0034[12]; //0x0034
	UIElement m_azimuth_west_color; //0x0040
	float m_azimuth_west_intensity; //0x0060
	char pad_0064[12]; //0x0064
	UIElement m_azimuth_transition_color; //0x0070
	float m_azimuth_transition_intensity; //0x0090
	UIElementInt m_azimuth_transition_position; //0x0094
	char pad_009C[4]; //0x009C
	UIElement m_zenith_color; //0x00B8
	float m_zenith_intensity; //0x00C0
	char pad_00C4[12]; //0x00C4
	UIElement m_zenith_transition_color; //0x00D0
	char pad_00F0[4]; //0x00F0
	UIElementConstant m_zenith_constants; //0x00F0
	char pad_0114[28]; //0x0114
	UIElement m_sky_plane_color; //0x0130
	char pad_0150[16]; //0x0150
	UIIndex m_sky_plane_params; //0x0160
	char pad_0164[40]; //0x0164
	bool m_update_sky_attributes; //0x018C
	char pad_018D[11]; //0x018D
	UIIndex m_sun_direction; //0x0198
	UIIndex m_sun_position; //0x019C
	char pad_01A0[16]; //0x01A0
	UIElement m_sun_color; //0x01B0
	UIElement m_sun_color_hdr; //0x01D0
	char pad_01F0[16]; //0x01F0
	UIElement m_sun_disc_color_hdr; //0x0200
	char pad_0220[32]; //0x0220
	UIElement m_sun_constants; //0x0240
	UIElementShader m_high_detail_noise; //0x0260
	UIElementConstant m_cloud_constant1; //0x0270
	char pad_0290[16]; //0x0290
	UIElementConstant m_cloud_constant2; //0x02A0
	char pad_02C0[16]; //0x02C0
	UIElementConstant m_cloud_constant3; //0x02D0
	char pad_02F0[16]; //0x02F0
	UIElementConstant m_cloud_detail_constants; //0x0300
	char pad_0320[48]; //0x0320
	UIElement m_cloud_base_minus_mid_colour; //0x0350
	UIElement m_cloud_mid_color; //0x0370
	UIElement m_cloud_shadow_minus_base_colour_times_shadow_strength; //0x0390
	UIElementConstant m_small_cloud_constants; //0x03B0
	char pad_03D0[32]; //0x03D0
	UIElement m_small_cloud_color_hdr; //0x03F0
	char pad_0410[16]; //0x0410
	UIIndex m_cloudgen_frequency; //0x0420
	char pad_0424[28]; //0x0424
	UIElement m_noise_phase; //0x0440
	char pad_0460[32]; //0x0460
	UIIndex m_speed_constants; //0x0480
	char pad_0484[28]; //0x0484
	UIElementInt m_horizon_level; //0x04A0
	char pad_04A8[16]; //0x04A8
	UIElementShader m_dither; //0x04B8
	char pad_04C8[24]; //0x04C8
	UIElementShader m_star_field; //0x04E0
	UIElementInt m_starfield_intensity; //0x04F0
	UIElementShader m_moon; //0x04F8
	char pad_0508[40]; //0x0508
	UIElementInt m_moon_intensity; //0x0530
	char pad_0538[8]; //0x0538
	UIElement m_moon_color; //0x0540
	char pad_0560[16]; //0x0560
	UIIndex m_lunar_cycle; //0x0570
	char pad_0574[28]; //0x0574
	UIIndex m_moon_direction; //0x0590
	UIIndex m_moon_position; //0x0594
	char pad_0598[24]; //0x0598
	UIElementInt m_noise_frequency; //0x05A8
	UIElementInt m_noise_scale; //0x05B0
	UIElementInt m_noise_threshold; //0x05B8
	UIElementInt m_noise_softness; //0x05C0
	UIElementInt m_noise_density_offset; //0x05C8
	UIElementShader m_noise; //0x05D8

	void set_sky(bool enabled)
	{
		*(bool*)((uint64_t)this - 0x60) = enabled;
	}
}; //Size: 0x05E8
static_assert(sizeof(TimecycleKeyframeData) == 0x5E8);

class CRequestData
{
public:
	char m_data[465]; //0x0000
}; //Size: 0x01D1
static_assert(sizeof(CRequestData) == 0x1D1);

class CHttpRequest
{
public:
	char pad_0000[16]; //0x0000
	CRequestData* m_request; //0x0010
	char pad_0018[64]; //0x0018
	CRequestData* m_response_headers; //0x0058
	char pad_0060[16]; //0x0060
	rage::sysMemAllocator* m_allocator; //0x0070
	char pad_0078[16]; //0x0078
	CRequestData* m_http_headers; //0x0088
	char pad_0090[1112]; //0x0090
	CRequestData* m_response; //0x04E8
	char pad_04F0[4]; //0x04F0
	uint16_t m_content_length; //0x04F4
	char pad_04F6[18]; //0x04F6
	char* m_protocol; //0x0508
	char* m_base_url; //0x0510
	char* m_endpoint_data; //0x0518
	char pad_0520[72]; //0x0520
	char* m_user_agent; //0x0568
}; //Size: 0x0570
static_assert(sizeof(CHttpRequest) == 0x570);

class CNetworkSCNewsStory
{
	//Not the real name
public:
	uint32_t m_index; //0x0000
	uint32_t unk_0000; //0x0004
	uint32_t unk_0004; //0x0008
	char* m_buffer; //0x0010
	uint32_t m_max_length; //0x0018
	uint32_t m_current_length; //0x001C
	uint32_t m_read; //0x0020
	uint8_t m_flags; //0x0024

	std::string str()
	{
		return m_buffer;
	}

	/*void set(const std::string& str)
	{
		strcpy(m_buffer, str.c_str());
		m_current_length = static_cast<uint32_t>(str.length());
		m_max_length = UINT_MAX;
	}*/

	void clear()
	{
		memset(m_buffer, NULL, m_max_length);
	}
};
#pragma pack(push, 4)
class CNetworkSCNewsStoryRequest
{
public:
	virtual ~CNetworkSCNewsStoryRequest() = default;
	virtual void GetNewsStory(CNetworkSCNewsStory* pNewsStory) = NULL;
	uint32_t m_response_code; //0x0008
	uint32_t m_response_length; //0x000C
	char m_response[136]; //0x0010
	uint32_t unk_0098; //0x0098
	uint32_t unk_009C; //0x009C
	char m_news_page[32]; //0x00A0
	char unk_00C0; //0x00C0
	char pad_00C1[39]; //0x00C1
	int unk_00E8; //0x00E8
	char pad_00EC[4]; //0x00EC
	uint64_t unk_00F0; //0x00F0
	uint32_t unk_00F8; //0x00F8
	char pad_00FC[4]; //0x00FC
	uint64_t unk_0100; //0x0100
	uint32_t unk_0108; //0x0108
	char pad_010C[4]; //0x010C
	uint64_t unk_0110; //0x0110
	uint32_t unk_0118; //0x0118
	char pad_011C[4]; //0x0011C
	uint64_t unk_0120; //0x0120
	uint32_t unk_0128; //0x0128
	char pad_012C[4]; //0x012C
	uint64_t unk_0130; //0x0130
	uint32_t unk_0138; //0x0138
	char pad_013C[4]; //0x013C
	uint64_t unk_0140; //0x0140
	uint32_t unk_0148; //0x0148
	char pad_014C[4]; //0x014C
	uint32_t unk_0150; //0x0150
	uint32_t unk_0154; //0x0154
	char unk_0158; //0x0158
	char pad_0159[327]; //0x0159
	uint16_t word2A0; //0x02A0
	uint64_t m_current_news_page; //0x02A4
	uint32_t unk_02AC; //0x02AC
	uint32_t unk_02B0; //0x02B0
}; //Size: 0x02B4
static_assert(sizeof(CNetworkSCNewsStoryRequest) == 0x2B4);
#pragma pack(pop)
class CGameScriptId : public rage::scriptId
{
public:
	char pad_002C[4]; //0x002C
	uint32_t m_timestamp; //0x0030
	uint32_t m_position_hash; //0x0034
	int32_t m_instance_id; //0x0038
	int32_t unk_003C; //0x003C
	void serialize(rage::datBitBuffer& buf)
	{
		buf.Write<u32>(m_hash, 32);
		buf.Write<u32>(m_timestamp, 32);
		buf.Write<bool>(true, 1);
		buf.Write<u32>(m_position_hash, 32);
		buf.Write<bool>(true, 1);
		buf.Write<i32>(m_instance_id, 32);
	}

	void deserialize(rage::datBitBuffer& buf)
	{
		m_hash = buf.Read<u32>(32);
		m_timestamp = buf.Read<u32>(32);
		if (buf.Read<bool>(1))
			m_position_hash = buf.Read<u32>(32);
		if (buf.Read<bool>(1))
			m_instance_id = buf.Read<i32>(32);
	}
}; //Size: 0x0040
#pragma pack(push, 4)
class CGameScriptObjInfo
{
public:
	virtual ~CGameScriptObjInfo() = default;

	ScrHandle m_local_handle; //0x0008
	uint16_t m_network_handle; //0x000C
	CGameScriptId m_script_id; //0x0010
};

static_assert(sizeof(CGameScriptObjInfo) == 0x50);
#pragma pack(pop)
class NodeCommonDataOperations
{
public:
	virtual ~NodeCommonDataOperations() = default;

	virtual void ReadFromBuffer(rage::netSyncDataNode* node)
	{
	}; //0x08
	virtual void WriteToBuffer(rage::netSyncDataNode* node)
	{
	}; //0x10
	virtual void Unk()
	{
	}; //0x18
	virtual int CalculateSize(rage::netSyncDataNode* node) { return 0; }; //0x20
	virtual int CalculateSize2(rage::netSyncDataNode* node) { return 0; }; //0x28
	virtual void LogSyncData(rage::netSyncDataNode* node)
	{
	}; //0x30
	virtual void LogSyncData2(rage::netSyncDataNode* node)
	{
	}; //0x38

	rage::datBitBuffer* m_buffer; //0x008
};

static_assert(sizeof(NodeCommonDataOperations) == 0x10);

class CProjectBaseSyncDataNode : public rage::netSyncDataNode
{
public:
	virtual bool IsSyncNode() { return false; } //0x50
	virtual bool _0x58() { return false; } //0x58
	virtual bool IsGlobalFlags() { return false; } //0x60
	virtual void DoPreCache(rage::netSyncData* data)
	{
	} //0x68
	virtual uint8_t GetSyncFrequency(int index) { return 0; } //0x70
	virtual int GetSyncInterval(int index) { return 0; } //0x78
	virtual int GetBandwidthForPlayer(int player) { return 200; } //0x80 (should always return 200)
	virtual void _0x88(void*)
	{
	} //0x88
	virtual bool _0x90(void*, void*, int, int, int) { return false; } //0x90
	virtual int CalculateSize() { return 0; } //0x98 need to verify later
	virtual bool IsPreCacheDisabled() { return false; } //0xA0
	virtual bool CanApply(rage::netObject* object) { return false; } //0xA8
	virtual int GetPlayersInScope() { return -1; } //0xB0
	virtual void DeserializeImpl()
	{
	} //0xB8 need to verify later
	virtual void SerializeImpl()
	{
	} //0xC0 need to verify later
	virtual int CalculateSize2() { return 0; } //0xC8
	virtual int _0xD0() { return 0; } //0xD0 calls NodeCommonDataOperations::Unk()
	virtual void Log()
	{
	} //0xD8
	virtual bool CanPreCache(int) { return false; } //0xE0 arg is always zero afaik
	virtual bool CanBeEmpty() { return false; } //0xE8
	virtual bool IsEmpty() { return false; } //0xF0 returns true if all data is default
	virtual void SetEmpty()
	{
	} //0xF8 sets all data to their default values
	virtual void Log2()
	{
	} //0x100
	virtual void ResetScriptData()
	{
	} //0x108
	virtual bool _0x110() { return false; } //0x110

private:
	NodeCommonDataOperations m_common_data_operations; //0xB0 this is generally invalidated by MoveCommonDataOpsVFT()
};

static_assert(sizeof(CProjectBaseSyncDataNode) == 0xC0);

class CSyncDataNodeFrequent : public CProjectBaseSyncDataNode
{
};

class CSyncDataNodeInfrequent : public CProjectBaseSyncDataNode
{
};
#pragma pack(push, 4)
class CSectorPositionDataNode : CSyncDataNodeFrequent
{
public:
	float m_sector_pos_x; //0x00C0
	float m_sector_pos_y; //0x00C4
	float m_sector_pos_z; //0x00C8
}; //Size: 0x00CC
static_assert(sizeof(CSectorPositionDataNode) == 0xCC);
#pragma pack(pop)
#pragma pack(push, 2)
class CSectorDataNode : CSyncDataNodeFrequent
{
public:
	rage::cvector3<uint16_t> m_pos; //0x00C0
};

static_assert(sizeof(CSectorDataNode) == 0xC6);
#pragma pack(pop)
#pragma pack(push, 4)
class CTrainGameStateDataNode : CSyncDataNodeInfrequent
{
public:
	bool m_is_engine; //0x00C0
	bool m_is_caboose; //0x00C1
	bool m_is_mission_train; //0x00C2
	bool m_direction; //0x00C3
	bool m_has_passenger_carriages; //0x00C4
	bool m_render_derailed; //0x00C5
	bool unk_00C6; //0x00C6
	bool unk_00C7; //0x00C7
	uint16_t m_engine_id; //0x00C8
	int8_t m_train_config_index; //0x00CA
	int8_t m_carriage_config_index; //0x00CB
	int8_t m_track_id; //0x00CC
	char pad_00CD[3]; //0x00CD
	float m_distance_from_engine; //0x00D0
	float m_cruise_speed; //0x00D4
	uint16_t m_linked_to_backward_id; //0x00D8
	uint16_t m_linked_to_forward_id; //0x00DA
	uint32_t m_train_state; //0x0DC
	bool unk_00E0; //0x00E0
	bool m_force_doors_open; //0x0E1
}; //Size: 0x00E4
static_assert(sizeof(CTrainGameStateDataNode) == 0xE4);
#pragma pack(pop)
#pragma pack(push, 4)
class CObjectCreationDataNode : CProjectBaseSyncDataNode
{
public:
	uint16_t unk_00C0; //0x00C0
	char pad_00C2[14]; //0x00C2
	rage::vector4 m_object_orientation; //0x00D0
	char pad_00E0[30]; //0x00E0
	rage::vector3 m_object_position; //0x0100
	char pad_010C[4]; //0x010C
	rage::vector3 m_dummy_position; //0x010E
	char pad_011A[20]; //0x011C
	rage::vector3 m_script_grab_position; //0x0130
	char pad_013C[12]; //0x013C
	float m_script_grab_radius; //0x0148
	uint32_t m_created_by; //0x014C
	uint32_t m_model; //0x0150
	uint32_t m_frag_group_index; //0x0154
	uint32_t m_ownership_token; //0x0158
	uint32_t unk_015C; //0x015C
	bool m_no_reassign; //0x0160
	bool unk_0161; //0x0161
	bool m_player_wants_control; //0x0162
	bool m_has_init_physics; //0x0163
	bool m_script_grabbed_from_world; //0x0164
	bool m_has_frag_group; //0x0165
	bool m_is_broken; //0x0166
	bool m_has_exploded; //0x0167
	bool m_keep_registered; //0x0168
	bool unk_0169; //0x0169
	bool unk_016A; //0x016A
	bool unk_016B; //0x016B
}; //Size: 0x016C
static_assert(sizeof(CObjectCreationDataNode) == 0x16C);
#pragma pack(pop)
#pragma pack(push, 8)
class CPickupCreationDataNode : CProjectBaseSyncDataNode
{
public:
	bool m_has_placement; //0x00C0
	char pad_00C1[7]; //0x00C1
	CGameScriptObjInfo m_script_object_info; //0x00C8
	uint32_t m_pickup_hash; //0x0118
	uint32_t m_amount; //0x011C
	uint32_t m_custom_model; //0x0120
	uint32_t m_life_time; //0x0124
	uint32_t m_weapon_component[11]; //0x0128
	uint32_t m_num_weapon_components; //0x0154
	uint32_t m_tint_index; //0x0158
	bool m_player_gift; //0x015C
	bool unk_015D; //0x015D
	char pad_015E[6]; //0x015E
	uint32_t unk_0164; //0x0164
	bool unk_0168; //0x0168
}; //Size: 0x0170
static_assert(sizeof(CPickupCreationDataNode) == 0x170);
#pragma pack(pop)
#pragma pack(push, 4)
class CDoorCreationDataNode : CProjectBaseSyncDataNode
{
public:
	uint32_t m_model; //0x00C0
	char pad_00C4[12]; //0x00C4
	rage::vector3 m_pos; //0x00D0
	char pad_00DC[12]; //0x00DC
	bool m_is_script_door; //0x00E8
	bool m_player_wants_control; //0x00E9
}; //Size: 0x00EC
static_assert(sizeof(CDoorCreationDataNode) == 0xEC);
#pragma pack(pop)
#pragma pack(push, 4)
class CPedCreationDataNode : CProjectBaseSyncDataNode
{
public:
	uint32_t m_pop_type; //0x00C0
	uint32_t m_model; //0x00C4
	uint32_t m_random_seed; //0x00C8
	uint32_t m_max_health; //0x00CC
	bool m_in_vehicle; //0x00D0
	char pad_0D1[1]; //0x00D1
	uint16_t m_vehicle_id; //0x00D2
	uint32_t m_vehicle_seat; //0x00D4
	bool m_has_prop; //0x00D8
	char pad_0D9[3]; //0x00D9
	uint32_t m_prop_model; //0x00DC
	bool m_is_standing; //0x00E0
	bool m_is_respawn_object_id; //0x00E1
	bool m_is_respawn_flagged_for_removal; //0x00E2
	bool m_has_attr_damage_to_player; //0x00E3
	uint8_t m_attribute_damage_to_player; //0x00E4
	uint32_t m_voice_hash; //0x00E8
}; //Size: 0x00EC
static_assert(sizeof(CPedCreationDataNode) == 0xEC);
#pragma pack(pop)
#pragma pack(push, 4)
class CVehicleCreationDataNode : CProjectBaseSyncDataNode
{
public:
	uint32_t m_pop_type; //0x00C0
	uint32_t m_random_seed; //0x00C4
	uint32_t m_model; //0x00C8
	uint32_t m_vehicle_status; //0x00CC
	uint32_t m_max_health; //0x00D0
	uint32_t m_creation_token; //0x00D4
	bool m_car_budget; //0x00D8
	bool m_needs_to_be_hotwired; //0x00D9
	bool m_tires_dont_burst; //0x00DA
	char pad_00DB[165]; //0x00DB
}; //Size: 0x0180
static_assert(sizeof(CVehicleCreationDataNode) == 0x180);
#pragma pack(pop)
#pragma pack(push, 4)
class CVehicleControlDataNode : CSyncDataNodeFrequent
{
public:
	uint32_t m_num_wheels; //0x00C0
	uint32_t unk_00C4; //0x00C4
	uint32_t m_brake_control; //0x00C8
	uint32_t unk_00CC; //0x00CC
	uint32_t m_road_node_address; //0x00D0
	bool m_kers_active; //0x00D4
	bool m_bringing_vehicle_to_halt; //0x00D5
	float m_halt_distance; //0x00D6
	bool m_control_vertical_velocity; //0x00DA
	bool m_has_suspension_data; //0x00DB
	bool unk_00DE; //0x00DE
	float m_suspension_heights[10]; //0x00DF
	bool unk_0108; //0x0108
	bool unk_0109; //0x0109
	bool unk_010A; //0x010A
	bool unk_010B; //0x010B
	bool unk_010C; //0x010C
	bool unk_010D; //0x010D
	bool unk_010E; //0x010E
	float unk_0110; //0x0110
	uint32_t unk_0114; //0x0114
	bool unk_0118; //0x0118
	bool m_is_submarine_car; //0x0119
	char pad_011A[2]; //0x011A
	float m_rudder_rotation_x; //0x011C
	float m_rudder_rotation_y; //0x0120
	float m_rudder_rotation_z; //0x0124
	bool unk_0128; //0x0128
	bool unk_0129; //0x0129
	char pad_0130[5]; //0x0130
}; //Size: 0x0130
static_assert(sizeof(CVehicleControlDataNode) == 0x130);
#pragma pack(pop)
#pragma pack(push, 4)
class CPlayerCreationDataNode : CProjectBaseSyncDataNode
{
public:
	uint32_t m_model; //0x00C0
	uint32_t m_num_scars; //0x00C4
	char unk_struct_00C8[192]; //0x00C8
	uint32_t unk_0188; //0x0188
	char pad_018C[4]; //0x018C
	char m_scar_struct[176]; //0x0190
	bool unk_0240; //0x0240
	char pad_0241[19]; //0x0241
}; //Size: 0x0254
static_assert(sizeof(CPlayerCreationDataNode) == 0x254);
#pragma pack(pop)
#pragma pack(push, 4)
class CPlayerGameStateDataNode : CSyncDataNodeInfrequent
{
public:
	uint32_t m_player_state; //0x00C0
	bool m_controls_disabled_by_script; //0x00C4
	bool m_is_max_armor_and_health_default; //0x00C5
	bool unk_000C6; //0x00C6
	bool m_is_spectating; //0x00C7
	bool m_is_antagonistic_to_another_player; //0x00C8
	bool m_never_target; //0x00C9
	bool m_use_kinematic_physics; //0x00CA
	bool m_has_tutorial_data; //0x00CB
	bool m_pending_tutorial_change; //0x00CC
	bool unk_00CD; //0x00CD
	bool m_respawning; //0x00CE
	bool m_will_jack_any_player; //0x00CF
	bool m_will_jack_wanted_players; //0x00D0
	bool m_dont_drag_from_car; //0x00D1
	bool m_random_peds_flee; //0x00D2
	bool m_every_ped_back_away; //0x00D3
	bool m_has_microphone; //0x00D4
	bool m_is_invincible; //0x00D5
	bool unk_00D6; //0x00D6
	bool unk_00D7; //0x00D7
	bool m_seatbelt; //0x00D8
	bool unk_00D9; //0x00D9
	bool m_bullet_proof; //0x00DA
	bool m_fire_proof; //0x00DB
	bool m_explosion_proof; //0x00DC
	bool m_collision_proof; //0x00DD
	bool m_melee_proof; //0x00DE
	bool m_water_proof; //0x00DF
	bool m_steam_proof; //0x00E0
	bool unk_00E1; //0x00E1
	bool unk_00E2; //0x00E2
	bool unk_00E3; //0x00E3
	bool unk_00E4; //0x00E4
	bool unk_00E5; //0x00E5
	bool unk_00E6; //0x00E6
	bool unk_00E7; //0x00E7
	bool unk_00E8; //0x00E8
	bool unk_00E9; //0x00E9
	bool unk_00EA; //0x00EA
	bool unk_00EB; //0x00EB
	bool unk_00EC; //0x00EC
	bool unk_00ED; //0x00ED
	bool unk_00EE; //0x00EE
	bool unk_00EF; //0x00EF
	bool unk_00F0; //0x00F0
	bool unk_00F1; //0x00F1
	bool unk_00F2; //0x00F2
	bool unk_00F3; //0x00F3
	bool unk_00F4; //0x00F4
	bool unk_00F5; //0x00F5
	bool unk_00F6; //0x00F6
	bool unk_00F7; //0x00F7
	bool unk_00F8; //0x00F8
	bool unk_00F9; //0x00F9
	bool unk_00FA; //0x00FA
	bool unk_00FB; //0x00FB
	uint32_t unk_00FC; //0x00FC
	uint32_t m_mobile_ring_state; //0x0100
	int32_t m_player_team; //0x0104
	float m_air_drag_multiplier; //0x0108
	uint32_t m_max_health; //0x010C
	uint32_t m_max_armor; //0x0110
	uint32_t m_jack_speed; //0x0114
	uint16_t m_player_is_targetable_by_team; //0x0118
	uint32_t m_override_receive_chat; //0x011C
	uint32_t m_override_send_chat; //0x0120
	bool unk_0124; //0x0124
	bool unk_0125; //0x0125
	bool unk_0126; //0x0126
	bool unk_0127; //0x0127
	uint16_t m_spectating_net_id; //0x0128
	uint8_t m_antagonistic_to_player_id; //0x012C
	uint8_t m_tutorial_index; //0x012B
	uint8_t m_tutorial_instance_id; //0x012C
	char pad_012D[2]; //0x012D
	float m_microphone_volume; //0x0130
	uint32_t m_voice_channel; //0x0134
	bool m_is_overriding_voice_proximity; //0x0138
	char pad_0139[7]; //0x0139
	float m_voice_proximity_x; //0x0140
	float m_voice_proximity_y; //0x0144
	float m_voice_proximity_z; //0x0148
	float m_voice_proximity_radius_maybe; //0x014C
	uint32_t unk_0150; //0x0150
	uint32_t m_vehicle_weapon_index; //0x0154
	bool m_has_vehicle_weapon_index; //0x0158
	uint32_t m_decor_count; //0x015C
	uint32_t m_decor_type[3]; //0x0160
	uint32_t m_decor_value[3]; //0x016C
	uint32_t m_decor_name_hash[3]; //0x0178
	bool m_friendly_fire_allowed; //0x0184
	bool unk_0185; //0x0185
	uint8_t m_current_garage_instance_index; //0x0186
	uint8_t m_current_property_id; //0x0187
	uint8_t unk_0188; //0x0188
	uint8_t unk_0189; //0x0189
	bool m_battle_aware; //0x018A
	bool m_vehicle_jump_down; //0x018B
	float m_weapon_defence_modifier; //0x018C
	float m_weapon_defence_modifier_2; //0x0190
	bool m_is_overriding_population_control_sphere; //0x0194
	char pad_0195[11]; //0x0195
	float m_population_control_sphere_x; //0x01A0
	float m_population_control_sphere_y; //0x01A4
	float m_population_control_sphere_z; //0x01A8
	uint16_t unk_01AC; //0x01AC
	uint16_t unk_01AE; //0x01AE
	uint16_t unk_01B0; //0x01B0
	bool pad_01B2; //0x01B2
	bool unk_01B3; //0x01B3
	bool m_no_collision; //0x01B4
	bool unk_01B5; //0x01B5
	bool unk_01B6; //0x01B6
	bool m_super_jump; //0x01B7
	bool unk_01B8; //0x01B8
	bool unk_01B9; //0x01B9
	uint16_t unk_01BA; //0x01BA
	uint32_t unk_01BC; //0x01BC
	float unk_01C0; //0x01C0
	float m_weapon_damage_modifier; //0x01C4 Divisor: 0x3F800000
	float m_melee_weapon_damage_modifier; //0x01C8 Divisor: 0x3F800000
	float unk_01CC; //0x01CC
	bool unk_01D0; //0x01D0
	char pad_01D1[15]; //0x01D1
	float unk_01E0; //0x01E0
	float unk_01E4; //0x01E4
	float unk_01E8; //0x01E8
	uint32_t unk_01EC; //0x01EC
	uint8_t unk_01F0; //0x01F0
	uint8_t unk_01F1; //0x01F1
	bool unk_01F2; //0x01F2
	uint8_t unk_01F3; //0x01F3
	bool unk_01F4; //0x01F4
	bool unk_01F5; //0x01F5
}; //Size: 0x01F8
static_assert(sizeof(CPlayerGameStateDataNode) == 0x1F8);
#pragma pack(pop)
#pragma pack(push,2)
class CPedGameStateDataNode : CSyncDataNodeInfrequent
{
public:
	bool m_weapon_component_something[12]; //0x0C0
	uint32_t m_weapon_component_hash[12]; //0x0CC
	uint32_t m_gadget_hash[3]; //0x0F8
	uint32_t unk_0104; //0x0104
	uint32_t unk_0108; //0x0108
	uint32_t unk_010C; //0x010C
	float unk_0110; //0x0110
	float unk_0114; //0x0114
	float unk_0118; //0x0118
	bool unk_011C; //0x011C
	char pad_011D[3]; //0x011D
	uint32_t m_arrest_state; //0x0120
	uint32_t m_death_state; //0x0124
	uint32_t m_weapon_hash; //0x0128
	uint32_t m_num_weapon_components; //0x012C
	uint32_t m_num_equiped_gadgets; //0x0130
	uint32_t m_seat; //0x0134
	uint32_t m_action_mode_override; //0x0138
	uint32_t unk_013C; //0x013C
	uint16_t m_vehicle; //0x0140
	uint16_t m_mount_id; //0x0142
	uint16_t m_custodian_id; //0x0144
	uint16_t unk_0146; //0x0146
	bool m_tint_index; //0x0148
	char pad_0149; //0x0149
	uint8_t unk_014A; //0x014A
	bool m_is_handcuffed; //0x014B
	bool m_can_preform_arrest; //0x014C
	bool m_can_preform_uncuff; //0x014D
	bool m_can_be_arrested; //0x014E
	bool m_is_in_custody; //0x014F
	char pad_0150; //0x0150
	bool m_weapon_exists; //0x0151
	bool m_weapon_visible; //0x0152
	bool m_weapon_has_ammo; //0x0153
	bool m_weapon_attach_left; //0x0154
	char pad_0155; //0x0155
	bool m_in_seat; //0x0156
	bool m_in_vehicle; //0x0157
	bool m_on_mount; //0x0158
	bool m_has_custodian_or_arrest_flags; //0x0159
	char pad_015A; //0x015A
	bool m_action_mode_enabled; //0x015B
	bool m_stealth_mode_enabled; //0x015C
	bool unk_015D; //0x015D
	bool unk_015E; //0x015E
	bool unk_015F; //0x015F
	bool unk_0160; //0x0160
	bool unk_0161; //0x0161
	bool unk_0162; //0x0162
	bool unk_0163; //0x0163
	bool unk_0164; //0x0164
	bool unk_0165; //0x0165
	bool unk_0166; //0x0166
	bool unk_0167; //0x0167
	bool unk_0168; //0x0168
	bool unk_0169; //0x0169
	bool unk_016A; //0x016A
	bool unk_016B; //0x016B
	bool unk_016C; //0x016C
	bool unk_016D; //0x016D
	bool unk_016E; //0x016E
	bool unk_016F; //0x016F
	bool unk_0170; //0x0170
	bool unk_0171; //0x0171
	bool unk_0172; //0x0172
}; //Size: 0x0174
static_assert(sizeof(CPedGameStateDataNode) == 0x178);
#pragma pack(pop)
#pragma pack(push, 4)
class CPhysicalAttachDataNode : CSyncDataNodeInfrequent
{
public:
	bool m_attached; //0x00C0
	bool unk_00C1; //0x00C1
	uint16_t m_attached_to; //0x00C2
	char pad_00C4[12]; //0x00C4
	rage::vector3 m_offset; //0x00D0
	char pad_00DC[4]; //0x00DC
	rage::vector4 m_orientation; //0x00E0
	rage::vector3 m_parent_offset; //0x00F0
	char pad_00FC[4]; //0x00FC
	uint16_t m_other_attach_bone; //0x0100
	uint16_t m_attach_bone; //0x0102
	uint32_t m_attach_flags; //0x0104
	bool m_allow_initial_separation; //0x0108
	char pad_00109[3]; //0x0109
	float unk_010C; //0x010C
	float unk_0110; //0x0110
	bool unk_0114; //0x0114
	bool unk_0115; //0x0115
	bool m_is_cargo_vehicle; //0x0116
}; //Size: 0x0118
static_assert(sizeof(CPhysicalAttachDataNode) == 0x118);
#pragma pack(pop)
#pragma pack(push,4)
class CPedAttachDataNode : CSyncDataNodeInfrequent
{
public:
	rage::vector3 m_offset; //0x00C0
	char pad_00CC[4]; //0x00CC
	rage::vector4 m_orientation; //0x00D0
	uint16_t m_attached_to; //0x00E0
	uint16_t m_attachment_bone; //0x00E2
	uint32_t m_attachment_flags; //0x00E4
	float m_heading_1; //0x00E8
	float m_heading_2; //0x00EC
	bool m_attached; //0x00F0
	bool unk_00F1; //0x00F1
};

static_assert(sizeof(CPedAttachDataNode) == 0xF4);
#pragma pack(pop)
#pragma pack(push, 4)
class CPedComponents
{
public:
	uint32_t m_component_bitset; //0x0
	char pad_0x4[4]; //0x4
	uint32_t unk_0x8[12]; //0x8
	uint32_t m_drawables[12]; //0x38
	uint32_t m_textures[12]; //0x68
	uint32_t m_palettes[12]; //0x98

	u32 get_drawable(int index)
	{
		if (m_component_bitset & (1 << index))
		{
			return m_drawables[index];
		}
		return 0;
	}

	u32 get_texture(i32 index)
	{
		if (m_component_bitset & (1 << index))
		{
			return m_textures[index];
		}
		return 0;
	}

	u32 get_palette(i32 index)
	{
		if (m_component_bitset & (1 << index))
		{
			return m_palettes[index];
		}
		return 0;
	}
};

static_assert(sizeof(CPedComponents) == 0xC8);
#pragma pack(pop)
#pragma pack(push, 4)
class CPlayerAppearanceDataNode : CSyncDataNodeInfrequent
{
public:
	uint32_t unk_00C0[56]; //0xC0
	class CPedComponents components; //0x1A0
	char pad_0268[8]; //0x268
	uint32_t unk_0270[6]; //0x270
	uint32_t unk_0288[6]; //0x288
	char pad_02A0[8]; //0x2A0
	float unk_02A8; //0x2A8
	uint8_t unk_02AC; //0x2AC
	uint8_t unk_02AD; //0x2AD
	char pad_02AE[26]; //0x2AE
	float m_shape_mix; //0x2C8
	float m_skin_mix; //0x2CC
	float m_third_mix; //0x2D0
	float unk_02D4; //0x2D4
	float unk_02D8[13]; //0x2D8
	float unk_030C[13]; //0x30C
	float unk_0340[20]; //0x340
	uint8_t unk_0390[13]; //0x390
	uint8_t unk_039D[13]; //0x39D
	uint8_t unk_03AA[13]; //0x3AA
	uint8_t m_shape_first; //0x3B7
	uint8_t m_shape_second; //0x3B8
	uint8_t m_shape_third; //0x3B9
	uint8_t m_skin_first; //0x3BA
	uint8_t m_skin_second; //0x3BB
	uint8_t m_skin_third; //0x3BC
	uint8_t unk_03BD[13]; //0x3BD
	uint8_t unk_03CA[11]; //0x3CA
	int16_t unk_03D6; //0x3D6
	uint8_t unk_03D8; //0x3D8
	uint8_t unk_03D9; //0x3D9
	char pad_03DA[1]; //0x3DA
	bool unk_03DB; //0x3DB
	bool unk_03DC; //0x3DC
	char pad_03DD[3]; //0x3DD
	uint32_t unk_03E0; //0x3E0
	uint32_t unk_03E4; //0x3E4
	uint32_t unk_03E8; //0x3E8
	uint32_t unk_03EC; //0x3EC
	uint32_t unk_03F0; //0x3F0
	float unk_03F4; //0x3F4
	float unk_03F8; //0x3F8
	float unk_03FC; //0x3FC
	uint32_t unk_0400; //0x400
	uint32_t unk_0404; //0x404
	uint32_t unk_0408; //0x408
	uint32_t unk_040C; //0x40C
	uint32_t unk_0410; //0x410
	bool unk_0414; //0x414
	bool unk_0415; //0x415
	bool unk_0416; //0x416
	bool unk_0417; //0x417
	bool unk_0418; //0x418
	bool unk_0419; //0x419
	uint32_t unk_041C; //0x41C
	uint32_t m_model_hash; //0x420
	uint32_t m_voice_hash; //0x424
	uint32_t m_phone_mode; //0x428
	uint32_t unk_042C; //0x42C
	uint8_t m_parachute_tint_index; //0x430
	uint8_t m_parachute_pack_tint_index; //0x431
	uint16_t m_respawn_object; //0x432
	bool m_has_head_blend_data; //0x434
	bool unk_0435; //0x435
	bool m_has_respawn_object; //0x436
	char pad_0437; //0x437
	uint32_t unk_0438_clip_maybe; //0x438
	uint32_t unk_043C; //0x43C
	uint32_t unk_0440; //0x440
	bool unk_0444; //0x444
	bool unk_0445; //0x445
	bool unk_0446; //0x446
	uint8_t unk_0447; //0x447
	uint16_t unk_0448; //0x448
	uint16_t unk_044A; //0x44A
	uint16_t unk_044C; //0x44C
	bool unk_044E; //0x44E
	bool unk_044F; //0x44F
	bool unk_0450; //0x450
	uint8_t unk_0451; //0x451
	uint32_t unk_0452; //0x452
	uint32_t unk_0456; //0x456
};

static_assert(sizeof(CPlayerAppearanceDataNode) == 0x45C);
#pragma pack(pop)
#pragma pack(push, 4)
class CVehicleProximityMigrationDataNode : CProjectBaseSyncDataNode
{
public:
	uint32_t m_max_occupants;
	bool m_has_occupants[16];
	int16_t m_occupants[16];
	char pad[16];
	bool m_override_position;
	char pad2[8];
	rage::vector3 m_position;
	rage::cvector3<int32_t> m_velocity;
	char pad3[352];
}; //Size: 0x0180
static_assert(sizeof(CVehicleProximityMigrationDataNode) == 0x288);
#pragma pack(pop)
class CObject : public rage::fwEntity
{
};

class Vector2 final
{
public:
	Vector2() = default;

	Vector2(float x, float y) : x(x), y(y)
	{
	}

public:
	alignas(8) float x{};
	alignas(8) float y{};

public:
	rage::vector2 deserialize()
	{
		return {x, y};
	}

	bool empty()
	{
		return x == 0.f && y == 0.f;
	}

public:
	Vector2 operator*(const float amount)
	{
		return {x * amount, y * amount};
	}

	Vector2 operator*(const Vector2 vec)
	{
		return {x * vec.x, y * vec.y};
	}

	Vector2 operator/(const float amount)
	{
		return {x / amount, y / amount};
	}

	Vector2 operator/(const Vector2 vec)
	{
		return {x / vec.x, y / vec.y};
	}

	Vector2 operator+(const float amount)
	{
		return {x + amount, y + amount};
	}

	Vector2 operator+(const Vector2 vec)
	{
		return {x + vec.x, y + vec.y};
	}

	Vector2 operator-(const float amount)
	{
		return {x - amount, y - amount};
	}

	Vector2 operator-(const Vector2 vec)
	{
		return {x - vec.x, y - vec.y};
	}

	Vector2& operator+=(const float& amount)
	{
		x += amount;
		y += amount;
		return *this;
	}

	Vector2& operator+=(const Vector2& vec)
	{
		x += vec.x;
		y += vec.y;
		return *this;
	}

	Vector2& operator-=(const float& amount)
	{
		x -= amount;
		y -= amount;
		return *this;
	}

	Vector2& operator-=(const Vector2& vec)
	{
		x -= vec.x;
		y -= vec.y;
		return *this;
	}

	Vector2& operator/=(const float& amount)
	{
		x /= amount;
		y /= amount;
		return *this;
	}

	Vector2& operator/=(const Vector2& vec)
	{
		x /= vec.x;
		y /= vec.y;
		return *this;
	}

	Vector2& operator*=(const float& amount)
	{
		x *= amount;
		y *= amount;
		return *this;
	}

	Vector2& operator*=(const Vector2& vec)
	{
		x *= vec.x;
		y *= vec.y;
		return *this;
	}
};

class Vector3
{
public:
	Vector3() = default;

	Vector3(float x, float y, float z) : x(x), y(y), z(z)
	{
	}

public:
	alignas(8) float x{};
	alignas(8) float y{};
	alignas(8) float z{};

public:
	rage::vector3 deserialize()
	{
		return {x, y, z};
	}

	bool empty()
	{
		return x == 0.f && y == 0.f && z == 0.f;
	}

	bool valid()
	{
		return x == 0.f || y == 0.f || z == 0.f;
	}

public:
	Vector3 operator*(const float amount)
	{
		return {x * amount, y * amount, z * amount};
	}

	Vector3 operator*(const Vector3 vec)
	{
		return {x * vec.x, y * vec.y, z * vec.z};
	}

	Vector3 operator/(const float amount)
	{
		return {x / amount, y / amount, z / amount};
	}

	Vector3 operator/(const Vector3 vec)
	{
		return {x / vec.x, y / vec.y, z / vec.z};
	}

	Vector3 operator+(const float amount)
	{
		return {x + amount, y + amount, z + amount};
	}

	Vector3 operator+(const Vector3 vec)
	{
		return {x + vec.x, y + vec.y, z + vec.z};
	}

	Vector3 operator-(const float amount)
	{
		return {x - amount, y - amount, z - amount};
	}

	Vector3 operator-(const Vector3 vec)
	{
		return {x - vec.x, y - vec.y, z - vec.z};
	}

	Vector3& operator+=(const float& amount)
	{
		x += amount;
		y += amount;
		z += amount;
		return *this;
	}

	Vector3& operator+=(const Vector3& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
		return *this;
	}

	Vector3& operator-=(const float& amount)
	{
		x -= amount;
		y -= amount;
		z -= amount;
		return *this;
	}

	Vector3& operator-=(const Vector3& vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		return *this;
	}

	Vector3& operator/=(const float& amount)
	{
		x /= amount;
		y /= amount;
		z /= amount;
		return *this;
	}

	Vector3& operator/=(const Vector3& vec)
	{
		x /= vec.x;
		y /= vec.y;
		z /= vec.z;
		return *this;
	}

	Vector3& operator*=(const float& amount)
	{
		x *= amount;
		y *= amount;
		z *= amount;
		return *this;
	}

	Vector3& operator*=(const Vector3& vec)
	{
		x *= vec.x;
		y *= vec.y;
		z *= vec.z;
		return *this;
	}

	bool operator==(const Vector3& vec)
	{
		return vec.x == x && vec.y == y && vec.z == z;
	}

	bool operator!=(const Vector3& vec)
	{
		return vec.x != x || vec.y != y || vec.z != z;
	}
};

class Vector4 final
{
public:
	Vector4() = default;

	Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
	{
	}

public:
	alignas(8) float x{};
	alignas(8) float y{};
	alignas(8) float z{};
	alignas(8) float w{};

public:
	rage::vector4 deserialize()
	{
		return {x, y, z, w};
	}

	bool empty()
	{
		return x == 0.f && y == 0.f && z == 0.f && w == 0.f;
	}

public:
	Vector4 operator*(const float amount)
	{
		return {x * amount, y * amount, z * amount, w * amount};
	}

	Vector4 operator*(const Vector4 vec)
	{
		return {x * vec.x, y * vec.y, z * vec.z, w * vec.w};
	}

	Vector4 operator/(const float amount)
	{
		return {x / amount, y / amount, z / amount, w / amount};
	}

	Vector4 operator/(const Vector4 vec)
	{
		return {x / vec.x, y / vec.y, z / vec.z, w / vec.w};
	}

	Vector4 operator+(const float amount)
	{
		return {x + amount, y + amount, z + amount, w + amount};
	}

	Vector4 operator+(const Vector4 vec)
	{
		return {x + vec.x, y + vec.y, z + vec.z, w + vec.w};
	}

	Vector4 operator-(const float amount)
	{
		return {x - amount, y - amount, z - amount, w - amount};
	}

	Vector4 operator-(const Vector4 vec)
	{
		return {x - vec.x, y - vec.y, z - vec.z, w - vec.w};
	}

	Vector4& operator+=(const float& amount)
	{
		x += amount;
		y += amount;
		z += amount;
		w += amount;
		return *this;
	}

	Vector4& operator+=(const Vector4& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
		return *this;
	}

	Vector4& operator-=(const float& amount)
	{
		x -= amount;
		y -= amount;
		z -= amount;
		w -= amount;
		return *this;
	}

	Vector4& operator-=(const Vector4& vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		w -= vec.w;
		return *this;
	}

	Vector4& operator/=(const float& amount)
	{
		x /= amount;
		y /= amount;
		z /= amount;
		w /= amount;
		return *this;
	}

	Vector4& operator/=(const Vector4& vec)
	{
		x /= vec.x;
		y /= vec.y;
		z /= vec.z;
		w /= vec.w;
		return *this;
	}

	Vector4& operator*=(const float& amount)
	{
		x *= amount;
		y *= amount;
		z *= amount;
		w *= amount;
		return *this;
	}

	Vector4& operator*=(const Vector4& vec)
	{
		x *= vec.x;
		y *= vec.y;
		z *= vec.z;
		w *= vec.w;
		return *this;
	}
};

inline Vector2 rage::vector2::serialize()
{
	return {x, y};
}

inline Vector2 rage::vector3::serialize_v2()
{
	return {x, y};
}

inline Vector3 rage::vector3::serialize()
{
	return {x, y, z};
}

inline Vector4 rage::vector4::serialize()
{
	return {x, y, z, w};
}

inline bool rage::rlGamerHandle::deserialize(datBitBuffer& buf)
{
	if ((m_platform = buf.Read<u8>(8)) != 3)
		return false;
	buf.ReadInt64((i64*)&m_rockstar_id, 64);
	m_flag = buf.Read<uint8_t>(8);
	return true;
}

inline bool rage::rlGamerHandle::serialize(datBitBuffer& buf)
{
	if (!buf.Write<u8>(m_platform, 8))
		return false;
	if (m_platform == 3)
	{
		buf.WriteQword(m_rockstar_id, 0x40);
		buf.Write<u8>(m_flag, 8);
		return true;
	}
	return false;
}
#pragma pack(push, 1)
class FriendInfo
{
public:
	char m_name[20]; //0x0000
	char pad_0014[36]; //0x0014
	uint64_t m_rockstar_id; //0x0038
	uint8_t unk_040; //0x0040
	char pad_0041[3]; //0x0041
	uint32_t m_friend_state; //0x0044
	char pad_0048[304]; //0x0048
	uint32_t m_is_joinable; //0x0178
	char pad_017C[4]; //0x017C
}; //Size: 0x0180
static_assert(sizeof(FriendInfo) == 0x180);

class FriendList
{
public:
	FriendInfo m_friends[250]; //0x0000
}; //Size: 0x17700
static_assert(sizeof(FriendList) == 0x17700);

class FriendRegistry
{
public:
	uint32_t m_friend_count; //0x0000
	char pad_0004[8]; //0x0004
	FriendList* m_friend_list; //0x000C

	FriendInfo* get(uint32_t idx)
	{
		return &m_friend_list->m_friends[idx];
	}
}; //Size: 0x0014
static_assert(sizeof(FriendRegistry) == 0x14);
#pragma pack(pop)
class CNonPhysicalPlayerData : public rage::nonPhysicalPlayerDataBase
{
public:
	int32_t m_bubble_id; //0x0008
	int32_t m_player_id; //0x000C
	rage::vector3 m_position; //0x0010
};

class CHandlingObject
{
public:
	virtual ~CHandlingObject() = NULL;
	virtual rage::parStructure* parser_GetStructure() = NULL;
}; //Size: 0x0008
static_assert(sizeof(CHandlingObject) == 0x8);

class CBaseSubHandlingData : public CHandlingObject
{
public:
	virtual eHandlingType GetHandlingType() = NULL;
	virtual void OnPostLoad() = NULL;
}; //Size: 0x0008
static_assert(sizeof(CBaseSubHandlingData) == 0x8);

class CAdvancedData
{
public:
	virtual ~CAdvancedData() = NULL;

	int m_slot; //0x0008
	int m_index; //0x000C
	float m_value; //0x0010
}; //Size: 0x0014
class CCarHandlingData : public CBaseSubHandlingData
{
public:
	float m_back_end_popup_car_impulse_mult; //0x0008
	float m_back_end_popup_building_impulse_mult; //0x000C
	float m_back_end_popup_max_delta_speed; //0x0010
	float m_toe_front; //0x0014
	float m_toe_rear; //0x0018
	float m_camber_front; //0x001C
	float m_camber_rear; //0x0020
	float m_castor; //0x0024
	float m_engine_resistance; //0x0028
	float m_max_drive_bias_transfer; //0x002C
	float m_jumpforce_scale; //0x0030
	float unk_0034; //0x0034
	uint32_t unk_0038; //0x0038
	uint32_t m_advanced_flags; //0x003C
	rage::atArray<CAdvancedData> m_advanced_data; //0x0040
};

class CHandlingData
{
public:
	char pad_0000[8]; //0x0000
	uint32_t m_model_hash; //0x0008
	float m_mass; //0x000C
	float m_initial_drag_coeff; //0x0010
	float m_downforce_multiplier; //0x0014
	float m_popup_light_rotation; //0x0018
	char pad_001C[4]; //0x001C
	rage::vector3 m_centre_of_mass; //0x0020
	char pad_002C[4]; //0x002C
	rage::vector3 m_inertia_mult; //0x0030
	char pad_003C[4]; //0x003C
	float m_buoyancy; //0x0040
	float m_drive_bias_rear; //0x0044
	float m_drive_bias_front; //0x0048
	float m_acceleration; //0x004C
	uint8_t m_initial_drive_gears; //0x0050
	char pad_0051[3]; //0x0051
	float m_drive_inertia; //0x0054
	float m_upshift; //0x0058
	float m_downshift; //0x005C
	float m_initial_drive_force; //0x0060
	float m_drive_max_flat_velocity; //0x0064
	float m_initial_drive_max_flat_vel; //0x0068
	float m_brake_force; //0x006C
	char pad_0070[4]; //0x0070
	float m_brake_bias_front; //0x0074
	float m_brake_bias_rear; //0x0078
	float m_handbrake_force; //0x007C
	float m_steering_lock; //0x0080
	float m_steering_lock_ratio; //0x0084
	float m_traction_curve_max; //0x0088
	float m_traction_curve_lateral; //0x008C
	float m_traction_curve_min; //0x0090
	float m_traction_curve_ratio; //0x0094
	float m_curve_lateral; //0x0098
	float m_curve_lateral_ratio; //0x009C
	float m_traction_spring_delta_max; //0x00A0
	float m_traction_spring_delta_max_ratio; //0x00A4
	float m_low_speed_traction_loss_mult; //0x00A8
	float m_camber_stiffness; //0x00AC
	float m_traction_bias_front; //0x00B0
	float m_traction_bias_rear; //0x00B4
	float m_traction_loss_mult; //0x00B8
	float m_suspension_force; //0x00BC
	float m_suspension_comp_damp; //0x00C0
	float m_suspension_rebound_damp; //0x00C4
	float m_suspension_upper_limit; //0x00C8
	float m_suspension_lower_limit; //0x00CC
	float m_suspension_raise; //0x00D0
	float m_suspension_bias_front; //0x00D4
	float m_suspension_bias_rear; //0x00D8
	float m_anti_rollbar_force; //0x00DC
	float m_anti_rollbar_bias_front; //0x00E0
	float m_anti_rollbar_bias_rear; //0x00E4
	float m_roll_centre_height_front; //0x00E8
	float m_roll_centre_height_rear; //0x00EC
	float m_collision_damage_mult; //0x00F0
	float m_weapon_damamge_mult; //0x00F4
	float m_deformation_mult; //0x00F8
	float m_engine_damage_mult; //0x00FC
	float m_petrol_tank_volume; //0x0100
	float m_oil_volume; //0x0104
	char pad_0108[4]; //0x0108
	rage::vector3 m_seat_offset_dist; //0x010C
	uint32_t m_monetary_value; //0x0118
	char pad_011C[8]; //0x011C
	uint32_t m_model_flags; //0x0124
	uint32_t m_handling_flags; //0x0128
	uint32_t m_damage_flags; //0x012C
	char pad_0130[12]; //0x0130
	uint32_t m_ai_handling_hash; //0x013C
	char pad_140[24]; //0x140
	rage::atArray<CBaseSubHandlingData*> m_sub_handling_data; //0x158
}; //Size: 0x0160
static_assert(sizeof(CHandlingData) == 0x168);
#pragma pack(push, 1)
class CVehicleModelInfo : public rage::CBaseModelInfo
{
public:
	char pad_00B0[72]; //0x00B0
	uint8_t m_primary_color_combinations[25]; //0x00F8
	uint8_t m_secondary_color_combinations[25]; //0x0111
	uint8_t m_unk_color_combos1[25]; //0x012A
	uint8_t m_unk_color_combos2[25]; //0x0143
	uint8_t m_interior_color_combinations[25]; //0x015C
	uint8_t m_dashboard_color_combinations[25]; //0x0175
	char pad_018E[266]; //0x018E
	char m_name[12]; //0x0298
	char m_manufacturer[12]; //0x02A4
	char pad_02B0[8]; //0x02B0
	uint16_t m_modkits_count; //0x02B8
	char pad_02BA[30]; //0x02BA
	uint8_t m_passenger_capacity; //0x02D8
	char pad_02D9[103]; //0x02D9
	eVehicleType m_vehicle_type; //0x0340
	uint32_t m_unk_vehicle_type; //0x0344
	uint32_t m_diffuse_tint; //0x0348
	char pad_034C[20]; //0x034C
	rage::vector3 m_first_person_driveby_ik_offset; //0x0360
	char pad_036C[4]; //0x036C
	rage::vector3 m_first_person_driveby_unarmed_ik_offset; //0x0370
	char pad_037C[20]; //0x037C
	rage::vector3 m_first_person_driveby_right_passenger_ik_offset; //0x0390
	char pad_039C[36]; //0x039C
	rage::vector3 m_first_person_driveby_right_passenger_unarmed_ik_offset; //0x03C0
	char pad_03CC[4]; //0x03CC
	rage::vector3 m_first_person_projectile_driveby_ik_offset; //0x03D0
	char pad_03DC[4]; //0x03DC
	rage::vector3 m_first_person_projectile_driveby_passenger_ik_offset; //0x03E0
	char pad_03EC[52]; //0x03EC
	rage::vector3 m_first_person_mobile_phone_offset; //0x0420
	char pad_042C[4]; //0x042C
	rage::vector3 m_first_person_passenger_mobile_phone_offset; //0x0430
	char pad_043C[20]; //0x043C
	rage::vector3 m_pov_camera_offset; //0x0450
	char pad_045C[36]; //0x045C
	float m_pov_camera_vertical_adjustement_for_rollcage; //0x0480
	char pad_0484[8]; //0x0484
	float m_wheel_scale; //0x048C
	float m_wheel_scale_rear; //0x0490
	float m_default_health; //0x0494
	char pad_0498[4]; //0x0498
	float m_steer_wheel_multiplier; //0x049C
	char pad_04A0[168]; //0x04A0
	uint32_t m_model_class; //0x0548
	char pad_0550[8]; //0x550
	float m_min_seat_height; //0x0554
	char pad_0558[40]; //0x0558
	uint32_t m_is_jetski; //0x0580
	char pad_0584[7]; //0x0584
	uint32_t m_ability_flags; //0x058B
	char pad_058F[17]; //0x0584

	uint32_t get_class()
	{
		return m_model_class & 0x1F;
	}

	bool is_class(eModelClass modelClass)
	{
		return get_class() == static_cast<uint32_t>(modelClass);
	}

	bool set_ability_flags(eAbilityFlags flag, bool value)
	{
		value ? m_ability_flags |= static_cast<int>(flag) : m_ability_flags &= ~static_cast<int>(flag);
	}
}; //Size: 0x05A0
static_assert(sizeof(CVehicleModelInfo) == 0x5A0);
#pragma pack(pop)
#pragma pack(push, 1)
class CVehicle : public rage::CPhysical
{
public:
	char pad_02EC[12]; //0x02EC
	uint8_t m_boost_state; //0x02F8
	char pad_02F9[2]; //0x02F9
	uint8_t m_boost_allow_recharge; //0x02FB
	char pad_02FC[4]; //0x02FC
	float m_boost; //0x0300
	float m_rocket_recharge_speed; //0x304
	float m_mkii_rocket_recharge_speed; //0x308
	char pad_030C[132]; //0x030C
	float m_jump_boost_charge; //0x0390
	char pad_0394[1084]; //0x0394
	rage::vector3 m_velocity; //0x07D0
	char pad_07DC[68]; //0x07DC
	float m_body_health; //0x0820
	float m_petrol_tank_health; //0x0824
	char pad_0828[72]; //0x0828
	uint16_t m_next_gear; //0x0870
	uint16_t m_current_gear; //0x0872
	char pad_0874[2]; //0x0874
	uint8_t m_top_gear; //0x0876
	char pad_0877[57]; //0x0877
	float m_rpm; //0x08D0
	char pad_08D4[52]; //0x8D4
	float m_engine_health; //0x08E8
	char pad_08EC[44]; //0x08EC
	CHandlingData* m_handling_data; //0x0918
	char pad_0920[2]; //0x0920
	uint8_t m_is_drivable; //0x0922
	uint8_t m_tyres_can_burst; //0x0923
	uint8_t m_deform_god; //0x0924
	char pad_0925[179]; //0x0925
	float m_dirt_level; //0x09D8
	char pad_09DC[194]; //0x09DC
	uint8_t m_is_targetable; //0x0A9E
	char pad_0A9F[413]; //0x0A9F
	float m_gravity; //0x0C3C
	uint8_t m_max_passengers; //0x0C40
	char pad_0C41; //0x0C41
	uint8_t m_num_of_passengers; //0x0C42
	char pad_0C43[5]; //0x0C43
	class CPed* m_driver; //0x0C48
	class CPed* m_passengers[15]; //0x0C50
	class CPed* m_last_driver; //0x0CC8
	char pad_0CD0[1696]; //0x0CD0
	uint32_t m_door_lock_status; //0x1370
	char pad_1374[220]; //0x1374

	float get_speed()
	{
		return sqrt(m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y + m_velocity.z * m_velocity.z);
	}

	CVehicleModelInfo* get_model_info()
	{
		return reinterpret_cast<CVehicleModelInfo*>(m_model_info);
	}
}; //Size: 0x1450
static_assert(sizeof(CVehicle) == 0x1450);
#pragma pack(pop)
class CItemInfo
{
public:
	char pad_0000[16]; //0x0000
	uint32_t m_name; //0x0010
	uint32_t m_model; //0x0014
	uint32_t m_audio; //0x0018
	uint32_t m_slot; //0x001C
}; //Size: 0x0020
static_assert(sizeof(CItemInfo) == 0x20);

class CAmmoInfo : public CItemInfo
{
public:
	int32_t m_ammo_max; //0x0020
	int32_t m_ammo_max_50; //0x0024
	int32_t m_ammo_max_100; //0x0028
	int32_t m_ammo_max_mp; //0x002C
	int32_t m_ammo_max_50_mp; //0x0030
	int32_t m_ammo_max_100_mp; //0x0034
	uint32_t m_ammo_flags; //0x0038
	uint32_t m_ammo_special_type; //0x003C
	char pad_0040[24]; //0x0040
	float m_missile_speed; //0x0058
	char pad_005C[284]; //0x005C
	uint32_t m_lock_on_time; //0x0178
}; //Size: 0x017C
static_assert(sizeof(CAmmoInfo) == 0x17C);

class CAimingInfo
{
public:
	uint32_t m_name; //0x0000
	float m_heading_limit; //0x0004
	float m_sweep_pitch_min; //0x0008
	float m_sweep_pitch_max; //0x000C
}; //Size: 0x0010
static_assert(sizeof(CAimingInfo) == 0x10);

class CForceInfo
{
public:
	int32_t m_bone_tag; //0x0000
	float m_front; //0x0004
	float m_back; //0x0008
}; //Size: 0x000C
static_assert(sizeof(CForceInfo) == 0xC);

class CWeaponInfo : public CItemInfo
{
public:
	enum eDamageType m_damage_type; //0x0020
	char pad_0024[48]; //0x0024
	enum eFireType m_fire_type; //0x0054
	enum eWheelSlot m_wheel_slot; //0x0058
	uint32_t m_group; //0x005C
	class CAmmoInfo* m_ammo_info; //0x0060
	class CAimingInfo* m_aiming_info; //0x0068
	uint32_t m_clip_size; //0x0070
	float m_accuracy_spread; //0x0074
	float m_accurate_mode_accuracy_modifier; //0x0078
	float m_run_and_gun_accuracy; //0x007C
	float m_run_and_gun_min_accuracy; //0x0080
	float m_recoil_accuracy_max; //0x0084
	float m_recoil_error_time; //0x0088
	float m_recoil_recovery_rate; //0x008C
	float m_recoil_accuracy_to_allow_headshot_ai; //0x0090
	float m_min_headshot_distance_ai; //0x0094
	float m_max_headshot_distance_ai; //0x0098
	float m_headshot_damage_modifier_ai; //0x009C
	float m_recoil_accuracy_to_allow_headshot_player; //0x00A0
	float m_min_headshot_distance_player; //0x00A4
	float m_max_headshot_distance_player; //0x00A8
	float m_headshot_damage_modifier_player; //0x00AC
	float m_damage; //0x00B0
	float m_damage_time; //0x00B4
	float m_damage_time_in_vehicle; //0x00B8
	float m_damage_time_in_vehicle_headshot; //0x00BC
	float N000008F9; //0x00C0
	uint32_t N00000898; //0x00C4
	float m_hit_limbs_damage_modifier; //0x00C8
	float m_network_hit_limbs_damage_modifier; //0x00CC
	float m_lightly_armoured_damage_modifier; //0x00D0
	float m_vehicle_damage_modifier; //0x00D4
	float m_force; //0x00D8
	float m_force_on_ped; //0x00DC
	float m_force_on_vehicle; //0x00E0
	float m_force_on_heli; //0x00E4
	char pad_00E8[16]; //0x00E8
	float m_force_max_strength_mult; //0x00F8
	float m_force_falloff_range_start; //0x00FC
	float m_force_falloff_range_end; //0x0100
	float m_force_falloff_range_min; //0x0104
	float m_project_force; //0x0108
	float m_frag_impulse; //0x010C
	float m_penetration; //0x0110
	float m_vertical_launch_adjustment; //0x0114
	float m_drop_forward_velocity; //0x0118
	float m_speed; //0x011C
	uint32_t m_bullets_in_batch; //0x0120
	float m_batch_spread; //0x0124
	float m_reload_time_mp; //0x0128
	float m_reload_time_sp; //0x012C
	float m_vehicle_reload_time; //0x0130
	float m_anim_reload_time; //0x0134
	int32_t m_bullets_per_anime_loop; //0x0138
	float m_time_between_shots; //0x013C
	float m_time_left_between_shots_where_should_fire_is_cached; //0x0140
	float m_spinup_time; //0x0144
	float m_spin_time; //0x0148
	float m_spindown_time; //0x014C
	float m_alternate_wait_time; //0x0150
	char pad_0154[296]; //0x0154
	float m_network_player_damage_modifier; //0x027C
	float m_network_ped_damage_modifier; //0x0280
	float m_network_headshot_modifier; //0x0284
	float m_lock_on_range; //0x0288
	float m_weapon_range; //0x028C
	char pad_0290[8]; //0x0290
	float m_damage_fall_off_range_min; //0x0298
	float m_damage_fall_off_range_max; //0x029C
	float m_damage_fall_off_modifier; //0x02A0
	char pad_02A4[64]; //0x02A4
	uint32_t m_recoil_shake_hash; //0x02E4
	uint32_t m_recoil_shake_hash_first_person; //0x02E8
	float m_min_time_between_recoil_shakes; //0x02EC
	float m_recoil_shake_amplitude; //0x02F0
	float m_explosion_shake_amplitude; //0x02F4
}; //Size: 0x02F8
static_assert(sizeof(CWeaponInfo) == 0x2F8);

class CPedWeaponManager
{
public:
	char pad_0000[16]; //0x0000
	class CPed* m_owner; //0x0010
	uint32_t m_selected_weapon_hash; //0x0018
	char pad_001C[4]; //0x001C
	class CWeaponInfo* m_weapon_info; //0x0020
	char pad_0028[72]; //0x0028
	class CWeaponInfo* m_vehicle_weapon_info; //0x0070
	class CObject* m_weapon_object; //0x0078
}; //Size: 0x0080
static_assert(sizeof(CPedWeaponManager) == 0x80);

class CPedFactory
{
public:
	enum class PedCreateFlags
	{
		IS_NETWORKED = (1 << 0),
		IS_PLAYER = (1 << 1)
	};

	virtual ~CPedFactory() = default;
	virtual CPed* CreatePed(std::uint8_t* flags, std::uint16_t* model_index, rage::matrix44* matrix,
	                        bool default_component_variation, bool register_network_object, bool give_default_loadout,
	                        bool, bool) = 0; //0x08
	virtual CPed* CreateClone(std::uint8_t* flags, std::uint16_t* model_index, rage::matrix44* matrix,
	                          bool default_component_variation, bool, bool register_network_object, bool) = 0; //0x10
	virtual CPed* ClonePed(CPed* ped, bool register_network_object, bool link_blends, bool clone_compressed_damage) = 0;
	//0x18
	virtual CPed* ClonePedToTarget(CPed* source, CPed* target, bool clone_compressed_damage) = 0; //0x20
	virtual CPed* CreatePlayer(std::uint8_t* flags, std::uint16_t model_index, rage::matrix44* matrix,
	                           CPlayerInfo* player_info) = 0; //0x28
	virtual void DestroyPed(CPed* ped) = 0; //0x30

	class CPed* m_local_ped; //0x0008
}; //Size: 0x0010
static_assert(sizeof(CPedFactory) == 0x10);

class CPedModelInfo : public rage::CBaseModelInfo
{
public:
}; //Size: 0x00B0
static_assert(sizeof(CPedModelInfo) == 0xB0);

class CPedBoneInfo
{
public:
	rage::vector3 m_model_coords; //0x0000
	char pad_000C[4]; //0x000C
}; //Size: 0x0010
static_assert(sizeof(CPedBoneInfo) == 0x10);

class CPedIntelligence
{
public:
	char pad_0000[632]; //0x0000
	float m_oxygen_time; //0x0278
}; //Size: 0x027C
static_assert(sizeof(CPedIntelligence) == 0x27C);
#pragma pack(push, 8)
class CPedInventory : rage::atRTTI<CPedInventory>
{
public:
	uint64_t unk_0008; //0x0008
	class CPed* m_ped; //0x0010
	uint64_t unk_0018; //0x0018
	uint32_t unk_0020; //0x0020
	uint64_t unk_0028; //0x0028
	uint64_t unk_0030; //0x0030
	uint32_t unk_0038; //0x0038
	char pad_003C[4]; //0x003C
	uint8_t unk_0040; //0x0040
	char pad_0041[7]; //0x0041
	uint64_t unk_0048; //0x0048
	uint32_t unk_0050; //0x0050
	uint64_t unk_0058; //0x0058
	uint64_t unk_0060; //0x0060
	uint32_t unk_0068; //0x0068
	char pad_006C[4]; //0x006C
	uint8_t unk_0070; //0x0070
	char pad_0071[7]; //0x0071
	bool m_infinite_ammo : 1; //0x0078
	bool m_infinite_clip : 1; //0x0078
	char pad_0079[7]; //0x0079
	uint64_t unk_0080; //0x0080
}; //Size: 0x0088
static_assert(sizeof(CPedInventory) == 0x88);
#pragma pack(pop)
#pragma pack(push, 1)
class CPed : public rage::CPhysical
{
public:
	char pad_02EC[20]; //0x0000
	rage::vector3 m_velocity; //0x0300
	char pad_030C[260]; //0x030C
	CPedBoneInfo m_bone_info[9]; //0x0410
	char pad_04A0[2160]; //0x04A0
	CVehicle* m_vehicle; //0x0D10
	char pad_0D18[896]; //0x0D18
	uint32_t m_ped_type; //0x1098
	char pad_109C[4]; //0x109C
	CPedIntelligence* m_ped_intelligence; //0x10A0
	CPlayerInfo* m_player_info; //0x10A8
	CPedInventory* m_inventory; //0x10B0
	CPedWeaponManager* m_weapon_manager; //0x10B8
	char pad_10C0[892]; //0x10C0
	uint8_t m_seatbelt; //0x143C
	char pad_143D[13]; //0x143D
	uint8_t m_can_switch_weapon; //0x144A
	uint8_t m_ped_task_flag; //0x144B
	char pad_144C[4]; //0x144C
	uint8_t m_forced_aim; //0x1450
	char pad_1451[187]; //0x1451
	float m_armour; //0x150C
	float unk_health_threshold; //0x1510
	float m_fatigued_health_threshold; //0x1514
	float m_injured_health_threshold; //0x1518
	float m_dying_health_threshold; //0x151C
	float m_hurt_health_threshold; //0x1520
	char pad_1524[240]; //0x1524
	uint16_t m_cash; //0x1614
	char pad_1616[842]; //0x1616
	uint8_t m_fired_sticky_bombs; //0x1960
	uint8_t m_fired_unk; //0x1961
	uint8_t m_fired_flares; //0x1962
	uint8_t m_fired_unk_2; //0x1963

	void set_ped_transformation(rage::vector2 size, rage::vector2 lean)
	{
		m_transformation_matrix.data[0][0] = size.x; //0x60 - Width
		m_transformation_matrix.data[2][0] = lean.x; //0x80 - Front Lean
		m_transformation_matrix.data[2][3] = lean.y; //0x80 - Back Lean
		m_transformation_matrix.data[2][2] = size.y; //0x88 - Height
	}

	float get_speed()
	{
		return sqrt(m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y + m_velocity.z * m_velocity.z);
	}

	rage::vector3 get_bone_coords(ePedBoneType type)
	{
		rage::vector3 world_coords;
		model_to_world(m_bone_info[static_cast<uint32_t>(type)].m_model_coords, world_coords);
		return world_coords;
	}

	bool is_current_task(ePedTask flag)
	{
		return m_ped_task_flag & static_cast<int>(flag);
	}

	bool on_foot()
	{
		return is_current_task(ePedTask::Foot) && !is_current_task(ePedTask::Driving);
	}

	bool can_be_ragdolled()
	{
		return m_ped_type & static_cast<int>(ePedType::Ragdogable);
	}

	uint32_t get_ped_type()
	{
		return m_ped_type << 11 >> 25;
	}

	bool has_seatbelt()
	{
		return m_seatbelt & 0x3;
	}

	void forced_aim(bool toggle)
	{
		m_forced_aim ^= (m_forced_aim ^ -static_cast<char>(toggle)) & 0x20;
	}

	void set_damage_bit(eEntityProofs flag, bool set)
	{
		set ? m_damage_bits |= static_cast<uint32_t>(flag) : m_damage_bits &= ~static_cast<uint32_t>(flag);
	}
}; //Size: 0x1964
static_assert(sizeof(CPed) == 0x1964);
#pragma pack(pop)
#pragma pack(push, 4)
class CPlayerInfo
{
public:
	char pad_0000[32]; //0x0000
	class rage::rlGamerInfo m_gamer_info; //0x0020
	char pad_0110[124]; //0x0110
	float m_walk_speed; //0x018C
	char pad_0190[56]; //0x0190
	float m_swim_speed; //0x01C8
	char pad_01CC[20]; //0x01CC
	uint32_t m_water_proof; //0x01E0
	char pad_01E4[10]; //0x01E4
	uint16_t m_maxarmour; //0x01EE
	char pad_01F0[62]; //0x01F0
	uint32_t m_game_state; //0x0230
	char pad_0234[12]; //0x0234
	class CPed* m_ped; //0x0240
	char pad_0248[40]; //0x0248
	uint32_t m_frame_flags; //0x0270
	char pad_0274[52]; //0x0274
	uint32_t m_player_controls; //0x02A8
	char pad_02AC[1248]; //0x02AC
	float m_wanted_can_change; //0x078C
	char pad_0790[304]; //0x0790
	uint32_t m_npc_ignore; //0x08C0
	char pad_08C4[12]; //0x08C4
	bool m_is_wanted; //0x08D0
	char pad_08D1[7]; //0x08D1
	uint32_t m_wanted_level; //0x08D8
	uint32_t m_wanted_level_display; //0x08DC
	char pad_08E0[1120]; //0x08E0
	float m_run_speed; //0x0D40
	float m_stamina; //0x0D44
	float m_stamina_regen; //0x0D48
	char pad_0D4C[16]; //0x0D4C
	float m_weapon_damage_mult; //0x0D5C
	float m_weapon_defence_mult; //0x0D60
	char pad_0D64[4]; //0x0D64
	float m_melee_weapon_damage_mult; //0x0D68
	float m_melee_damage_mult; //0x0D6C
	float m_melee_defence_mult; //0x0D70
	char pad_0D74[8]; //0x0D74
	float m_melee_weapon_defence_mult; //0x0D7C
}; //Size: 0x0D80
static_assert(sizeof(CPlayerInfo) == 0xD80);
#pragma pack(pop)
#pragma pack(push, 1)
class ClanData
{
public:
	int64_t m_clan_member_id; //0x0000
	int64_t m_clan_id; //0x0008
	int32_t m_clan_color; //0x0010
	int32_t m_clan_member_count; //0x0014
	int32_t m_clan_created_time; //0x0018
	bool m_is_system_clan; //0x001C
	bool m_is_clan_open; //0x001D
	char m_clan_name[25]; //0x001E
	char m_clan_tag[5]; //0x0037
	char m_clan_motto[65]; //0x003C
	char pad_007D[3]; //0x007D
	int64_t m_clan_id_2; //0x0080
	char m_clan_rank_name[25]; //0x0088
	char pad_00A1[3]; //0x00A1
	int32_t m_clan_rank_order; //0x00A4
	int64_t m_clan_rank_flags; //0x00A8
	char unk_00B0[8]; //0x00B0
}; //Size: 0x00B8
static_assert(sizeof(ClanData) == 0xB8);
#pragma pack(pop)
#pragma pack(push, 8)
class CNetGamePlayer : public rage::netPlayer
{
public:
    void* m_unk;
    CPlayerInfo* m_player_info; //0x00A0
    uint32_t m_matchmaking_group; //0x0008
    bool m_is_spectating; //0x000C
    char pad_00AD[3]; //0x000AD
    uint64_t unk_00B0; //0x00B0
    char unk_00B8; //0x00B8
    char pad_00B9[3]; //0x00B9
    uint32_t unk_00BC; //0x00BC
    uint32_t unk_00C0; //0x00C0
    char pad_00C4[4]; //0x00C4
    ClanData m_clan_data; //0x00C8
    char m_crew_rank_title[25]; //0x0180
    bool m_is_rockstar_dev; //0x0199
    bool m_is_rockstar_qa; //0x019A
    bool m_is_cheater; //0x019B
    uint32_t unk_019C; //0x019C
    uint16_t unk_01A0; //0x01A0
    char unk_01A2; //0x01A2
    char pad_01A3; //0x01A3
    uint32_t m_phone_explosion_vehicle_net_id; //0x01A4
    uint16_t unk_01A8; //0x01A8
    bool m_has_started_transition; //0x01AA
    char pad_01AB[5]; //0x01AB
    rage::rlSessionInfo m_transition_session_info; //0x01A3
    char pad_022D[16]; //0x022D
    void* m_unk2;
    uint64_t unk_0230; //0x0230
    uint64_t unk_0238; //0x0238
    uint32_t m_mute_count; //0x0240
    uint32_t m_mute_talkers_count; //0x0244
    char pad_0248[5]; //0x0248
    bool m_have_communication_privileges; //0x024D
    uint16_t unk_024E; //0x024E
    uint16_t unk_0250; //0x0250
    char pad_0252[2]; //0x0252
    uint32_t m_cheat_report_ids[20]; //0x0254
    uint32_t m_num_cheat_reports; //0x02A4
    uint8_t unk_02A8; //0x02A8
    char pad_02A9[3]; //0x02A9
    uint32_t unk_02AC; //0x02AC
    char unk_02B0; //0x02B0
    char pad_02B1[3]; //0x02B1
    uint32_t unk_02B4; //0x02B4
    uint32_t m_account_id; //0x02B4
    uint32_t m_unk_02BC; //0x02BC
}; //Size: 0x02C0
static_assert(sizeof(CNetGamePlayer) == 0x330);
#pragma pack(pop)
#pragma pack(push, 2)
class CNetworkPlayerMgr : public rage::netPlayerMgrBase
{
public:
	CNetGamePlayer m_net_players[32]; //0x08E0
	uint64_t unk_60E0; //0x60E0
	uint64_t unk_60E8; //0x60E8
	uint64_t unk_60F0; //0x60F0
	uint64_t unk_60F8; //0x60F8
	CNetGamePlayer m_net_players_2[32]; //0x6100
	uint64_t unk_B900; //0xB900
	uint64_t unk_B908; //0xB908
	uint64_t unk_B910; //0xB910
	uint64_t unk_B918; //0xB918
	uint64_t unk_B920; //0xB920
	uint64_t unk_B928; //0xB928
	uint64_t unk_B930; //0xB930
	uint32_t unk_B938; //0xB938
	char pad_B93C[3]; //0xB93C
	bool unk_B93F; //0xB93F
	uint32_t unk_B940; //0xB940
	uint32_t unk_B944; //0xB944
	uint16_t unk_B948; //0xB948
}; //Size: 0xB94A
static_assert(sizeof(CNetworkPlayerMgr) == 0xD552);
#pragma pack(pop)
class GtaThread : public rage::scrThread
{
public:
	uint32_t m_script_hash; //0x0128
	uint32_t m_pointer_count_cleanup; //0x012C
	uint32_t m_unk_130; //0x0130
	uint32_t m_unk_134; //0x0134
	uint32_t m_unk_138; //0x0138
	int32_t m_unk_13C; //0x013C
	int32_t m_instance_id; //0x0140
	uint32_t m_base_instance_id; //0x0144
	uint8_t m_flag; //0x0148
	bool m_safe_for_network_game; //0x0149
	bool m_allow_host_broadcast; //0x014A
	bool m_unk_14B; //0x014A
	bool m_is_minigame_script; //0x014C
	char pad_014D[2]; //0x014D
	bool m_can_be_paused; //0x014F
	bool m_can_remove_blips_from_other_scripts; //0x0150
	bool unk_0151; //0x0151
	bool m_allow_attachment; //0x0152
	bool m_detached; //0x0153
	uint8_t m_flag_2; //0x0154
	char pad_0155[11]; //0x0155
};

static_assert(sizeof(GtaThread) == 0x160);

class CNetworkObjectMgr : public rage::netObjectMgrBase
{
};
#pragma pack(push, 1)
class HashNode
{
public:
	int32_t m_hash; //0x0000
	uint16_t m_idx; //0x0004
	char pad_0006[2]; //0x0006
	HashNode* m_next; //0x0008
}; //Size: 0x0010
static_assert(sizeof(HashNode) == 0x10);

template <typename T>
class HashTable
{
public:
	T* m_data; //0x0000
	uint16_t m_size; //0x0008
	char pad_000A[14]; //0x000A
	uint64_t m_item_size; //0x0018
	char pad_0020[64]; //0x0020
	HashNode** m_lookup_table; //0x0060
	uint16_t m_lookup_key; //0x0068
}; //Size: 0x006A
static_assert(sizeof(HashTable<void*>) == 0x6A);
#pragma pack(pop)
class CPickup
{
public:
	char pad_0000[48]; //0x0000
	class CNavigation* m_navigation; //0x0030
	char pad_0038[88]; //0x0083
	rage::vector3 m_position; //0x0090
	char pad_009C[1012]; //0x009C
	int32_t m_money; //0x0490
	char pad_0484[260]; //0x0484
}; //Size: 0x0588
class CPickupHandle
{
public:
	CPickup* m_pointer; //0x0000 
	int32_t m_handle; //0x0008 
	char pad_0000C[4]; //0x000C
}; //Size: 0x0010
class CPickupList
{
public:
	CPickupHandle m_array[73]; //0x0000 
	uintptr_t addr(size_t index)
	{
		uintptr_t addr = reinterpret_cast<uintptr_t>(static_cast<void*>(this)) + index * 0x10;
		return *reinterpret_cast<uintptr_t*>(addr);
	}
}; //Size=0x0490
class CPickupInterface
{
public:
	char pad_00000[0x100]; //0x0000
	CPickupList* m_list; //0x0100 
	int32_t m_size; //0x0108 
	int32_t m_count; //0x0110 
	CPickup* get(const size_t& index)
	{
		if (index < m_size)
			return m_list->m_array[index].m_pointer;
		return nullptr;
	}
}; //Size: 0x0114
class CObjectHandle
{
public:
	CObject* m_pointer; //0x0000
	int32_t m_handle; //0x0008
	char pad_000C[4]; //0x000C
}; //Size: 0x0010
static_assert(sizeof(CObjectHandle) == 0x10);

class CObjectList
{
public:
	CObjectHandle m_array[2300]; //0x0000
	uintptr_t addr(size_t index)
	{
		uintptr_t addr = reinterpret_cast<uintptr_t>(static_cast<void*>(this)) + index * 0x10;
		return *reinterpret_cast<uintptr_t*>(addr);
	}
}; //Size: 0x8FC0
class CObjectInterface
{
public:
	char pad_0000[344]; //0x0000
	CObjectList* m_list; //0x0158
	int32_t m_size; //0x0160
	char pad_0164[4]; //0x0164
	int32_t m_count; //0x0168
	CObject* get(const size_t& index)
	{
		if (index < m_size)
			return m_list->m_array[index].m_pointer;
		return nullptr;
	}
}; //Size: 0x016C
class CPedHandle
{
public:
	CPed* m_pointer; //0x0000
	int32_t m_handle; //0x0008
	char pad_000C[4]; //0x000C
}; //Size: 0x0010
static_assert(sizeof(CPedHandle) == 0x10);

class CPedList
{
public:
	CPedHandle m_array[256]; //0x0000
	uintptr_t addr(size_t index)
	{
		uintptr_t addr = reinterpret_cast<uintptr_t>(static_cast<void*>(this)) + index * 0x10;
		return *reinterpret_cast<uintptr_t*>(addr);
	}
}; //Size: 0x1000
class CPedInterface
{
public:
	char pad_0000[256]; //0x0000
	CPedList* m_list; //0x0100
	int32_t m_size; //0x0108
	char pad_010C[4]; //0x010C
	int32_t m_count; //0x0110
	CPed* get(const size_t& index)
	{
		if (index < m_size)
			return m_list->m_array[index].m_pointer;
		return nullptr;
	}
}; //Size: 0x0114
class CVehicleHandle
{
public:
	class CVehicle* m_pointer; //0x0000
	int32_t m_handle; //0x0008
	char pad_000C[4]; //0x000C
}; //Size: 0x0010
static_assert(sizeof(CVehicleHandle) == 0x10);

class CVehicleList
{
public:
	CVehicleHandle m_array[300]; //0x0000
	uintptr_t addr(size_t index)
	{
		uintptr_t addr = reinterpret_cast<uintptr_t>(static_cast<void*>(this)) + index * 0x10;
		return *reinterpret_cast<uintptr_t*>(addr);
	}
}; //Size: 0x12C0
class CVehicleInterface
{
public:
	char pad_0000[384]; //0x0000
	CVehicleList* m_list; //0x0180
	int32_t m_size; //0x0188
	char pad_018C[4]; //0x018C
	int32_t m_count; //0x0190
	CVehicle* get(const size_t& index)
	{
		if (index < m_size)
			return m_list->m_array[index].m_pointer;
		return nullptr;
	}
}; //Size: 0x0194
class CCameraInterface
{
public:
	char pad_00000[1032]; //0x0000
}; //Size: 0x0408
class CReplayInterface
{
public:
	char pad_0000[8]; //0x0000
	CCameraInterface* m_camera_interface; //0x0008
	CVehicleInterface* m_vehicle_interface; //0x0010
	CPedInterface* m_ped_interface; //0x0018
	CPickupInterface* m_pickup_interface; //0x0020 
	CObjectInterface* m_object_interface; //0x0028
	char pad_0030[16]; //0x0030
}; //Size: 0x0040
class CGameScriptHandlerMgr : public rage::scriptHandlerMgr
{
public:
};

class CScriptParticipant
{
public:
	char pad_0000[16]; //0x0000
	class CNetGamePlayer* m_net_game_player; //0x0010
	char pad_0018[2]; //0x0018
	int16_t m_participant_index; //0x001A
	char pad_001C[12]; //0x001C
}; //Size: 0x0028
static_assert(sizeof(CScriptParticipant) == 0x28);

class CGameScriptHandlerNetComponent : public rage::scriptHandlerNetComponent
{
public:
	char pad_0010[32]; //0x0010
	class CScriptParticipant* m_host; //0x0030
	int16_t m_local_participant_index; //0x0038
	char pad_003A[6]; //0x003A
	uint32_t m_participant_bitset; //0x0040
	char pad_0044[36]; //0x0044
	class CScriptParticipant* m_participants[32]; //0x0068
	char pad_0168[12]; //0x0168
	int16_t m_num_participants; //0x0174
	char pad_0176[28]; //0x0176
	uint8_t m_host_migration_flags; //0x0192
	char pad_0193[29]; //0x0193
	int get_participant_index(CNetGamePlayer* player);

	bool is_player_a_participant(CNetGamePlayer* player)
	{
		return m_participant_bitset & (1 << player->m_player_id);
	}

	bool is_local_player_host()
	{
		if (!m_host)
			return true;

		return m_host->m_participant_index == m_local_participant_index;
	}

	bool is_player_host(CNetGamePlayer* player)
	{
		return m_host->m_participant_index == get_participant_index(player);
	}

	CNetGamePlayer* get_host()
	{
		if (!m_host)
			return nullptr;

		return m_host->m_net_game_player;
	}

	void block_host_migration(bool toggle)
	{
		if (toggle)
			m_host_migration_flags |= (1 << 7);
		else
			m_host_migration_flags &= ~(1 << 7);
	}

	bool force_host();

	bool give_host(CNetGamePlayer* player)
	{
		if (!force_host())
			return false;
		send_host_migration_event(player);
		do_host_migration(player, NULL, true);
		return is_player_host(player);
	}
}; //Size: 0x01B0
static_assert(sizeof(CGameScriptHandlerNetComponent) == 0x1B0);

class CScriptedGameEvent : public rage::netGameEvent
{
public:
	char pad_00030[64]; //0x0030
	int64_t m_args[54]; //0x0070
	uint32_t m_bitset; //0x0220
	uint32_t m_args_size; //0x0224
}; //Size: 0x0228
class CNetworkIncrementStatEvent : public rage::netGameEvent
{
public:
	uint32_t m_stat; //0x0030
	uint32_t m_amount; //0x0034
}; //Size: 0x0038
#pragma pack(push, 1)
class ScInfo
{
public:
	char m_ticket[208]; //0x0000
	char pad_00D0[304]; //0x00D0
	char m_session_ticket[88]; //0x0200
	char pad_0258[40]; //0x0258
	uint32_t m_nonce; //0x0280
	char pad_0284[4]; //0x0284
	uint32_t m_account_id; //0x0288
	char pad_028C[16]; //0x028C
	char m_profile_pic[128]; //0x029C
	char pad_031C[32]; //0x031C
	char m_country_code[4]; //0x033C
	char pad_0340[31]; //0x0340
	char m_email_address[96]; //0x035F
	char pad_03BF[6]; //0x03BF
	char m_language_subtag[8]; //0x03C5
	char pad_03CD[2]; //0x03CD
	char m_sc_name[20]; //0x03CF
	char pad_03E3[533]; //0x03E3
	char m_session_key[16]; //0x05F8
	char pad_0608[2296]; //0x0608
}; //Size: 0x0F00
static_assert(sizeof(ScInfo) == 0xF00);
#pragma pack(pop)
class ScGame
{
public:
	char pad_0000[64]; //0x0000
	char* m_string; //0x0040
	char pad_0048[256]; //0x0048
};

static_assert(sizeof(ScGame) == 0x148);

class ScGameInfo
{
public:
	virtual ~ScGameInfo() = default;
	virtual ScGame* GetGame() = 0;
	virtual void Unk0010() = 0;
	virtual void Unk0018() = 0;
	virtual int GetStringIndex(const char* StringId, u64 Unk, u32 GameId) = 0;
	char pad_0008[12]; //0x0008
	uint32_t m_id; //0x0014
	char pad_0018[8]; //0x0018
	ScGame m_games[10]; //0x0020
	u64 GetGamesAddress()
	{
		return (u64)this + offsetof(ScGameInfo, m_games);
	}
}; //Size: 0x0028
static_assert(sizeof(ScGameInfo) == 0xCF0);
#pragma pack(push, 1)
class MetricSessionMigrated : public rage::rlMetric
{
public:
	char pad_0008[828]; //0x0008
	uint32_t m_num_players; //0x0344
}; //Size: 0x0348
static_assert(sizeof(MetricSessionMigrated) == 0x348);

class NetworkGameConfig
{
public:
	char pad_0000[48]; //0x0000
	uint32_t m_public_slots; //0x0030
	uint32_t m_private_slots; //0x0034
	char pad_0038[272]; //0x0038
}; //Size: 0x0148
static_assert(sizeof(NetworkGameConfig) == 0x148);

class NetworkGameFilterMatchmakingComponent
{
public:
	uint32_t m_filter_type; //0x0000
	char m_filter_name[24]; //0x0004
	uint32_t m_num_parameters; //0x001C
	uint16_t m_game_mode; //0x0020
	uint16_t m_session_type; //0x0022
	uint32_t m_param_unk[8]; //0x0024
	char m_param_names[8][24]; //0x0044
	char pad_0104[4]; //0x0104
	uint32_t m_param_mappings[8]; //0x0108
	char pad_0128[352]; //0x0128
	uint32_t m_param_values[8]; //0x0288
	char pad_02A8[96]; //0x02A8
	uint32_t m_enabled_params_bitset; //0x0308
	char pad_030C[8]; //0x030C

	//Do not use for actual network filters, this will break things
	void SetParameter(const char* name, uint32_t index, uint32_t value)
	{
		strcpy_s(m_param_names[index], name);
		m_param_mappings[index] = index;
		m_param_values[index] = value;
		m_enabled_params_bitset |= (1 << index);
		if (m_num_parameters <= index)
			m_num_parameters++;
	}
}; //Size: 0x0314
static_assert(sizeof(NetworkGameFilterMatchmakingComponent) == 0x314);

class MatchmakingAttributes
{
public:
	uint32_t m_game_mode; //0x0000
	uint32_t m_num_params; //0x0004
	uint32_t m_param_unk[8]; //0x0008
	char m_param_names[8][24]; //0x0028
	uint32_t m_param_values[8]; //0x00E8
	uint32_t m_params_bitset; //0x0108
}; //Size: 0x010C
static_assert(sizeof(MatchmakingAttributes) == 0x10C);

class NetworkGameFilter
{
public:
	virtual ~NetworkGameFilter() = default;

	virtual void Reset()
	{
	};

public:
	uint32_t m_build_type; //0x0008
	uint32_t m_discriminator; //0x000C
	uint32_t m_discriminator_mapping; //0x0010
	uint32_t m_region_mapping; //0x0014
	uint32_t m_language_mapping; //0x0018
	uint32_t m_mm_group_1_mapping; //0x001C
	uint32_t m_mm_group_2_mapping; //0x0020
	uint32_t m_activity_type_mapping; //0x0024
	uint32_t m_activity_id_mapping; //0x0028
	uint32_t m_activity_players_mapping; //0x002C
	class NetworkGameFilterMatchmakingComponent m_matchmaking_component; //0x0030
}; //Size: 0x0344
static_assert(sizeof(NetworkGameFilter) == 0x344);

class SessionInfoBackup
{
public:
	class rage::rlSessionInfo m_session_info; //0x0000
	uint32_t m_unk; //0x00D0
	char pad_0074[4]; //0x00D4
	uint32_t m_flags; //0x00D8
}; //Size: 0x00DC
static_assert(sizeof(SessionInfoBackup) == 0xDC);

class MatchmakingSessionResult
{
public:
	class rage::rlSessionDetail m_detail; //0x0000
	char pad_0478[24]; //0x0478
}; //Size: 0x0490
static_assert(sizeof(MatchmakingSessionResult) == 0x490);

class CNetRemoteComplaint
{
public:
	uint64_t m_complainer_token; //0x0000
	uint64_t m_complainee_token; //0x0008
	uint32_t m_flags; //0x0010
	uint32_t m_time; //0x0014
}; //Size: 0x0018
static_assert(sizeof(CNetRemoteComplaint) == 0x18);

class CNetComplaintMgr
{
public:
	uint64_t m_peer_address; //0x0000
	uint32_t m_host_peer_id; //0x0008
	char pad_000C[4]; //0x000C
	class rage::netConnectionManager* m_net_connection_mgr; //0x0010
	char pad_0018[64]; //0x0018
	class rage::rlGamerHandle m_handles_in_scope[64]; //0x0058
	uint32_t m_num_handles_in_scope; //0x0458
	char pad_045C[4]; //0x045C
	class CNetRemoteComplaint m_remote_complaints[64]; //0x0460
	uint32_t m_num_remote_complaints; //0x0A60
	char pad_0A64[4]; //0x0A64
	uint64_t m_peer_addresses_complained[64]; //0x0A68
	uint32_t m_num_addresses_complained; //0x0C68
	char pad_0C6C[520]; //0x0C6C
	uint32_t m_connection_identifier; //0x0E74
	uint32_t m_last_resend_time; //0x0E78
	char pad_0E7C[4]; //0x0E7C
	uint32_t m_time_to_resend; //0x0E80
	uint32_t m_flags; //0x0E84
	char pad_0E88[16]; //0x0E88

	bool HasComplaint(uint64_t PeerAddress)
	{
		for (uint32_t i{}; i != m_num_addresses_complained; ++i)
			if (m_peer_addresses_complained[i] == PeerAddress)
				return true;
		return false;
	}

	void Add(uint64_t PeerAddress, bool UseAll, bool SpoofAddress);

	uint32_t Count()
	{
		return m_num_addresses_complained;
	}

	void Remove(uint64_t peer_address)
	{
		if (!HasComplaint(peer_address))
			return;
		for (uint32_t i{}; i != m_num_addresses_complained; ++i)
			if (m_peer_addresses_complained[i] == peer_address)
				m_peer_addresses_complained[i] = m_peer_addresses_complained[m_num_addresses_complained - 1];
		m_num_addresses_complained--;
	}

	void RemoveAll()
	{
		for (uint32_t i{}; i != m_num_addresses_complained; ++i)
			m_peer_addresses_complained[i] = m_peer_addresses_complained[m_num_addresses_complained - 1];
		m_num_addresses_complained = 0;
	}
}; //Size: 0x0C98
static_assert(sizeof(CNetComplaintMgr) == 0xE98);

class PlayerNameMapNode
{
public:
	char m_name[16]; //0x0000
	class rage::rlGamerHandle m_handle; //0x0010
	class PlayerNameMapNode* m_next; //0x0020
	class PlayerNameMapNode* m_prev; //0x0028
}; //Size: 0x0030
static_assert(sizeof(PlayerNameMapNode) == 0x30);

class JoiningPlayerNameMap
{
public:
	class PlayerNameMapNode m_names[100]; //0x0000
	char pad_12C0[40]; //0x12C0
	uint32_t m_num_name_nodes; //0x12E8
	char pad_12EC[796]; //0x12EC
}; //Size: 0x1608
static_assert(sizeof(JoiningPlayerNameMap) == 0x1608);

class CNetBlacklistNode : public rage::rlGamerHandle
{
public:
	bool m_block_rejoin; //0x0010
	char pad_0011[3]; //0x0011
	uint32_t m_added_time; //0x0014
	class CNetBlacklistNode* m_next; //0x0018
	class CNetBlacklistNode* m_prev; //0x0020
}; //Size: 0x0028
static_assert(sizeof(CNetBlacklistNode) == 0x28);

class CNetBlacklist
{
public:
	class CNetBlacklistNode m_nodes[16]; //0x0000
	class CNetBlacklistNode* m_head; //0x0280
	class CNetBlacklistNode* m_tail; //0x0288
	uint32_t m_free_nodes; //0x0290
	char pad_0294[4]; //0x0294
	class CNetBlacklistNode* m_start; //0x0298
	char pad_02A0[24]; //0x02A0
}; //Size: 0x02B8
static_assert(sizeof(CNetBlacklist) == 0x2B8);

class RemotePlayerData
{
public:
	class rage::netGamePlayerData m_data[32]; //0x0000
	uint32_t m_count; //0x0600
	char pad_0604[4]; //0x0604
}; //Size: 0x0608
static_assert(sizeof(RemotePlayerData) == 0x608);

class InvitedGamer
{
public:
	class rage::rlGamerHandle m_handle;
	char pad_0010[12]; //0x0010
	uint32_t m_flags; //0x001C
}; //Size: 0x0020
static_assert(sizeof(InvitedGamer) == 0x20);

class InvitedGamers
{
public:
	class InvitedGamer m_invited_gamers[100]; //0x0000
	uint32_t m_num_invited_gamers; //0x0C80
	char pad_0C84[4]; //0x0C84
}; //Size: 0x0C88
static_assert(sizeof(InvitedGamers) == 0xC88);

class CNetwork
{
public:
	rage::rlSessionInfo m_steam_unk_session; //0x0000
	rage::Obf32 m_num_dinput8_instances; //0x0070
	rage::Obf32 m_last_time_dinput8_checked; //0x0080
	class rage::snSession m_game_session; //0x00F0
	class rage::snSession m_transition_session; //0x5578
	char pad_AA00[16]; //0xAA00
	class rage::snSession* m_game_session_ptr_2; //0xAA10
	class rage::snSession* m_transition_session_ptr_2; //0xAA18
	char pad_AA20[16]; //0xAA20
	class rage::snSession* m_game_session_ptr; //0xAA30
	class rage::snSession* m_transition_session_ptr; //0xAA38
	char pad_AA40[24]; //0xAA40
	class NetworkGameConfig m_network_game_config; //0xAA58
	class NetworkGameConfig m_network_transition_config; //0xABA0
	bool m_session_attributes_dirty; //0xACE8
	char pad_ACE9[19]; //0xACE9
	uint32_t m_session_visibility_flags; //0xACFC
	uint32_t m_transition_visibility_flags; //0xAD00
	char pad_AD04[68]; //0xAD04
	class MetricSessionMigrated m_metric_session_migrated; //0xAD48
	bool m_migrated_metric_enabled; //0xB090
	char pad_B091[3]; //0xB091
	uint32_t m_game_session_state; //0xB094
	class NetworkGameFilter m_network_game_filter; //0xB098
	char pad_B3DC[33]; //0xB3DC
	bool m_was_invited; //0xB3FD
	char pad_B3FE[10]; //0xB3FE
	class rage::rlSessionInfo m_unk_session_info; //0xB408
	char pad_B4D8[635]; //0xB4D8
	bool m_need_host_change; //0xB753
	char pad_B754[74316]; //0xB754
	class rage::rlSessionDetail m_joining_session_detail; //0x1D9A0
	class SessionInfoBackup m_last_joined_session; //0x1DE18
	char pad_1DEF4[40]; //0x1DEF4
	uint32_t m_current_matchmaking_group; //0x1DF1C
	uint32_t m_matchmaking_group_max_players[5]; //0x1DF20
	uint32_t m_num_active_matchmaking_groups; //0x1DF34
	char pad_1DF38[8]; //0x1DF38
	uint8_t m_matchmaking_property_id; //0x1DF40
	uint8_t m_matchmaking_mental_state; //0x1DF41
	char pad_1DF42[366]; //0x1DF42
	class rage::rlMatchmakingFindResult m_game_session_matchmaking[3]; //0x1E0B0
	char pad_2ABC0[40]; //0x2ABC0
	class MatchmakingSessionResult m_game_matchmaking_session_results[10]; //0x2ABE8
	char pad_2D988[308]; //0x2D988
	uint32_t m_num_bosses; //0x2DABC
	bool m_num_bosses_set; //0x2DAC0
	char pad_2DAC1[7]; //0x2DAC1
	class rage::rlGamerHandle m_transition_creator_handle; //0x2DAC8
	char pad_2DAD8[12]; //0x2DAD8
	bool m_is_waiting_async; //0x2DAE4
	bool m_is_preferred_activity; //0x2DAE5
	char pad_2DAE6[2]; //0x2DAE6
	uint32_t m_in_progress_finish_time; //0x2DAE8
	char pad_2DAEC[4]; //0x2DAEC
	bool m_local_player_info_dirty; //0x2DAF0
	char pad_2DAF1[495]; //0x2DAF1
	class rage::rlGamerHandle m_inviter_handle; //0x2DCE0
	class CNetComplaintMgr m_game_complaint_mgr; //0x2DCF0
	class CNetComplaintMgr m_transition_complaint_mgr; //0x2EB88
	char pad_2FA20[32]; //0x2FA20
	class JoiningPlayerNameMap m_unused_joining_player_name_map; //0x2FA40
	char pad_31048[8]; //0x31048
	class CNetBlacklist m_blacklist; //0x31050
	char pad_31308[8]; //0x31308
	class InvitedGamers m_game_invited_gamers; //0x31310
	char pad_31F98[5888]; //0x31F98
	class SessionInfoBackup m_last_joined_transition; //0x33698
	uint32_t m_activity_max_players; //0x33774
	uint32_t m_activity_max_spectators; //0x33778
	char pad_3377C[48]; //0x3377C
	bool m_do_not_launch_from_join_as_migrated_host; //0x337AC
	char pad_337AD[7]; //0x337AD
	bool m_is_activity_session; //0x337B4
	char pad_337B5[35]; //0x337B5
	class RemotePlayerData m_remote_player_data; //0x337D8
	char pad_33DE0[8]; //0x33DE0
	class rage::netGamePlayerData m_local_net_game_player_data; //0x33DE8
	char pad_33E18[608]; //0x33E18
	class rage::rlMatchmakingFindResult m_transition_matchmaking[4]; //0x34078
	class NetworkGameFilter m_transition_filters[4]; //0x44F38
	char pad_45C48[20]; //0x45C48
	uint32_t m_transition_quickmatch_group_handle_count; //0x45C5C
	class rage::rlGamerHandle m_transition_quickmatch_group_handles[32]; //0x45C60
	bool m_retain_activity_group; //0x45E60
	char pad_45E61[7]; //0x45E61
	class rage::rlSessionInfo m_transition_to_activity_session_info; //0x45E68
	char pad_45F38[48]; //0x45F38
	class MatchmakingSessionResult m_transition_matchmaking_session_results[10]; //0x45F68
	char pad_48D08[8]; //0x48D08
	class InvitedGamers m_transition_invited_gamers; //0x48D10
	char pad_49998[16]; //0x49998
	class rage::rlGamerHandle m_transition_to_game_handle; //0x499A8
	class rage::rlSessionInfo m_transition_to_game_session_info; //0x499B8
	char pad_49A88[4]; //0x49A88
	uint32_t m_transition_to_game_session_participant_count; //0x49A8C
	class rage::rlGamerHandle m_transition_to_game_session_participants[32]; //0x49A90
	char pad_49C90[80]; //0x49C90
	class rage::rlGamerHandle m_follower_handles[32]; //0x49CE0
	uint32_t m_follower_count; //0x49EE0
	char pad_49EE4[628]; //0x49EE4
}; //Size: 0x38650
static_assert(sizeof(CNetwork) == 0x4A158);
#pragma pack(pop)
#pragma pack(push, 1)
class CMsgTextMessage
{
public:
	char m_message[256]; //0x0000
	uint64_t m_peer_id; //0x0100
	bool m_is_team; //0x0108
}; //Size: 0x0109
static_assert(sizeof(CMsgTextMessage) == 0x109);
#pragma pack(pop)
#pragma pack(push, 1)
class CMsgTextMessage2
{
public:
	char m_message[256]; //0x0000
	uint64_t m_peer_id; //0x0100
}; //Size: 0x0108
static_assert(sizeof(CMsgTextMessage2) == 0x108);
#pragma pack(pop)
class PresenceData
{
public:
	virtual ~PresenceData() = default;
	virtual bool UpdateAttributeInt(int ProfileIndex, char* Attribute, u64 Value) = 0;
	virtual bool UpdateAttributeUnk(int ProfileIndex, char* Attribute, u64 Value) = 0;
	virtual bool UpdateAttributeString(int ProfileIndex, char* Attribute, char* Value) = 0;
};

class CPlayerListMenu
{
public:
	virtual ~CPlayerListMenu()
	{
	}

	virtual void unk_0008()
	{
	}

	char pad_0008[112]; //0x0008
	bool m_failed; //0x0076
	char pad_0079[21]; //0x0077
	uint32_t m_selected_friend; //0x008C
	char pad_0090[40]; //0x0090
	uint32_t m_online_bitset; //0x00B8
	char pad_00BC[28]; //0x00BC
	uint8_t m_is_joinable; //0x00D8
	char pad_00D9[3]; //0x00D9
	uint32_t unk_00DC; //0x00DC
	class unk* unk_00E0; //0x00E0
	char pad_00E8[80]; //0x00E8
	uint32_t unk_0138; //0x0138
};

class CFriend
{
public:
	uint64_t m_rockstar_id; //0x0000
};
#pragma pack(push, 16)
class CFriendMenu
{
public:
	virtual ~CFriendMenu() = default;

	CFriend get_friend(u8 index)
	{
		return *reinterpret_cast<CFriend*>(reinterpret_cast<u64>(this) + 16ui64 * index);
	}

	u8 get_online_friend()
	{
		auto data{reinterpret_cast<u8*>(reinterpret_cast<u64>(this) + 0x8)};
		for (u8 i{}; i != 20; ++i, data += 16)
		{
			if (data[i] == 3)
			{
				return i;
			}
		}
		return NULL;
	}
};
#pragma pack(pop)
#pragma pack(push, 1)
class CPackedMessageData
{
public:
	CPackedMessageData(void* buffer, bool nullifyData = true)
	{
		if (nullifyData)
		{
			m_count = NULL;
			m_type = NULL;
			memset(m_data_buffer, NULL, sizeof(m_data_buffer));
		}
		if (m_data_buffer != buffer)
			memcpy(m_data_buffer, buffer, sizeof(m_data_buffer));
		m_buffer = rage::datBitBuffer(m_data_buffer, sizeof(m_data_buffer));
		m_buffer.Seek(0);
	}

	CPackedMessageData(bool nullifyData = true) : CPackedMessageData(m_data_buffer, nullifyData)
	{
	}

	uint32_t m_count; //0x0000
	uint32_t m_type; //0x0004
	rage::datBitBuffer m_buffer; //0x0008
	uint8_t m_data_buffer[912]; //0x0028

	bool Deserialise(rage::datBitBuffer* buffer)
	{
		m_count = buffer->Read<u32>(5);
		if (m_count > 0)
		{
			auto bufferSize{buffer->Read<u32>(13)};
			char data[4096]{};
			if (!buffer->ReadArray(data, bufferSize))
			{
				return false;
			}
			m_buffer = rage::datBitBuffer(data, bufferSize);
			m_buffer.m_maxBit = bufferSize;
		}
		return true;
	}

	bool Serialise(rage::datBitBuffer* buffer, rage::datBitBuffer& eventBuffer)
	{
		buffer->Write<u32>(5, m_count);
		if (m_count > 0)
		{
			u32 bufferSize{eventBuffer.m_maxBit / 8};
			buffer->Write<u32>(13, bufferSize);
			if (!buffer->WriteArray(eventBuffer.m_data, bufferSize))
			{
				return false;
			}
			m_buffer = eventBuffer;
			m_buffer.m_maxBit = bufferSize;
		}
		return true;
	}
}; //Size: 0x03B8
static_assert(sizeof(CPackedMessageData) == 0x3B8);
#pragma pack(pop)
#pragma pack(push, 1)
class CMsgPackedEvents
{
public:
	CMsgPackedEvents(bool nullifyBuffer = true) : m_data(CPackedMessageData())
	{
	}

	void* m_unk; //0x0000
	CPackedMessageData m_data;

	bool Deserialise(rage::datBitBuffer* buffer)
	{
		return m_data.Deserialise(buffer);
	}

	bool Serialise(rage::datBitBuffer* buffer, rage::datBitBuffer& eventBuffer)
	{
		return m_data.Serialise(buffer, eventBuffer);
	}
}; //Size: 0x03C0
static_assert(sizeof(CMsgPackedEvents) == 0x3C0);
#pragma pack(pop)
class netInventoryBaseItem
{
public:
	virtual ~netInventoryBaseItem() = default;
	uint32_t m_hash; //0x0008
	uint32_t m_category_hash; //0x000C
	uint32_t m_price; //0x0010
};

class netCatalog
{
public:
	HashTable<netInventoryBaseItem*> m_items; //0x0000
	char pad_006A[18]; //0x006A
	uint32_t m_version; //0x007C

	netInventoryBaseItem* getCatalogEntry(u32 hash)
	{
		for (HashNode* node{m_items.m_lookup_table[hash % m_items.m_lookup_key]}; node; node = node->m_next)
		{
			if (node->m_hash == hash)
			{
				if (netInventoryBaseItem* transaction{m_items.m_data[node->m_idx]})
				{
					return transaction;
				}
			}
		}
		return nullptr;
	}

	int getServiceThreshold(const DWORD service)
	{
		const netInventoryBaseItem* item{getCatalogEntry(service)};
		return item ? item->m_price : -1;
	}
}; //Size: 0x0080
