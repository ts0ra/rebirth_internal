#include "hooks.h"
#include "data.h"
#include "menu.h"
#include "esp.h"

#include "../imgui/imgui_impl_win32.h"

#include <iostream>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void hooks::initHooks()
{
	hooks::initSuccess = MH_Initialize();
	if (hooks::initSuccess != MH_OK)
		std::cout << "Failed to initialize MinHook\n";
	else
		std::cout << "MinHook initialized\n";
}

void hooks::createHooks()
{
	// swapbuffers
	MH_STATUS createSwapBuffers = MH_CreateHook(
		reinterpret_cast<LPVOID>(hooks::targetSwapBuffers),
		&detours::detourSwapBuffers,
		reinterpret_cast<LPVOID*>(&hooks::originalSwapBuffers)
	);
	if (createSwapBuffers != MH_OK)
		std::cout << "Failed to create SwapBuffers hook\n";
	else
		std::cout << "SwapBuffers hook created\n";

	// wndproc
	MH_STATUS createWndProc = MH_CreateHook(
		reinterpret_cast<LPVOID>(hooks::targetWndProc),
		&detours::detourWndProc,
		reinterpret_cast<LPVOID*>(&hooks::originalWndProc)
	);
	if (createWndProc != MH_OK)
		std::cout << "Failed to create WndProc hook\n";
	else
		std::cout << "WndProc hook created\n";
}

void hooks::enableHooks()
{
	MH_STATUS enableSwapBuffers = MH_EnableHook(hooks::targetSwapBuffers);
	if (enableSwapBuffers != MH_OK)
		std::cout << "Failed to enable SwapBuffers hook\n";
	else
		std::cout << "SwapBuffers hook enabled\n";

	MH_STATUS enableWndProc = MH_EnableHook(hooks::targetWndProc);
	if (enableWndProc != MH_OK)
		std::cout << "Failed to enable WndProc hook\n";
	else
		std::cout << "WndProc hook enabled\n";
}

void hooks::shutdownHooks()
{
	MH_DisableHook(MH_ALL_HOOKS);
	if (hooks::initSuccess == MH_OK)
		MH_Uninitialize();
	gui::shutdownContext();
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
	if (gui::showMenu)
		gui::menu();
	esp::draw();
	gui::endRender();

	wglMakeCurrent(hdc, gui::gameContext);

	return hooks::originalSwapBuffers(hdc);
}

BOOL WINAPI detours::detourWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (gui::showMenu)
	{
		if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
			return 0;

		switch (msg)
		{ // can be used to detect key presses to bypass some anticheat
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_CHAR:
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			return 0;
		}
	}

	return CallWindowProc(hooks::originalWndProc, hwnd, msg, wParam, lParam);
}
