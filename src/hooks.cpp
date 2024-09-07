#include "hooks.h"

#include "gui.h"
#include "esp.h"
#include "data.h"
#include "hack.h"
#include "mem.h"

#include "../imgui/imgui_impl_win32.h"

#include <iostream>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace hooks
{
	// unhookMouse(0) : Disable ingame mouse input
	// unhookMouse(1) : Enable ingame mouse input
	SDL_SetRelativeMouseMode unhookMouse;

	wglSwapBuffers originalSwapBuffers{ nullptr };
	wglSwapBuffers targetSwapBuffers;

	WNDPROC originalWndProc;

	//WndProc originalWndProc{ nullptr };
	//const WndProc targetWndProc = reinterpret_cast<WndProc>(GetWindowLongPtr(FindWindow(NULL, L"AssaultCube"), GWLP_WNDPROC)); 

	//mousemove originalMouseMove{ nullptr };
	//const mousemove targetMouseMove = reinterpret_cast<mousemove>(offsets::function::mousemove);

	//const map targetMap = reinterpret_cast<map>(offsets::function::radarMap);
	//const minimap targetMinimap = reinterpret_cast<minimap>(offsets::function::radarMinimap);

	std::vector<customHook> hookStorage;

	void initHooks()
	{
		data::hWndGame = FindWindow(NULL, L"AssaultCube");
		GetClientRect(data::hWndGame, &data::rectGame);
		data::widthGame = data::rectGame.right - data::rectGame.left;
		data::heightGame = data::rectGame.bottom - data::rectGame.top;

		unhookMouse = reinterpret_cast<SDL_SetRelativeMouseMode>(GetProcAddress(GetModuleHandle(L"SDL2.dll"), "SDL_SetRelativeMouseMode"));
		targetSwapBuffers = reinterpret_cast<wglSwapBuffers>(GetProcAddress(GetModuleHandle(L"opengl32.dll"), "wglSwapBuffers"));
		originalWndProc = (WNDPROC)SetWindowLongPtr(FindWindow(NULL, L"AssaultCube"), GWLP_WNDPROC, (LONG_PTR)detours::detourWndProc); // most AC won't detect FindWindow

		/*initSuccess = MH_Initialize();
		if (initSuccess != MH_OK)
			std::cout << "Failed to initialize MinHook\n";
		else
			std::cout << "MinHook initialized\n";*/
	}

	void createHooks()
	{
		originalSwapBuffers = (wglSwapBuffers)trampoline((BYTE*)targetSwapBuffers, (BYTE*)detours::detourSwapBuffers, 5);
		// swapbuffers
		/*MH_STATUS createSwapBuffers = MH_CreateHook(
			reinterpret_cast<LPVOID>(targetSwapBuffers),
			&detours::detourSwapBuffers,
			reinterpret_cast<LPVOID*>(&originalSwapBuffers)
		);
		if (createSwapBuffers != MH_OK)
			std::cout << "Failed to create SwapBuffers hook\n";
		else
			std::cout << "SwapBuffers hook created\n";*/

		// wndproc
		/*MH_STATUS createWndProc = MH_CreateHook(
			reinterpret_cast<LPVOID>(targetWndProc),
			&detours::detourWndProc,
			reinterpret_cast<LPVOID*>(&originalWndProc)
		);
		if (createWndProc != MH_OK)
			std::cout << "Failed to create WndProc hook\n";
		else
			std::cout << "WndProc hook created\n";*/

		// mousemove
		/*MH_STATUS createMouseMove = MH_CreateHook(
			reinterpret_cast<LPVOID>(targetMouseMove),
			&detours::detourMouseMove,
			reinterpret_cast<LPVOID*>(&originalMouseMove)
		);
		if (createMouseMove != MH_OK)
			std::cout << "Failed to create MouseMove hook\n";
		else
			std::cout << "MouseMove hook created\n";*/

		// map
		//trampoline((BYTE*)targetMap, (BYTE*)0x45D3E6, 6);

		// minimap
		//trampoline((BYTE*)targetMinimap, (BYTE*)0x45C606, 6); // maybe still broken, take a look later
	}

	void enableHooks()
	{
		enableDetour((std::uintptr_t)targetSwapBuffers);
		// swapbuffers
		/*MH_STATUS enableSwapBuffers = MH_EnableHook(targetSwapBuffers);
		if (enableSwapBuffers != MH_OK)
			std::cout << "Failed to enable SwapBuffers hook\n";
		else
			std::cout << "SwapBuffers hook enabled\n";*/

		// wndproc
		/*MH_STATUS enableWndProc = MH_EnableHook(targetWndProc);
		if (enableWndProc != MH_OK)
			std::cout << "Failed to enable WndProc hook\n";
		else
			std::cout << "WndProc hook enabled\n";*/

		// mousemove
		/*MH_STATUS enableMouseMove = MH_EnableHook(targetMouseMove);
		if (enableMouseMove != MH_OK)
			std::cout << "Failed to enable MouseMove hook\n";
		else
			std::cout << "MouseMove hook enabled\n";*/
	}

	void shutdownHooks()
	{
		if (gui::showMenu)
			gui::toggleMenu();
		unhookDetour();
		/*MH_STATUS disableStatus = MH_DisableHook(MH_ALL_HOOKS);
		if (disableStatus == MH_OK)
		if (initSuccess == MH_OK)
			MH_Uninitialize();*/
		SetWindowLongPtr(data::hWndGame, GWLP_WNDPROC, (LONG_PTR)originalWndProc);
		gui::shutdownContext();
	}

	bool detour(BYTE* src, BYTE* dst, const uintptr_t len)
	{

		if (len < 5) return false;

		DWORD oldProtect;
		VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &oldProtect);

		memset(src, 0x90, len); // Fill the memory with NOPs

		// Use intptr_t to allow for negative offsets
		std::intptr_t relativeAddress = reinterpret_cast<std::intptr_t>(dst) - reinterpret_cast<std::intptr_t>(src) - 5;

		*src = 0xE9; // Opcode for JMP
		*reinterpret_cast<std::intptr_t*>(src + 1) = relativeAddress; // Write the relative address

		VirtualProtect(src, len, oldProtect, &oldProtect);
		return true;
	}

	LPVOID trampoline(BYTE* src, BYTE* dst, const uintptr_t len)
	{
		if (len < 5) return nullptr;

		customHook tempHook;
		
		memcpy(tempHook.originalBytes, src, len);
		
		BYTE* gateaway = (BYTE*)VirtualAlloc(0, len, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

		if (gateaway)
		{
			memcpy(gateaway, src, len);

			std::intptr_t gateawayRelativeAddr = reinterpret_cast<std::intptr_t>(src) - reinterpret_cast<std::intptr_t>(gateaway) - 5;

			*(gateaway + len) = 0xE9;

			*reinterpret_cast<std::intptr_t*>(reinterpret_cast<std::intptr_t>(gateaway) + len + 1) = gateawayRelativeAddr;

			tempHook.gateaway = gateaway;
		}

		tempHook.address = reinterpret_cast<std::uintptr_t>(src);
		tempHook.length = len;
		tempHook.dst = reinterpret_cast<std::uintptr_t>(dst);

		hookStorage.push_back(tempHook);
		
		return gateaway;
		//detour(src, dst, len);
	}

	void enableDetour(std::uintptr_t address)
	{
		if (address)
		{
			for (auto& hook : hookStorage)
			{
				if (hook.address == address)
				{
					detour((BYTE*)hook.address, (BYTE*)hook.dst, hook.length);
				}
			}

			return;
		}

		for (auto& hook : hookStorage)
		{
			detour((BYTE*)hook.address, (BYTE*)hook.dst, hook.length);
		}
	}

	void disableDetour(std::uintptr_t address)
	{
		if (address)
		{
			for (auto& hook : hookStorage)
			{
				if (hook.address == address)
				{
					DWORD oldProtect;
					VirtualProtect((LPVOID)hook.address, hook.length, PAGE_EXECUTE_READWRITE, &oldProtect);

					memcpy((LPVOID)hook.address, hook.originalBytes, hook.length);

					VirtualProtect((LPVOID)hook.address, hook.length, oldProtect, &oldProtect);
				}
			}

			return;
		}

		for (auto& hook : hookStorage)
		{
			DWORD oldProtect;
			VirtualProtect((LPVOID)hook.address, hook.length, PAGE_EXECUTE_READWRITE, &oldProtect);

			memcpy((LPVOID)hook.address, hook.originalBytes, hook.length);

			VirtualProtect((LPVOID)hook.address, hook.length, oldProtect, &oldProtect);
		}
	}

	void unhookDetour()
	{
		for (auto& hook : hookStorage)
		{
			DWORD oldProtect;
			VirtualProtect((LPVOID)hook.address, hook.length, PAGE_EXECUTE_READWRITE, &oldProtect);

			memcpy((LPVOID)hook.address, hook.originalBytes, hook.length);

			VirtualProtect((LPVOID)hook.address, hook.length, oldProtect, &oldProtect);

			VirtualFree(hook.gateaway, 0, MEM_RELEASE);
		}
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

		return CallWindowProc(hooks::originalWndProc, hwnd, msg, wParam, lParam);
		//return hooks::originalWndProc(hwnd, msg, wParam, lParam);
	}

	void __fastcall detourMouseMove(int idx, int idy)
	{
		if (gui::showMenu)
			return;

		//hooks::originalMouseMove(idx, idy);
	}
}