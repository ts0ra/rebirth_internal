// paired header file

// other header files
#include "data.h"
#include "console.h"
#include "hooks.h"
#include "gui.h"
#include "esp.h"
// 3rd party header files

// Standard library header files
#include <Windows.h>
#include <thread>
#include <iostream>

// Forward
void mainThread(const HINSTANCE hinstDLL);

BOOL WINAPI DllMain(
    _In_ HINSTANCE hinstDLL,
    _In_ DWORD     fdwReason,
    _In_ LPVOID    lpvReserved
)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hinstDLL);

        const HANDLE mainThreadHandle = CreateThread(
            nullptr,
            0,
            reinterpret_cast<LPTHREAD_START_ROUTINE>(mainThread),
            hinstDLL,
            0,
            nullptr
        );

        if (mainThreadHandle)
            CloseHandle(mainThreadHandle);
    }

    return TRUE;
}

void mainThread(const HINSTANCE hinstDLL)
{
    console::initConsole();
	hooks::initHooks();

	hooks::createHooks();
	hooks::enableHooks();

    while (!GetAsyncKeyState(VK_END) & 1)
    {
		if (GetAsyncKeyState(VK_INSERT) & 1)
		{
            gui::toggleMenu();
		}

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
   
	hooks::shutdownHooks();
    console::shutdownConsole();

    FreeLibraryAndExitThread(hinstDLL, 0);
}