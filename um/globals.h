#pragma once
#include <Windows.h>
#include <cstdint>

namespace globals {
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
}
