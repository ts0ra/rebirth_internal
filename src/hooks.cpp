#include "hooks.h"

#include "gui.h"
#include "esp.h"
#include "data.h"
#include "hack.h"
#include "mem.h"

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

	//minimap originalMap{ nullptr };
	const minimap targetMap = reinterpret_cast<minimap>(offsets::function::radarMap);

	std::vector<customHook> hookStorage;

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
		//detour((BYTE*)targetMap, (BYTE*)(0x45D3E6), 6);
		trampoline((BYTE*)targetMap, (BYTE*)0x45D3E6, 6);
	}

	void shutdownHooks()
	{
		unhookMouse(1);
		//unhookDetour((BYTE*)targetMap, 6, originalMap1);
		MH_STATUS disableStatus = MH_DisableHook(MH_ALL_HOOKS);
		if (disableStatus == MH_OK)
			std::cout << "All hooks disabled\n";
		if (initSuccess == MH_OK)
			MH_Uninitialize();
		gui::shutdownContext();
	}

	bool detour(BYTE* src, BYTE* dst, const uintptr_t len)
	{
		//BYTE_ARRAY tempBytes;

		if (len < 5) return false;

		DWORD oldProtect;
		VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &oldProtect);
		
		// Copy the original bytes to the provided buffer
		//memcpy(tempBytes.data(), src, len);
		//stolenBytes.push_back(tempBytes);

		memset(src, 0x90, len); // Fill the memory with NOPs

		// Use intptr_t to allow for negative offsets
		std::intptr_t relativeAddress = reinterpret_cast<std::intptr_t>(dst) - reinterpret_cast<std::intptr_t>(src) - 5;

		*src = 0xE9; // Opcode for JMP
		*reinterpret_cast<std::intptr_t*>(src + 1) = relativeAddress; // Write the relative address

		VirtualProtect(src, len, oldProtect, &oldProtect);
		return true;
	}

	void trampoline(BYTE* src, BYTE* dst, const uintptr_t len, bool saveTrampoline)
	{
		if (len < 5) return;

		customHook tempHook;
		
		memcpy(tempHook.originalBytes, src, len);
		
		if (saveTrampoline)
		{
			BYTE* gateaway = (BYTE*)VirtualAlloc(0, len, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

			if (gateaway)
			{
				memcpy(gateaway, src, len);

				std::intptr_t gateawayRelativeAddr = reinterpret_cast<std::intptr_t>(src) - reinterpret_cast<std::intptr_t>(gateaway) - 5;

				*(gateaway + len) = 0xE9;

				*reinterpret_cast<std::intptr_t*>(reinterpret_cast<std::intptr_t>(gateaway) + len + 1) = gateawayRelativeAddr;

				tempHook.gateaway = gateaway;
			}
		}
		tempHook.address = reinterpret_cast<std::uintptr_t>(src);
		tempHook.saveTrampoline = saveTrampoline;

		hookStorage.push_back(tempHook);
		
		detour(src, dst, len);
	}

	//void unhookDetour(BYTE* src, const uintptr_t len, BYTE* originalBytes)
	//{
	//	DWORD oldProtect;
	//	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &oldProtect);

	//	memcpy(src, originalBytes, len); // Restore the original bytes

	//	VirtualProtect(src, len, oldProtect, &oldProtect);
	//}
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

	/*void __declspec(naked) detourMap()
	{
		__asm
		{
			mov eax, 0x45D3E6
			jmp eax
		}
	}*/
}