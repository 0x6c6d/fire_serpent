#include "hacks.h"
#include "../../globals.h"
#include "../gui/gui.h"
#include "../utils/driver.h"
#include <thread>
#include "../logs/logs.h"

void hacks::visuals_thread() noexcept
{
	static bool log_msg = true;
	static bool log_glow = true;
	static bool log_radar = true;
	
	while (gui::is_runninig) 
	{
		std::this_thread::sleep_for(std::chrono::microseconds(1));

		const auto local_player = driver::rpm<std::uintptr_t>(globals::client + offsets::dwLocalPlayerPawn);
		if (!local_player)
		{
			if (log_msg)
				logs::log(logs::Level::Error, "[e] unable to find local_player: %p\n", globals::client + offsets::dwLocalPlayerPawn);
			continue;
		}

		const auto local_team = driver::rpm<std::uintptr_t>(local_player + offsets::m_iTeamNum);
		const auto entity_list = driver::rpm<std::uintptr_t>(globals::client + offsets::dwEntityList);
		if (!entity_list)
		{
			if (log_msg)
				logs::log(logs::Level::Error, "[e] unable to find entity_list: %p\n", globals::client + offsets::dwEntityList);
			continue;
		}

		for(int i = 0; i < 32; i++)
		{
			uintptr_t list_entry1 = driver::rpm<std::uintptr_t>(entity_list + (8 * ((i & 0x7FFF) >> 9)) + 16);
			if (!list_entry1)
			{
				if (log_msg)
					logs::log(logs::Level::Warning, "[w] unable to find list_entry1 for player %d: %p\n", i, entity_list + (8 * ((i & 0x7FFF) >> 9)) + 16);
				continue;
			}

			uintptr_t player_controller = driver::rpm<std::uintptr_t>(list_entry1 + 120 * (i & 0x1FF));
			if (!player_controller)
			{
				if (log_msg)
					logs::log(logs::Level::Warning, "[w] unable to find player_controller for player %d: %p\n", i, list_entry1 + 120 * (i & 0x1FF));
				continue;
			}

			uintptr_t player_pawn = driver::rpm<std::uintptr_t>(player_controller + offsets::m_hPlayerPawn);
			if (!player_pawn)
			{
				if (log_msg)
					logs::log(logs::Level::Warning, "[w] unable to find player_pawn for player %d: %p\n", i, player_controller + offsets::m_hPlayerPawn);
				continue;
			}

			uintptr_t list_entry2 = driver::rpm<std::uintptr_t>(entity_list + 0x8 * ((player_pawn & 0x7FFF) >> 9) + 16);
			if (!list_entry2)
			{
				if (log_msg)
					logs::log(logs::Level::Warning, "[w] unable to find list_entry2 for player %d: %p\n", i, ((player_pawn & 0x7FFF) >> 9) + 16);
				continue;
			}

			uintptr_t p_cs_player_pawn_ptr = driver::rpm<std::uintptr_t>(list_entry2 + 120 * (player_pawn & 0x1FF));
			if (!p_cs_player_pawn_ptr)
			{
				if (log_msg)
					logs::log(logs::Level::Warning, "[w] unable to find p_cs_player_pawn_ptr for player %d: %p\n", i, list_entry2 + 120 * (player_pawn & 0x1FF));
				continue;
			}			

			int team = driver::rpm<std::int32_t>(p_cs_player_pawn_ptr + offsets::m_iTeamNum);
			if (team == local_team)
				continue;

			int health = driver::rpm<std::int32_t>(p_cs_player_pawn_ptr + offsets::m_iHealth);
			if (health <= 0)
				continue;

			if (log_msg)
				logs::log(logs::Level::Info, "[i] found everything for player %d", i);

			// glow hack
			if (globals::glow)
			{
				if (log_glow)
				{
					logs::log(logs::Level::Info, "[i] glow hack active");
					log_glow = false;
				}

				uint8_t r_t = static_cast<uint8_t>(globals::glow_color_t[0] * 255.0f);
				uint8_t g_t = static_cast<uint8_t>(globals::glow_color_t[1] * 255.0f);
				uint8_t b_t = static_cast<uint8_t>(globals::glow_color_t[2] * 255.0f);
				uint8_t a_t = static_cast<uint8_t>(globals::glow_color_t[3] * 255.0f);
				uint8_t r_ct = static_cast<uint8_t>(globals::glow_color_ct[0] * 255.0f);
				uint8_t g_ct = static_cast<uint8_t>(globals::glow_color_ct[1] * 255.0f);
				uint8_t b_ct = static_cast<uint8_t>(globals::glow_color_ct[2] * 255.0f);
				uint8_t a_ct = static_cast<uint8_t>(globals::glow_color_ct[3] * 255.0f);
				uintptr_t glow_color = ((uintptr_t)(0) << 24) | ((uintptr_t)(0) << 16) | ((uintptr_t)(0) << 8) | ((uintptr_t)(0));
				uint32_t glow_type = 1;
				uint32_t glow_enabled = 1;

				// t
				if (team == 2) { glow_color = ((uintptr_t)a_t << 24) | ((uintptr_t)b_t << 16) | ((uintptr_t)g_t << 8) | ((uintptr_t)r_t); }

				// ct
				if (team == 3) { glow_color = ((uintptr_t)a_ct << 24) | ((uintptr_t)b_ct << 16) | ((uintptr_t)g_ct << 8) | ((uintptr_t)r_ct); }

				driver::wpm(p_cs_player_pawn_ptr + offsets::m_Glow + offsets::m_iGlowType, glow_type);
				driver::wpm(p_cs_player_pawn_ptr + offsets::m_Glow + offsets::m_glowColorOverride, glow_color);
				driver::wpm(p_cs_player_pawn_ptr + offsets::m_Glow + offsets::m_bGlowing, glow_enabled);
			}
			else
			{
				if(!log_glow)
					logs::log(logs::Level::Info, "[i] glow hack inactive");
				log_glow = true;
			}

			// radars hack (doesn't work)
			if (globals::radar)
			{
				if (log_radar)
				{
					logs::log(logs::Level::Info, "[i] radar hack active");
					log_radar = false;
				}

				driver::wpm(player_pawn + offsets::m_bSpotted, 1);
			}
			else
			{
				if (!log_radar)
					logs::log(logs::Level::Info, "[i] radar hack inactive");
				log_radar = true;
			}
		}

		log_msg = false;
	}
}