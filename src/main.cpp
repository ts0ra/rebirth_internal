#include <Windows.h>
#include <thread>

#include "console.h"
#include "hooks.h"
#include "menu.h"


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
    debug::initConsole();
	hooks::initHooks();

	hooks::createHooks();
	hooks::enableHooks();

    while (!GetAsyncKeyState(VK_END))
    {
		if (GetAsyncKeyState(VK_INSERT))
		{
			gui::showMenu = !gui::showMenu;
		}

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

	hooks::shutdownHooks();
	debug::shutdownConsole();

    FreeLibraryAndExitThread(hinstDLL, 0);
}