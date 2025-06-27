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

// https://github.com/a2x/cs2-dumper?tab=readme-ov-file
namespace offsets
{
	constexpr ::std::ptrdiff_t dwLocalPlayerPawn = 0x18560D0;	// offsets.hpp > cs2_dumper > offsets > client_dll
	constexpr ::std::ptrdiff_t dwEntityList = 0x1A020A8;		// offsets.hpp > cs2_dumper > offsets > client_dll

	constexpr ::std::ptrdiff_t m_iTeamNum = 0x3E3;				// client_dll.hpp > cs2_dumper > schemas > client_dll > C_BaseEntity
	constexpr ::std::ptrdiff_t m_lifeState = 0x348;				// client_dll.hpp > cs2_dumper > schemas > client_dll > C_BaseEntity
	constexpr ::std::ptrdiff_t m_iHealth = 0x344;				// client_dll.hpp > cs2_dumper > schemas > client_dll > C_BaseEntity
	constexpr ::std::ptrdiff_t m_hPlayerPawn = 0x824;			// client_dll.hpp > cs2_dumper > schemas > client_dll > CCSPlayerController
	constexpr ::std::ptrdiff_t m_Glow = 0xC00;					// client_dll.hpp > cs2_dumper > schemas > client_dll > C_BaseModelEntity
	constexpr ::std::ptrdiff_t m_glowColorOverride = 0x40;		// client_dll.hpp > cs2_dumper > schemas > client_dll > CGlowProperty
	constexpr ::std::ptrdiff_t m_bGlowing = 0x51;				// client_dll.hpp > cs2_dumper > schemas > client_dll > CGlowProperty
	constexpr ::std::ptrdiff_t m_bSpotted = 0x8;				// client_dll.hpp > cs2_dumper > schemas > client_dll > EntitySpottedState_t

}
