// paired header file
#include "shared.h"

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

// Function-like macro that evaluates to true if the type (or object) is equal to or smaller than
// the size of two memory addresses
// can be used to determine if a type (or object) is cheap to copy accordingly
// An object of type T is cheap to copy if sizeof(T) <= 2 * sizeof(void*) and has no additional setup costs.
#define isSmall(T) (sizeof(T) <= 2 * sizeof(void*))

// Prefer passing strings using std::string_view (by value) instead of const std::string&, unless your function calls other functions that require C-style strings or std::string parameters.

BOOL WINAPI DllMain(
    _In_ HINSTANCE hinstDLL,
    _In_ DWORD   fdwReason,
    _In_ LPVOID  lpvReserved
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

// Forward
void mainThread(HINSTANCE hinstDLL)
{
    console::initConsole();
    data::getData((HMODULE)hinstDLL);
    data::game::getData();

    hook::initHooks();
    hook::createHooks();
    hook::enableHooks();
    /*while (!GetAsyncKeyState(VK_END) & 1)
    {
        if (GetAsyncKeyState(VK_INSERT) & 1)
        {
            gui::toggleMenu();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    hook::shutdownHooks();
    console::shutdownConsole();

    FreeLibraryAndExitThread(hinstDLL, 0);*/
}

void exitThread()
{
    FreeLibraryAndExitThread(data::hModule, 0);
}
