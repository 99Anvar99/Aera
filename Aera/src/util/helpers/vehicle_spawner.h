#pragma once
#include <unordered_set>
#include "gxt_helper.h"
#include "memory/pointers.h"
#include "rage/classes.h"
#include "util/player_mgr.h"
#include "util/util.h"

namespace util::vehicle
{
	class vehicle_data
	{
	public:
		vehicle_data(CVehicleModelInfo* model_info, std::string manufacturer_label, std::string name_label) :
			m_model_info(model_info),
			m_manufacturer_label(std::move(manufacturer_label)),
			m_name_label(std::move(name_label))
		{
		}

		CVehicleModelInfo* m_model_info{};
		std::string m_manufacturer_label{};
		std::string m_name_label{};
	};

	class models
	{
	public:
		std::vector<vehicle_data>& get(uint8_t idx)
		{
			if (idx < m_vehicle_categories.size())
			{
				return m_vehicle_categories[idx];
			}
			return m_dummy;
		}

	private:
		std::array<std::vector<vehicle_data>, 23> m_vehicle_categories{};
		std::vector<vehicle_data> m_dummy{};
	};

	inline std::size_t g_selected_vehicle_from_class{};
	inline std::uint8_t g_selected_vehicle_class{};
	inline models g_models{};
	inline std::vector<std::string> g_model_classes{};

	inline HashTable<CVehicleModelInfo*>& get_hash_table()
	{
		return *reinterpret_cast<HashTable<CVehicleModelInfo*>*>(pointers::g_hashTable);
	}

	inline std::string get_vehicle_class_name(const std::uint8_t veh_class)
	{
		const std::string class_id = std::to_string(veh_class);
		const std::string label = "VEH_CLASS_" + class_id;
		auto value = gxt::get_gxt_label(label);

		if (value.value_or("NULL") == "NULL" || value.value().empty())
			return "Unknown Class (" + class_id + ")";

		return value.value();
	}

	constexpr uint32_t invalid_vehicles[] = {
		"arbitergt"_joaat, "astron2"_joaat, "cyclone2"_joaat, "ignus2"_joaat, "s95"_joaat
	};

	inline void cache_model_table()
	{
		for (std::uint8_t i{}; i != static_cast<std::uint8_t>(23); ++i)
		{
			g_model_classes.push_back(get_vehicle_class_name(i));
		}

		const auto& [m_data, m_size, pad_000A, m_item_size, pad_0020, m_lookup_table, m_lookup_key] = get_hash_table();
		std::unordered_set invalid_vehicles_set(std::begin(invalid_vehicles), std::end(invalid_vehicles));
		for (int i{}; i != m_size; ++i)
		{
			for (const HashNode* node{m_lookup_table[i]}; node; node = node->m_next)
			{
				if (const uint16_t table_idx{node->m_idx}; table_idx != m_size)
				{
					if (CVehicleModelInfo* model{m_data[table_idx]}; model && model->is_type(eModelType::Vehicle))
					{
						if (invalid_vehicles_set.contains(model->m_hash))
						{
							continue; // skip unwanted / unavailable vehicles
						}

						auto manufacturer_label = gxt::get_gxt_label(model->m_manufacturer);
						auto name_label = gxt::get_gxt_label(model->m_name, true);
						g_models.get(static_cast<uint8_t>(model->get_class())).emplace_back(
							model, manufacturer_label.value_or(""), name_label.value());
					}
				}
			}
		}
	}

	inline bool seamless_re_enter_vehicle{};
	inline bool set_vehicle_godmode_when_spawned{};
	inline bool set_ped_in_vehicle_when_spawned{};
	inline bool set_vehicle_maxed_when_spawned{};
	constexpr int MOD_KIT_INDEX = 0x0;
	constexpr int MIN_MOD_INDEX = 0x11;
	constexpr int MAX_MOD_INDEX = 0x14;
	constexpr int SKIP_MOD_INDEX = 0x13;

	inline void spawn_vehicle(const Hash hash)
	{
		if (!natives::request_model(hash) || !STREAMING::IS_MODEL_A_VEHICLE(hash))
		{
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
			return;
		}

		float previous_vehicle_speed{};
		if (Vehicle previous_vehicle; seamless_re_enter_vehicle && (previous_vehicle = PED::GET_VEHICLE_PED_IS_IN(
			PLAYER::PLAYER_PED_ID(), TRUE)) != NULL)
		{
			previous_vehicle_speed = ENTITY::GET_ENTITY_SPEED(previous_vehicle);
			TASK::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::PLAYER_PED_ID());
			natives::delete_selected_entity(previous_vehicle, true);
		}

		Vehicle new_spawned_vehicle{};
		auto ped_direction = ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID());
		auto location = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.f, 5.f, 0.f);
		classes::runUnderThread(
			classes::getGtaThread(*pointers::g_isSessionActive ? "freemode"_joaat : "main_persistent"_joaat), [&]
			{
				new_spawned_vehicle = VEHICLE::CREATE_VEHICLE(hash, location.x, location.y, location.z, ped_direction,
				                                              *pointers::g_isSessionActive, FALSE, FALSE);
			});
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);

		if (new_spawned_vehicle == 0)
			return;

		if (*pointers::g_isSessionActive)
		{
			DECORATOR::DECOR_SET_INT(new_spawned_vehicle, "MPBitset", 0);
			if (NETWORK::NETWORK_GET_ENTITY_IS_NETWORKED(new_spawned_vehicle))
				NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(NETWORK::VEH_TO_NET(new_spawned_vehicle), TRUE);
			VEHICLE::SET_VEHICLE_IS_STOLEN(new_spawned_vehicle, FALSE);
			ENTITY::SET_ENTITY_SHOULD_FREEZE_WAITING_ON_COLLISION(new_spawned_vehicle, TRUE);
		}

		VEHICLE::SET_VEHICLE_ENGINE_ON(new_spawned_vehicle, TRUE, TRUE, FALSE);
		ENTITY::SET_ENTITY_HEADING(new_spawned_vehicle, ped_direction);

		if (ENTITY::GET_ENTITY_HEIGHT_ABOVE_GROUND(PLAYER::PLAYER_PED_ID()) <= 1.5f)
			VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(new_spawned_vehicle, 5.0f);

		if (set_vehicle_godmode_when_spawned)
			ENTITY::SET_ENTITY_INVINCIBLE(new_spawned_vehicle, TRUE);

		if (set_ped_in_vehicle_when_spawned)
			PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), new_spawned_vehicle, -1);

		if (seamless_re_enter_vehicle)
			VEHICLE::SET_VEHICLE_FORWARD_SPEED(new_spawned_vehicle, previous_vehicle_speed);

		if (VEHICLE::IS_THIS_MODEL_A_HELI(hash) || VEHICLE::IS_THIS_MODEL_A_PLANE(hash))
			VEHICLE::SET_HELI_BLADES_FULL_SPEED(new_spawned_vehicle);

		if (VEHICLE::IS_THIS_MODEL_A_PLANE(hash))
			VEHICLE::SET_VEHICLE_KEEP_ENGINE_ON_WHEN_ABANDONED(new_spawned_vehicle, TRUE);

		if (set_vehicle_maxed_when_spawned)
		{
			VEHICLE::SET_VEHICLE_MOD_KIT(new_spawned_vehicle, MOD_KIT_INDEX);
			for (int i = MIN_MOD_INDEX; i <= MAX_MOD_INDEX; ++i)
			{
				if (i != SKIP_MOD_INDEX)
					VEHICLE::TOGGLE_VEHICLE_MOD(new_spawned_vehicle, i, TRUE);
			}
			VEHICLE::SET_VEHICLE_WINDOW_TINT(new_spawned_vehicle, 1);
			VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(new_spawned_vehicle, FALSE);
			VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(new_spawned_vehicle, 0, 0, 0);
			for (int j = 0; j < 50; ++j)
			{
				VEHICLE::SET_VEHICLE_MOD(new_spawned_vehicle, j,
				                         VEHICLE::GET_NUM_VEHICLE_MODS(new_spawned_vehicle, j) - 1, TRUE);
			}
			VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(new_spawned_vehicle, 1);
			VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(new_spawned_vehicle, BRAND);
			VEHICLE::SET_VEHICLE_DIRT_LEVEL(new_spawned_vehicle, 0.0f);
		}

		ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&new_spawned_vehicle);
	}
}
