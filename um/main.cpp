#include "src/gui/gui.h"
#include "src/hacks/hacks.h"
#include <thread>
#include <iostream>
#include "src/logs/logs.h"
#include "src/utils/driver.h"

void km_check();

int __stdcall wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int)
{
    km_check();

    std::thread(hacks::visuals_thread).detach();

    // start guis
    gui::CreateHWindow(gui::WIN_NAME);
    gui::CreateDevice();
    gui::CreateImGui();

    while (gui::is_runninig)
    {
        gui::BeginRender();
        gui::Render();
        gui::EndRender();

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    gui::DestroyImGui();
    gui::DestroyDevice();
    gui::DestroyHWindow();

    return EXIT_SUCCESS;
}

void km_check()
{
    logs::log(logs::Level::Info, "[i] program started\n");

    globals::process_id = driver::get_process_id("cs2.exe");
    if (!globals::process_id) {
        logs::log(logs::Level::Error, "[e] unable to find process 'cs2.exe'\n");
        return;
    }
    logs::log(logs::Level::Info, "[i] found process id: %p\n", globals::process_id);

    globals::client = driver::get_client(globals::process_id);
    if (!globals::client) {
        logs::log(logs::Level::Error, "[e] unable to find client\n");
        return;
    }
    logs::log(logs::Level::Info, "[i] found client: %p\n", (void*)globals::client);

    globals::engine = driver::get_engine(globals::process_id);
    if (!globals::engine) {
        logs::log(logs::Level::Error, "[e] unable to find engine\n");
        return;
    }
    logs::log(logs::Level::Info, "[i] found engine: %p\n", (void*)globals::engine);
}
