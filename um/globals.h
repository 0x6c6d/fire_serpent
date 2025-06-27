#pragma once
#include <Windows.h>
#include <cstdint>
#include <cstddef>

namespace globals 
{
	// gui
	inline auto window_name = "FS Menu";
	inline constexpr int window_w = 500;
	inline constexpr int window_h = 300;

	// memory
	inline HANDLE process_id = NULL;
	inline uintptr_t module_base = NULL;
	inline uintptr_t peb_address = NULL;

	inline uintptr_t client = NULL;
	inline uintptr_t engine = NULL;
	inline uintptr_t client_size = NULL;
	inline uintptr_t engine_size = NULL;

    // hacks
    inline bool glow = false;
	inline float glow_color[] = { 1.f, 0.f, 0.f, 1.f }; // rgba: equal to 255, 0, 0, 255

	inline bool radar = false;
}

// https://github.com/sezzyaep/CS2-OFFSETS/
namespace offsets
{
	constexpr ::std::ptrdiff_t dwLocalPlayer = 0x1912578;
	constexpr ::std::ptrdiff_t dwGlowObjectManager = 0x19200C0;
	constexpr ::std::ptrdiff_t dwEntityList = 0x18C2D58;

	constexpr ::std::ptrdiff_t m_iTeamNum = 0x3CB;
	constexpr ::std::ptrdiff_t m_lifeState = 0x338;
	constexpr ::std::ptrdiff_t m_iGlowIndex = 0x10488;
	constexpr ::std::ptrdiff_t m_bSpotted = 0x93D;
}
