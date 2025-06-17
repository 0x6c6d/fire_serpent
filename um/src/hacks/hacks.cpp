#include "hacks.h"
#include "../../globals.h"
#include "../gui/gui.h"
#include "../utils/driver.h"
#include <thread>

void hacks::visuals_thread() noexcept
{
	while (gui::is_runninig) 
	{
		std::this_thread::sleep_for(std::chrono::microseconds(1));

		const auto local_player = driver::rpm<std::uintptr_t>(globals::client + offsets::dwLocalPlayer);
		if (!local_player)
			continue;

		const auto glow_manager = driver::rpm<std::uintptr_t>(globals::client + offsets::dwGlowObjectManager);
		if (!glow_manager)
			continue;

		const auto local_team = driver::rpm<std::uintptr_t>(local_player + offsets::m_iTeamNum);

		for(auto i = 1; i <= 32; ++i)
		{
			// player addr = offset + (index * size of player)
			const auto player = driver::rpm<std::uintptr_t>(globals::client + offsets::dwEntityList + i * 0x10);
			if (!player)
				continue;

			const auto team = driver::rpm<std::uintptr_t>(player + offsets::m_iTeamNum);
			if (team == local_team)
				continue;

			const auto life_state = driver::rpm<std::uintptr_t>(player + offsets::m_lifeState);
			// life_state zero means that player is alive
			if (life_state != 0)
				continue;


		}
	}
}