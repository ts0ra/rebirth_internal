#include "hooks.h"
#include "data.h"
#include "menu.h"
#include "esp.h"
#include <iostream>



void hooks::initHooks()
{
	hooks::initSuccess = MH_Initialize();
	if (hooks::initSuccess != MH_OK)
		std::cout << "Failed to initialize MinHook\n";
	else
		std::cout << "MinHook initialized\n";

	hooks::createHooks();
}

void hooks::createHooks()
{
	MH_STATUS createSwapBuffers;
}

void hooks::enableHooks()
{
	
}

void hooks::shutdownHooks()
{
	MH_DisableHook(MH_ALL_HOOKS);
	if (hooks::initSuccess == MH_OK)
		MH_Uninitialize();
}

BOOL __stdcall detours::detourSwapBuffers(HDC hdc)
{
	gui::gameContext = wglGetCurrentContext();

	if (!gui::isContextCreated)
	{
		gui::createContext(hdc);
	}

	wglMakeCurrent(hdc, gui::myContext);
	
	gui::startRender();
	gui::menu();
	gui::endRender();

	wglMakeCurrent(hdc, gui::gameContext);

	return hooks::originalSwapBuffers(hdc);
}

BOOL __stdcall detours::detourWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}
