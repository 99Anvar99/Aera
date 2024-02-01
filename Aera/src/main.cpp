#include "pch/pch.h"
#include "memory/pointers.h"
#include "renderer/renderer.h"
#include "hooking/hooking.h"
#include "rage/commands/invoker/invoker.h"
#include "fiber/manager.h"
#include "fiber/pool.h"
#include "script/script.h"
#include "commands/commands.h"
#include "exceptions/handler.h"
#include "core/logger.h"
#include "fiber/dxfiber.h"
#include "gui/gui.h"
#include "thread pool/threadpool.h"
#include "util/imageLoaderHelpers.h"

std::unique_ptr<hooking> hooking_instance{};
std::unique_ptr<renderer> renderer_instance{};
std::unique_ptr<thread_pool> thread_pool_instance;

void generate_offsets()
{
	std::cout << R"(//Offset from WORLD)" << std::endl;
	std::cout << R"(#define OFFSET_PLAYER 0x)" << std::hex << std::uppercase << offsetof(CPedFactory, m_local_ped) <<
		" //CPedFactory, m_local_ped" << std::endl;
	std::cout << std::endl;
	std::cout << R"(//Offset from ENTITY)" << std::endl;
	std::cout << R"(#define OFFSET_ENTITY_POSBASE 0x)" << std::hex << std::uppercase <<
		offsetof(rage::CEntity, m_navigation) << " //rage::fwEntity, m_navigation" << std::endl;
	std::cout << R"(#define OFFSET_ENTITY_POSBASE_POS_X 0x)" << std::hex << std::uppercase <<
		offsetof(CNavigation, m_transformation_matrix) + (offsetof(rage::matrix44, rows) * 3) + (4 * 0) <<
		" //CNavigation, m_transformation_matrix.rows[3].x" << std::endl;
	std::cout << R"(#define OFFSET_ENTITY_POSBASE_POS_Y 0x)" << std::hex << std::uppercase <<
		offsetof(CNavigation, m_transformation_matrix) + (offsetof(rage::matrix44, rows) * 3) + (4 * 1) <<
		" //CNavigation, m_transformation_matrix.rows[3].y" << std::endl;
	std::cout << R"(#define OFFSET_ENTITY_POSBASE_POS_Z 0x)" << std::hex << std::uppercase <<
		offsetof(CNavigation, m_transformation_matrix) + (offsetof(rage::matrix44, rows) * 3) + (4 * 2) <<
		" //CNavigation, m_transformation_matrix.rows[3].z" << std::endl;
	std::cout << R"(#define OFFSET_ENTITY_POS 0x)" << std::hex << std::uppercase <<
		offsetof(rage::fwEntity, m_transformation_matrix) + (offsetof(rage::matrix44, rows) * 3) <<
		" //rage::fwEntity, m_transformation_matrix.rows[3]" << std::endl;
	std::cout << R"(#define OFFSET_ENTITY_HEALTH 0x)" << std::hex << std::uppercase <<
		offsetof(rage::CPhysical, m_health) << " //rage::CPhysical, m_health" << std::endl;
	std::cout << R"(#define OFFSET_ENTITY_HEALTH_MAX 0x)" << std::hex << std::uppercase <<
		offsetof(rage::CPhysical, m_maxhealth) << " //rage::CPhysical, m_maxhealth" << std::endl;
	std::cout << R"(#define OFFSET_ENTITY_ATTACKER 0x2A8 //CPed, m_attackers)" << std::endl;
	std::cout << R"(#define OFFSET_ENTITY_GOD 0x)" << std::hex << std::uppercase <<
		offsetof(rage::CPhysical, m_damage_bits) << " //rage::CPhysical, m_damage_bits + 1" << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << R"(#define OFFSET_RID 0x)" << std::hex << std::uppercase <<
		offsetof(rage::rlGamerInfo, m_unk_gamer_handle) + offsetof(rage::rlGamerHandle, m_rockstar_id) <<
		" //rage::rlGamerInfo, m_unk_gamer_handle.m_rockstar_id" << std::endl;
	std::cout << std::endl;
	std::cout << R"(//Offset from PED)" << std::endl;
	std::cout << R"(#define OFFSET_PLAYER_MONEY 0x)" << std::hex << std::uppercase << offsetof(CPed, m_cash) <<
		" //CPed, m_cash" << std::endl;
	std::cout << R"(#define OFFSET_PLAYER_ARMOR 0x)" << std::hex << std::uppercase << offsetof(CPed, m_armour) <<
		" //CPed, m_armour" << std::endl;
	std::cout << R"(#define OFFSET_PLAYER_INFO 0x)" << std::hex << std::uppercase << offsetof(CPed, m_player_info) <<
		" //CPed, m_player_info" << std::endl;
	std::cout << R"(#define OFFSET_PLAYER_INFO_WANTED_CAN_CHANGE 0x)" << std::hex << std::uppercase <<
		offsetof(CPlayerInfo, m_wanted_can_change) << " //CPlayerInfo, m_wanted_can_change" << std::endl;
	std::cout << R"(#define OFFSET_PLAYER_INFO_WANTED 0x)" << std::hex << std::uppercase <<
		offsetof(CPlayerInfo, m_wanted_level) << " //CPlayerInfo, m_wanted_level" << std::endl;
	std::cout << R"(#define OFFSET_PLAYER_WANTED_DISPLAY 0x)" << std::hex << std::uppercase <<
		offsetof(CPlayerInfo, m_wanted_level_display) << " //CPlayerInfo, m_wanted_level_display" << std::endl;
	std::cout << R"(#define OFFSET_PLAYER_INFO_RUN_SPD 0x)" << std::hex << std::uppercase <<
		offsetof(CPlayerInfo, m_run_speed) << " //CPlayerInfo, m_run_speed" << std::endl;
	std::cout << R"(#define OFFSET_PLAYER_INFO_WALK_SPD 0x)" << std::hex << std::uppercase <<
		offsetof(CPlayerInfo, m_walk_speed) << " //CPlayerInfo, m_walk_speed" << std::endl;
	std::cout << R"(#define OFFSET_PLAYER_INFO_SWIM_SPD 0x)" << std::hex << std::uppercase <<
		offsetof(CPlayerInfo, m_swim_speed) << " //CPlayerInfo, m_swim_speed" << std::endl;
	std::cout << R"(#define OFFSET_PLAYER_INFO_FRAMEFLAGS 0x)" << std::hex << std::uppercase <<
		offsetof(CPlayerInfo, m_frame_flags) << " //CPlayerInfo, m_frame_flags" << std::endl;
	std::cout << R"(#define OFFSET_PLAYER_INFO_STAMINA 0x)" << std::hex << std::uppercase <<
		offsetof(CPlayerInfo, m_stamina) << " //CPlayerInfo, m_stamina" << std::endl;
	std::cout << R"(#define OFFSET_PLAYER_INFO_STAMINA_REGEN 0x)" << std::hex << std::uppercase <<
		offsetof(CPlayerInfo, m_stamina_regen) << " //CPlayerInfo, m_stamina_regen" << std::endl;
	std::cout << R"(#define OFFSET_PLAYER_NAME 0x)" << std::hex << std::uppercase << offsetof(CPlayerInfo, m_gamer_info)
		+ offsetof(rage::rlGamerInfo, m_name) << " //CPlayerInfo, m_gamer_info.m_name" << std::endl;
	std::cout << R"(#define OFFSET_PLAYER_RAGDOLL 0x)" << std::hex << std::uppercase << offsetof(CPed, m_ped_type) <<
		" //CPed, m_ped_type" << std::endl;
	std::cout << R"(#define OFFSET_PLAYER_SEATBELT 0x)" << std::hex << std::uppercase << offsetof(CPed, m_seatbelt) <<
		" //CPed, m_ped_type" << std::endl;
	std::cout << R"(#define OFFSET_PLAYER_INVEHICLE 0x)" << std::hex << std::uppercase <<
		offsetof(CPed, m_ped_task_flag) << " //CPed, m_ped_task_flag" << std::endl;
	std::cout << R"(#define OFFSET_PLAYER_SUPERJUMP OFFSET_PLAYER_INFO_FRAMEFLAGS //CPlayerInfo, m_frame_flags)" <<
		std::endl;
	std::cout << R"(#define OFFSET_PLAYER_INVEHICLE 0x)" << std::hex << std::uppercase <<
		offsetof(CPed, m_ped_task_flag) << " //CPed, m_ped_task_flag" << std::endl;
	std::cout << R"(#define OFFSET_PLAYER_VEHICLE_DAMAGE_MP 0x)" << std::hex << std::uppercase <<
		offsetof(CPlayerInfo, m_melee_damage_mult) << " //CPlayerInfo, m_melee_damage_mult" << std::endl;
	std::cout <<
		R"(#define OFFSET_PLAYER_SUPER_PUNCH OFFSET_PLAYER_VEHICLE_DAMAGE_MP //CPlayerInfo, m_melee_damage_mult)" <<
		std::endl;
	std::cout << R"(#define OFFSET_PLAYER_WATER_PROOF 0x)" << std::hex << std::uppercase <<
		offsetof(rage::CPhysical, m_damage_bits) << " //rage::CPhysical, m_damage_bits" << std::endl;
	std::cout << R"(#define OFFSET_FRAME_FLAGS OFFSET_PLAYER_INFO_FRAMEFLAGS //CPlayerInfo, m_frame_flags)" <<
		std::endl;
	std::cout << std::endl;
	std::cout << R"(#define OFFSET_PED_INTELLIGENCE 0x)" << std::hex << std::uppercase <<
		offsetof(CPed, m_ped_intelligence) << " //CPed, m_ped_intelligence" << std::endl;
	std::cout << R"(#define OFFSET_PED_OXYGEN_TIME 0x)" << std::hex << std::uppercase <<
		offsetof(CPedIntelligence, m_oxygen_time) << " //CPedIntelligence, m_oxygen_time" << std::endl;
	std::cout << std::endl;
	std::cout << R"(//Offset from VEHICLE)" << std::endl;
	std::cout << R"(#define OFFSET_PLAYER_VEHICLE 0x)" << std::hex << std::uppercase << offsetof(CPed, m_vehicle) <<
		" //CPed, m_vehicle" << std::endl;
	std::cout << R"(#define OFFSET_VEHICLE_HEALTH 0x)" << std::hex << std::uppercase <<
		offsetof(CVehicle, m_body_health) << " //CVehicle, m_body_health" << std::endl;
	std::cout << R"(#define OFFSET_VEHICLE_GRAVITY 0x)" << std::hex << std::uppercase << offsetof(CVehicle, m_gravity)
		<< " //CVehicle, m_gravity" << std::endl;
	std::cout << R"(#define OFFSET_VEHICLE_BULLETPROOF_TIRES 0x)" << std::hex << std::uppercase <<
		offsetof(CVehicle, m_tyres_can_burst) << " //CVehicle, m_tyres_can_burst" << std::endl;
	std::cout << R"(#define OFFSET_VEHICLE_HANDLING 0x)" << std::hex << std::uppercase <<
		offsetof(CVehicle, m_handling_data) << " //CVehicle, m_handling_data" << std::endl;
	std::cout << R"(#define OFFSET_VEHICLE_HANDLING_ACCELERATION 0x)" << std::hex << std::uppercase <<
		offsetof(CHandlingData, m_acceleration) << " //CHandlingData, m_acceleration" << std::endl;
	std::cout << R"(#define OFFSET_VEHICLE_HANDLING_BRAKEFORCE 0x)" << std::hex << std::uppercase <<
		offsetof(CHandlingData, m_brake_force) << " //CHandlingData, m_brake_force" << std::endl;
	std::cout << R"(#define OFFSET_VEHICLE_HANDLING_TRACTION_CURVE_MIN 0x)" << std::hex << std::uppercase <<
		offsetof(CHandlingData, m_traction_curve_min) << " //CHandlingData, m_traction_curve_min" << std::endl;
	std::cout << R"(#define OFFSET_VEHICLE_HANDLING_TRACTION_CURVE_MAX 0x)" << std::hex << std::uppercase <<
		offsetof(CHandlingData, m_traction_curve_max) << " //CHandlingData, m_traction_curve_max" << std::endl;
	std::cout << R"(#define OFFSET_VEHICLE_HANDLING_DEFORM_MULTIPLIER 0x)" << std::hex << std::uppercase <<
		offsetof(CHandlingData, m_deformation_mult) << " //CHandlingData, m_deformation_mult" << std::endl;
	std::cout << R"(#define OFFSET_VEHICLE_HANDLING_COLISION_DAMAGE_MP 0x)" << std::hex << std::uppercase <<
		offsetof(CHandlingData, m_collision_damage_mult) << " //CHandlingData, m_collision_damage_mult" << std::endl;
	std::cout << R"(#define OFFSET_VEHICLE_HANDLING_UPSHIFT 0x)" << std::hex << std::uppercase <<
		offsetof(CHandlingData, m_upshift) << " //CHandlingData, m_upshift" << std::endl;
	std::cout << R"(#define OFFSET_VEHICLE_HANDLING_SUSPENSION_HEIGHT 0x)" << std::hex << std::uppercase <<
		offsetof(CHandlingData, m_suspension_raise) << " //CHandlingData, m_suspension_raise" << std::endl;
	std::cout << R"(#define OFFSET_VEHICLE_HANDLING_SUSPENSION_FORCE 0x)" << std::hex << std::uppercase <<
		offsetof(CHandlingData, m_suspension_force) << " //CHandlingData, m_suspension_force" << std::endl;
	std::cout << R"(#define OFFSET_VEHICLE_HEADLIGHT_COLOR 0x406 //CVehicle, m_headlight_color)" << std::endl;
	std::cout << R"(#define OFFSET_VEHICLE_MASS 0x)" << std::hex << std::uppercase << offsetof(CHandlingData, m_mass) <<
		" //CHandlingData, m_mass" << std::endl;
	std::cout << R"(#define OFFSET_VEHICLE_INITIAL_DRIVE_FORCE 0x)" << std::hex << std::uppercase <<
		offsetof(CHandlingData, m_initial_drive_force) << " //CHandlingData, m_initial_drive_force" << std::endl;
	std::cout << R"(#define OFFSET_VEHICLE_DRIVE_INITIAL 0x)" << std::hex << std::uppercase <<
		offsetof(CHandlingData, m_drive_inertia) << " //CHandlingData, m_drive_inertia" << std::endl;
	std::cout << R"(#define OFFSET_VEHICLE_THRUST 0x)" << std::hex << std::uppercase <<
		offsetof(CVehicle, m_jump_boost_charge) << " //CVehicle, m_jump_boost_charge" << std::endl;
	std::cout << R"(#define OFFSET_VEHICLE_BOUYANCY 0x)" << std::hex << std::uppercase <<
		offsetof(CHandlingData, m_buoyancy) << " //CHandlingData, m_buoyancy" << std::endl;
	std::cout << R"(#define OFFSET_VEHICLE_HANDBRAKE_FORCE 0x)" << std::hex << std::uppercase <<
		offsetof(CHandlingData, m_handbrake_force) << " //CHandlingData, m_handbrake_force" << std::endl;
	std::cout << R"(#define OFFSET_VEHICLE_DOWNFORCE 0x)" << std::hex << std::uppercase <<
		offsetof(CHandlingData, m_downforce_multiplier) << " //CHandlingData, m_downforce_multiplier" << std::endl;
	std::cout << R"(#define OFFSET_VEHICLE_ROCKET 0x)" << std::hex << std::uppercase <<
		offsetof(CVehicle, m_rocket_recharge_speed) << " //CVehicle, m_rocket_recharge_speed" << std::endl;
	std::cout << R"(#define OFFSET_VEHICLE_ROCKET_MK2 0x)" << std::hex << std::uppercase <<
		offsetof(CVehicle, m_mkii_rocket_recharge_speed) << " //CVehicle, m_mkii_rocket_recharge_speed" << std::endl;
	std::cout << std::endl;
	std::cout << R"(//Offset from WEAPON)" << std::endl;
	std::cout << R"(#define OFFSET_WEAPON_SHOT_TIME 0x)" << std::hex << std::uppercase <<
		offsetof(CWeaponInfo, m_time_between_shots) << " //CWeaponInfo, m_time_between_shots" << std::endl;
	std::cout << R"(#define OFFSET_WEAPON_MANAGER 0x)" << std::hex << std::uppercase << offsetof(CPed, m_weapon_manager)
		<< " //CPed, m_weapon_manager" << std::endl;
	std::cout << R"(#define OFFSET_WEAPON_CURRENT 0x)" << std::hex << std::uppercase <<
		offsetof(CPedWeaponManager, m_weapon_info) << " //CPedWeaponManager, m_weapon_info" << std::endl;
	std::cout << R"(#define OFFSET_WEAPON_AMMOINFO 0x)" << std::hex << std::uppercase <<
		offsetof(CWeaponInfo, m_ammo_info) << " //CWeaponInfo, m_ammo_info" << std::endl;
	std::cout << R"(#define OFFSET_WEAPON_AMMOINFO_MAX 0x)" << std::hex << std::uppercase <<
		offsetof(CAmmoInfo, m_ammo_max) << " //CWeaponInfo, m_ammo_max" << std::endl;
	std::cout << R"(#define OFFSET_WEAPON_AMMOINFO_CURAMMO 0x)" << std::hex << std::uppercase <<
		offsetof(CPedWeaponManager, m_selected_weapon_hash) << " //CPedWeaponManager, m_selected_weapon_hash" <<
		std::endl;
	std::cout << R"(#define OFFSET_WEAPON_SPREAD 0x)" << std::hex << std::uppercase <<
		offsetof(CWeaponInfo, m_accuracy_spread) << " //CWeaponInfo, m_accuracy_spread" << std::endl;
	std::cout << R"(#define OFFSET_WEAPON_BULLET_DMG 0x)" << std::hex << std::uppercase <<
		offsetof(CWeaponInfo, m_damage) << " //CWeaponInfo, m_damage" << std::endl;
	std::cout << R"(#define OFFSET_WEAPON_RECOIL 0x)" << std::hex << std::uppercase <<
		offsetof(CWeaponInfo, m_recoil_shake_amplitude) << " //CWeaponInfo, m_recoil_shake_amplitude" << std::endl;
	std::cout << R"(#define OFFSET_WEAPON_MODEL_HASH 0x)" << std::hex << std::uppercase << offsetof(CItemInfo, m_name)
		<< " //CItemInfo, m_name" << std::endl;
	std::cout << R"(#define OFFSET_WEAPON_NAME_HASH 0x)" << std::hex << std::uppercase << offsetof(CItemInfo, m_name) <<
		" //CItemInfo, m_name" << std::endl;
	std::cout << R"(#define OFFSET_WEAPON_RELOAD_VEHICLE 0x)" << std::hex << std::uppercase <<
		offsetof(CWeaponInfo, m_vehicle_reload_time) << " //CWeaponInfo, m_vehicle_reload_time" << std::endl;
	std::cout << R"(#define OFFSET_WEAPON_RANGE 0x)" << std::hex << std::uppercase <<
		offsetof(CWeaponInfo, m_weapon_range) << " //CWeaponInfo, m_weapon_range" << std::endl;
	std::cout << R"(#define OFFSET_WEAPON_SPINUP 0x)" << std::hex << std::uppercase <<
		offsetof(CWeaponInfo, m_spinup_time) << " //CWeaponInfo, m_spinup_time" << std::endl;
	std::cout << R"(#define OFFSET_WEAPON_SPIN 0x)" << std::hex << std::uppercase << offsetof(CWeaponInfo, m_spin_time)
		<< " //CWeaponInfo, m_spin_time" << std::endl;
	std::cout << R"(#define OFFSET_WEAPON_BULLET_BATCH 0x)" << std::hex << std::uppercase <<
		offsetof(CWeaponInfo, m_bullets_in_batch) << " //CWeaponInfo, m_bullets_in_batch" << std::endl;
	std::cout << R"(#define OFFSET_WEAPON_MUZZLE_VELOCITY 0x)" << std::hex << std::uppercase <<
		offsetof(CWeaponInfo, m_force) << " //CWeaponInfo, m_force" << std::endl;
	std::cout << R"(#define OFFSET_WEAPON_RELOAD_SPEED 0x)" << std::hex << std::uppercase <<
		offsetof(CWeaponInfo, m_reload_time_sp) << " //CWeaponInfo, m_reload_time_sp" << std::endl;
	std::cout << R"(#define OFFSET_WEAPON_RELOAD_MULTIPLY 0x)" << std::hex << std::uppercase <<
		offsetof(CWeaponInfo, m_anim_reload_time) << " //CWeaponInfo, m_anim_reload_time" << std::endl;
}

std::mutex exit_mutex;
void thread_function()
{
	std::lock_guard lock(exit_mutex);
	std::exit(0);
}

void init()
{
	exceptions::init_exception_handler();
	thread_pool_instance = std::make_unique<thread_pool>();
	g_logger = std::make_unique<logger>(BRAND" | Developer (0.00.1, b101)");
	pointers::scan_all();
	//Wait until game is loaded
	while (*pointers::g_loadingScreenState != eLoadingScreenState::Finished)
	{
		std::this_thread::sleep_for(1000ms);
	}
	//Create rendering pointer
	renderer_instance = std::make_unique<renderer>();
	//Create and enable hooks. (Enable is handled in hooking::hooking)
	hooking_instance = std::make_unique<hooking>();
	hooking_instance->enable();
	//Create fibers
	g_pool.create();
	g_manager.add("commands", &commands::on_tick);
	g_manager.add("playerManager", &util::network::manager::onTick);
	g_manager.add("commandStream", &commands::engine::commandStreamTick);
	g_manager.add("script", &script::on_tick);
	//Create our own GTA thread, meant to replace a main hook
	create_thread(&g_manager);
	// DX Thread for drawing Menu
	g_dx_fiber_mgr.add(std::make_unique<fiber>([] {
		while (true)
		{
			ui::drawing::tick();
			image_loader::header_handler();
			ui::draw();
			fiber::current()->sleep();
		}
	}), "dS");
}

void loop()
{
	while (g_running)
	{
		if (GetAsyncKeyState(VK_END))
			g_running = false;
		std::this_thread::sleep_for(1000ms);
	}
}

void uninit()
{
	//Restore the GTA threads we allocated at init
	engine::cleanup_threads();
	//Cleanup fibers
	g_manager.destroy();
	g_dx_fiber_mgr.remove_all();
	//Disable hooks
	hooking_instance->disable();
	//Cleanup renderer
	renderer_instance.reset();
	//We wait a bit before we destory our hooking pointer because MinHook can be a bit slow.
	std::this_thread::sleep_for(350ms);
	hooking_instance.reset();
	thread_pool_instance->destroy();
	thread_pool_instance.reset();
	g_logger.reset();
	exceptions::uninit_exception_handler();
}

DWORD entry_point(LPVOID)
{
	init();
	loop();
	uninit();
	CloseHandle(g_thread);
	if (commands::g_exit_with_clean_up)
	{
		std::thread exit_thread_function(thread_function);
		exit_thread_function.join();
	}
	FreeLibraryAndExitThread(g_module, 0);
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason_for_call, LPVOID)
{
	g_module = module;
	DisableThreadLibraryCalls(g_module);
	if (reason_for_call == DLL_PROCESS_ATTACH)
	{
		g_thread = CreateThread(nullptr, NULL, &entry_point, nullptr, NULL, g_main_thread);
	}
	return TRUE;
}
