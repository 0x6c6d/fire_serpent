#include "hacks.h"
#include "../../globals.h"
#include "../gui/gui.h"
#include "../utils/driver.h"
#include <thread>
#include "../logs/logs.h"

void hacks::visuals_thread() noexcept
{
	static bool has_logged = false;
	
	while (gui::is_runninig) 
	{
		std::this_thread::sleep_for(std::chrono::microseconds(1));

		const auto local_player = driver::rpm<std::uintptr_t>(globals::client + offsets::dwLocalPlayerPawn);
		if (!local_player)
		{
			if (!has_logged)
				logs::log(logs::Level::Error, "[e] unable to find local_player: %p\n", globals::client + offsets::dwLocalPlayerPawn);
			continue;
		}

		const auto local_team = driver::rpm<std::uintptr_t>(local_player + offsets::m_iTeamNum);
		const auto entity_list = driver::rpm<std::uintptr_t>(globals::client + offsets::dwEntityList);
		if (!entity_list)
		{
			if (!has_logged)
				logs::log(logs::Level::Error, "[e] unable to find entity_list: %p\n", globals::client + offsets::dwEntityList);
			continue;
		}

		for(int i = 1; i < 32; i++)
		{
			uintptr_t list_entry1 = driver::rpm<std::uintptr_t>(entity_list + (8 * ((i & 0x7FFF) >> 9)) + 0x10);
			if (!list_entry1)
			{
				if (!has_logged)
					logs::log(logs::Level::Warning, "[w] unable to find list_entry1 for player %d: %p\n", i, entity_list + (8 * ((i & 0x7FFF) >> 9)) + 0x10);
				continue;
			}

			uintptr_t player_controller = driver::rpm<std::uintptr_t>(list_entry1 + 120 * (i & 0x1FF));
			if (!player_controller)
			{
				if (!has_logged)
					logs::log(logs::Level::Warning, "[w] unable to find player_controller for player %d: %p\n", i, list_entry1 + 120 * (i & 0x1FF));
				continue;
			}

			uintptr_t player_pawn = driver::rpm<std::uintptr_t>(player_controller + offsets::m_hPlayerPawn);
			if (!player_pawn)
			{
				if (!has_logged)
					logs::log(logs::Level::Warning, "[w] unable to find player_pawn for player %d: %p\n", i, player_controller + offsets::m_hPlayerPawn);
				continue;
			}

			uintptr_t list_entry2 = driver::rpm<std::uintptr_t>(entity_list + 0x8 * ((player_pawn & 0x7FFF) >> 9) + 16);
			if (!list_entry2)
			{
				if (!has_logged)
					logs::log(logs::Level::Warning, "[w] unable to find list_entry2 for player %d: %p\n", i, ((player_pawn & 0x7FFF) >> 9) + 16);
				continue;
			}

			uintptr_t p_cs_player_pawn_ptr = driver::rpm<std::uintptr_t>(list_entry2 + 120 * (player_pawn & 0x1FF));
			if (!p_cs_player_pawn_ptr)
			{
				if (!has_logged)
					logs::log(logs::Level::Warning, "[w] unable to find p_cs_player_pawn_ptr for player %d: %p\n", i, list_entry2 + 120 * (player_pawn & 0x1FF));
				continue;
			}

			int team = driver::rpm<std::int32_t>(p_cs_player_pawn_ptr + offsets::m_iTeamNum);
			if (team == local_team)
				continue;

			int health = driver::rpm<std::int32_t>(p_cs_player_pawn_ptr + offsets::m_iHealth);
			if (health <= 0)
				continue;

			if (!has_logged)
				logs::log(logs::Level::Info, "[i] found everything for player %d", i);

			// glow hack
			if (globals::glow)
			{
				if (!has_logged)
					logs::log(logs::Level::Info, "[i] glow hack active: %p", p_cs_player_pawn_ptr);
				DWORD colorArgb = ((DWORD)(globals::glow_color[3]) << 24) |
					((DWORD)(globals::glow_color[0]) << 16) |
					((DWORD)(globals::glow_color[1]) << 8) |
					((DWORD)(globals::glow_color[2]));

				//driver::wpm((p_cs_player_pawn_ptr + offsets::m_Glow + offsets::m_glowColorOverride), colorArgb);
				//driver::wpm((p_cs_player_pawn_ptr + offsets::m_Glow + offsets::m_bGlowing), 1);
			}

			// radars hack
			if (globals::radar)
			{
				if (!has_logged)
					logs::log(logs::Level::Info, "[i] radar hack active: %p", player_pawn + offsets::m_bSpotted);
				//driver::wpm(player_pawn + offsets::m_bSpotted, true);
			}
		}

		has_logged = true;
	}
}