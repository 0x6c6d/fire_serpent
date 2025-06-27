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

		const auto local_player = driver::rpm<std::uintptr_t>(globals::client + offsets::dwLocalPlayer);
		if (!local_player)
		{
			if(!has_logged)
				logs::log(logs::Level::Error, "[e] can't find local_player at: %p\n", local_player);
			continue;
		}

		const auto glow_manager = driver::rpm<std::uintptr_t>(globals::client + offsets::dwGlowObjectManager);
		if (!glow_manager)
		{
			if (!has_logged)
				logs::log(logs::Level::Error, "[e] can't find glow_manager at: %p\n", glow_manager);
			continue;
		}

		const auto local_team = driver::rpm<std::uintptr_t>(local_player + offsets::m_iTeamNum);

		for(auto i = 1; i <= 32; ++i)
		{
			// player addr = offset + (index * size of player)
			const auto player = driver::rpm<std::uintptr_t>(globals::client + offsets::dwEntityList + i * 0x10);
			if (!player)
			{
				if (!has_logged)
					logs::log(logs::Level::Error, "[e] can't find player nr. %d at: %p\n", i, player);
				continue;
			}

			const auto team = driver::rpm<std::int32_t>(player + offsets::m_iTeamNum);
			if (team == local_team)
				continue;

			const auto life_state = driver::rpm<std::int32_t>(player + offsets::m_lifeState);
			// life_state zero means that player is alive
			if (life_state != 0)
				continue;

			// glow hack
			if (globals::glow)
			{
				const auto glow_index = driver::rpm<std::int32_t>(player + offsets::m_iGlowIndex);

				driver::wpm(glow_manager + (glow_index * 0x38) + 0x8, globals::glow_color[0]); // red
				driver::wpm(glow_manager + (glow_index * 0x38) + 0xC, globals::glow_color[1]); // green
				driver::wpm(glow_manager + (glow_index * 0x38) + 0x10, globals::glow_color[2]); // blue
				driver::wpm(glow_manager + (glow_index * 0x38) + 0x14, globals::glow_color[3]); // alpha

				driver::wpm(glow_manager + (glow_index * 0x38) + 0x28, true);
				driver::wpm(glow_manager + (glow_index * 0x38) + 0x38, false);
			}

			// radars hack
			if (globals::radar)
			{
				driver::wpm(player + offsets::m_bSpotted, true);
			}
		}

		has_logged = true;
	}
}