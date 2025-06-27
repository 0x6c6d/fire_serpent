#include "gui.h"
#include "../logs/logs.h"

#include "../../third_party/imgui/imgui.h"
#include "../../third_party/imgui/imgui_impl_dx9.h"
#include "../../third_party/imgui/imgui_impl_win32.h"

/*
 * PROCESS FLOW
 * 1. create Windows window
 * 2. create directx device with that window
 * 3. using those to draw an imgui menu on the window
 */

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND window,
	UINT message,
	WPARAM wide_param,
	LPARAM long_param
);

// handles all the events of a windows window (e.g. klicking)
LRESULT __stdcall WindowProcess(
	HWND window,
	UINT message,
	WPARAM wide_param,
	LPARAM long_param)
{
	// send window process msgs to imgui handler
	if (ImGui_ImplWin32_WndProcHandler(window, message, wide_param, long_param))
		return true;

	switch (message)
	{
		// resizing
		case WM_SIZE: {
			if (gui::device && wide_param != SIZE_MINIMIZED)
			{
				gui::presentParameters.BackBufferWidth = LOWORD(long_param);
				gui::presentParameters.BackBufferHeight = HIWORD(long_param);
				gui::ResetDevice();
			}
		}return 0;

		// disable ALT application menu
		case WM_SYSCOMMAND: {
			if ((wide_param & 0xfff0) == SC_KEYMENU) 
				return 0;
		}break;

		// close window
		case WM_DESTROY: {
			PostQuitMessage(0);
		}return 0;

		// set click points
		case WM_LBUTTONDOWN: {
			gui::position = MAKEPOINTS(long_param);
		}return 0;

		// move window
		case WM_MOUSEMOVE: {
			if (wide_param == MK_LBUTTON)
			{
				const auto points = MAKEPOINTS(long_param);
				auto rect = ::RECT{ };

				GetWindowRect(gui::window, &rect);

				rect.left += points.x - gui::position.x;
				rect.top += points.y - gui::position.y;

				if (gui::position.x >= 0 &&
					gui::position.x <= gui::WIDTH &&
					gui::position.y >= 0 && gui::position.y <= 19)
					SetWindowPos(
						gui::window,
						HWND_TOPMOST,
						rect.left,
						rect.top,
						0, 0,
						SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER
					);
			}

		}return 0;
	}

	return DefWindowProc(window, message, wide_param, long_param);
}

// creates window
void gui::CreateHWindow(const char* window_name) noexcept
{
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_CLASSDC;
	windowClass.lpfnWndProc = WindowProcess;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandleA(0);
	windowClass.hIcon = 0;
	windowClass.hCursor = 0;
	windowClass.hbrBackground = 0;
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = "class001";
	windowClass.hIconSm = 0;

	RegisterClassEx(&windowClass);

	window = CreateWindowEx(
		0,
		"class001",
		window_name,
		WS_POPUP,
		100,
		100,
		WIDTH,
		HEIGHT,
		0,
		0,
		windowClass.hInstance,
		0
	);

	ShowWindow(window, SW_SHOWDEFAULT);
	UpdateWindow(window);
}

// clean up window
void gui::DestroyHWindow() noexcept
{
	DestroyWindow(window);
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}

// create direct x9 device
bool gui::CreateDevice() noexcept
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d)
		return false;

	ZeroMemory(&presentParameters, sizeof(presentParameters));

	presentParameters.Windowed = TRUE;
	presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
	presentParameters.EnableAutoDepthStencil = TRUE;
	presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
	presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	if (d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		window,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&presentParameters,
		&device) < 0)
		return false;

	return true;
}

// called whenever updating the window
void gui::ResetDevice() noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();

	const auto result = device->Reset(&presentParameters);

	if (result == D3DERR_INVALIDCALL)
		IM_ASSERT(0);

	ImGui_ImplDX9_CreateDeviceObjects();
}

// clean up the device
void gui::DestroyDevice() noexcept
{
	if (device)
	{
		device->Release();
		device = nullptr;
	}

	if (d3d)
	{
		d3d->Release();
		d3d = nullptr;
	}
}

// init imgui context
void gui::CreateImGui() noexcept
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ::ImGui::GetIO();

	io.IniFilename = NULL;

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);
}

// clean up imgui context
void gui::DestroyImGui() noexcept
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

// start rendering window
void gui::BeginRender() noexcept
{
	MSG message;
	while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);

		if (message.message == WM_QUIT)
		{
			is_runninig = !is_runninig;
			return;
		}
	}

	// start imgui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

// handle direct x stuff
void gui::EndRender() noexcept
{
	ImGui::EndFrame();

	device->SetRenderState(D3DRS_ZENABLE, FALSE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	if (device->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		device->EndScene();
	}

	const auto result = device->Present(0, 0, 0, 0);

	// handle loss of D3D9 device
	if (result == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		ResetDevice();
}

// stuff to render in the window
void gui::Render() noexcept
{
	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::SetNextWindowSize({ WIDTH, HEIGHT });
	ImGui::Begin(
		"fire_serpent",
		&is_runninig,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove
	);

	// glow hack
	ImGui::Checkbox("glow", &globals::glow);
	ImGui::ColorEdit4("glow color", globals::glow_color);

	// radar hack
	ImGui::Checkbox("radar", &globals::radar);

	// collapsable log msgs
	if (ImGui::CollapsingHeader("Log"))
	{
		ImGui::BeginChild("LogChild", ImVec2(0, 150), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
		const auto& logs = logs::get_logs();

		for (const auto& entry : logs)
		{
			ImVec4 color;

			switch (entry.level) {
			case logs::Level::Info:
				color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // white
				break;
			case logs::Level::Warning:
				color = ImVec4(1.0f, 0.8f, 0.2f, 1.0f); // yellow
				break;
			case logs::Level::Error:
				color = ImVec4(1.0f, 0.3f, 0.3f, 1.0f); // red
				break;
			default:
				color = ImVec4(1, 1, 1, 1);
				break;
			}

			ImGui::PushStyleColor(ImGuiCol_Text, color);
			ImGui::TextUnformatted(entry.message.c_str());
			ImGui::PopStyleColor();
		}

		ImGui::SetScrollHereY(1.0f); // auto-scroll
		ImGui::EndChild();
	}


	ImGui::End();
}