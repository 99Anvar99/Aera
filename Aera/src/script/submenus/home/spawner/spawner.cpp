#include "spawner.h"

#include "util/helpers/vehicle_spawner.h"

namespace ui::submenus
{
	void spawnerSubmenu::init(submenu& submenu)
	{
		submenu.add(submenuOption(homeSpawnerVehicle::get()));
	}

	void homeSpawnerVehicle::init(submenu& submenu)
	{
		for (std::uint8_t i = 0; i != static_cast<std::uint8_t>(23); ++i)
		{
			submenu.add(option(util::vehicle::g_model_classes[i], [i]
			{
				util::vehicle::g_selected_vehicle_class = i;
				const auto class_sub = selectedVehicleSpawnerSubmenmu::get();
				class_sub->m_name = util::vehicle::g_model_classes[i];
				class_sub->m_options.clear();
				class_sub->once();

				menu::push(class_sub);
			}));
		}
	}

	void selectedVehicleSpawnerSubmenmu::init(submenu& submenu)
	{
		auto vehicles{util::vehicle::g_models.get(util::vehicle::g_selected_vehicle_class)};
		for (std::size_t i = 0; i != vehicles.size(); ++i)
		{
			auto& veh{vehicles[i]};
			submenu.add(option(veh.m_name_label,
			                   veh.m_manufacturer_label.empty() ? "" : "Manufacturer: " + veh.m_manufacturer_label,
			                   [i, veh]
			                   {
				                   g_pool.add([i, veh]
				                   {
					                   util::vehicle::g_selected_vehicle_from_class = i;
					                   const auto veh_sub = finalselectedVehicleSpawnerSubmenmu::get();
					                   veh_sub->m_name = veh.m_name_label;
					                   veh_sub->m_options.clear();
					                   veh_sub->once();

					                   menu::push(veh_sub);
				                   });
			                   }));
		}
	}

	void finalselectedVehicleSpawnerSubmenmu::init(submenu& submenu)
	{
		const auto vehicles{util::vehicle::g_models.get(util::vehicle::g_selected_vehicle_class)};
		const auto& veh{vehicles[util::vehicle::g_selected_vehicle_from_class]};
		submenu.add(toggleOption("Spawn In GodMode", "Makes your vehicle indestructible",
		                         util::vehicle::set_vehicle_godmode_when_spawned));
		submenu.add(toggleOption("Sit In Vehicle", "Makes your player sit in the vehicle when spawned",
		                         util::vehicle::set_ped_in_vehicle_when_spawned));
		submenu.add(toggleOption("Max Vehicle", "Maxes out all modifications on your newly spawned vehicle",
		                         util::vehicle::set_vehicle_maxed_when_spawned));
		submenu.add(toggleOption(
				"Seamless Spawning",
				"This will make your player 'seamlessly' change vehicles without kicking you out of the previous vehicle, if you were in a vehicle. The newly spawned vehicle will remain at the same speed as the previous vehicle",
				util::vehicle::seamless_re_enter_vehicle)
		);
		submenu.add(option("Spawn", [veh]
		{
			g_pool.add([veh]
			{
				util::vehicle::spawn_vehicle(veh.m_model_info->m_hash);
			});
		}));
	}
}
