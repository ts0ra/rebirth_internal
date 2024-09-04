#include "hooks.h"

#include "gui.h"
#include "esp.h"
#include "data.h"
#include "hack.h"

#include "../imgui/imgui_impl_win32.h"

#include <iostream>

//#pragma comment(lib, "libMinHook.x86.lib")
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace hooks
{
	MH_STATUS initSuccess;

	// unhookMouse(0) : Disable ingame mouse input
	// unhookMouse(1) : Enable ingame mouse input
	const SDL_SetRelativeMouseMode unhookMouse = reinterpret_cast<SDL_SetRelativeMouseMode>(GetProcAddress(GetModuleHandle(L"SDL2.dll"), "SDL_SetRelativeMouseMode"));

	wglSwapBuffers originalSwapBuffers{ nullptr };
	const wglSwapBuffers targetSwapBuffers = reinterpret_cast<wglSwapBuffers>(GetProcAddress(GetModuleHandle(L"opengl32.dll"), "wglSwapBuffers"));

	WndProc originalWndProc{ nullptr };
	const WndProc targetWndProc = reinterpret_cast<WndProc>(GetWindowLongPtr(FindWindow(NULL, L"AssaultCube"), GWLP_WNDPROC)); // most AC won't detect FindWindow

	mousemove originalMouseMove{ nullptr };
	const mousemove targetMouseMove = reinterpret_cast<mousemove>(offsets::function::mousemove);

	minimap originalMap{ nullptr };
	const minimap targetMap = reinterpret_cast<minimap>(offsets::function::radarMap);

	void initHooks()
	{
		data::hWndGame = FindWindow(NULL, L"AssaultCube");
		GetClientRect(data::hWndGame, &data::rectGame);
		data::widthGame = data::rectGame.right - data::rectGame.left;
		data::heightGame = data::rectGame.bottom - data::rectGame.top;

		initSuccess = MH_Initialize();
		if (initSuccess != MH_OK)
			std::cout << "Failed to initialize MinHook\n";
		else
			std::cout << "MinHook initialized\n";
	}

	void createHooks()
	{
		// swapbuffers
		MH_STATUS createSwapBuffers = MH_CreateHook(
			reinterpret_cast<LPVOID>(targetSwapBuffers),
			&detours::detourSwapBuffers,
			reinterpret_cast<LPVOID*>(&originalSwapBuffers)
		);
		if (createSwapBuffers != MH_OK)
			std::cout << "Failed to create SwapBuffers hook\n";
		else
			std::cout << "SwapBuffers hook created\n";

		// wndproc
		MH_STATUS createWndProc = MH_CreateHook(
			reinterpret_cast<LPVOID>(targetWndProc),
			&detours::detourWndProc,
			reinterpret_cast<LPVOID*>(&originalWndProc)
		);
		if (createWndProc != MH_OK)
			std::cout << "Failed to create WndProc hook\n";
		else
			std::cout << "WndProc hook created\n";

		// mousemove
		MH_STATUS createMouseMove = MH_CreateHook(
			reinterpret_cast<LPVOID>(targetMouseMove),
			&detours::detourMouseMove,
			reinterpret_cast<LPVOID*>(&originalMouseMove)
		);
		if (createMouseMove != MH_OK)
			std::cout << "Failed to create MouseMove hook\n";
		else
			std::cout << "MouseMove hook created\n";

		// map
		MH_STATUS createMap = MH_CreateHook(
			reinterpret_cast<LPVOID>(targetMap),
			&detours::detourMap,
			reinterpret_cast<LPVOID*>(&originalMap)
		);
		if (createMap != MH_OK)
			std::cout << "Failed to create Map hook\n";
		else
			std::cout << "Map hook created\n";
	}

	void enableHooks()
	{
		// swapbuffers
		MH_STATUS enableSwapBuffers = MH_EnableHook(targetSwapBuffers);
		if (enableSwapBuffers != MH_OK)
			std::cout << "Failed to enable SwapBuffers hook\n";
		else
			std::cout << "SwapBuffers hook enabled\n";

		// wndproc
		MH_STATUS enableWndProc = MH_EnableHook(targetWndProc);
		if (enableWndProc != MH_OK)
			std::cout << "Failed to enable WndProc hook\n";
		else
			std::cout << "WndProc hook enabled\n";

		// mousemove
		MH_STATUS enableMouseMove = MH_EnableHook(targetMouseMove);
		if (enableMouseMove != MH_OK)
			std::cout << "Failed to enable MouseMove hook\n";
		else
			std::cout << "MouseMove hook enabled\n";

		// map
		MH_STATUS enableMap = MH_EnableHook(targetMap);
		if (enableMap != MH_OK)
			std::cout << "Failed to enable Map hook\n";
		else
			std::cout << "Map hook enabled\n";
	}

	void shutdownHooks()
	{
		unhookMouse(1);
		MH_STATUS disableStatus = MH_DisableHook(MH_ALL_HOOKS);
		if (disableStatus == MH_OK)
			std::cout << "All hooks disabled\n";
		if (initSuccess == MH_OK)
			MH_Uninitialize();
		gui::shutdownContext();
	}
}

namespace detours
{
	BOOL __stdcall detourSwapBuffers(HDC hdc)
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
		hack::run();

		wglMakeCurrent(hdc, gui::gameContext);

		return hooks::originalSwapBuffers(hdc);
	}

	BOOL WINAPI detourWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
			case WM_MBUTTONDBLCLK:
			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP:
			case WM_MOUSEWHEEL:
			case WM_MOUSEHWHEEL:
				return 0;
			}
		}

		//return CallWindowProc(hooks::originalWndProc, hwnd, msg, wParam, lParam);
		return hooks::originalWndProc(hwnd, msg, wParam, lParam);
	}

	void __fastcall detourMouseMove(int idx, int idy)
	{
		if (gui::showMenu)
			return;

		hooks::originalMouseMove(idx, idy);
	}

	void __declspec(naked) detourMap()
	{
		__asm
		{
			mov eax, 0x45D3E6
			jmp eax
		}
	}
}