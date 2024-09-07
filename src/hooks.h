#pragma once

#include "offsets.h"
#include "types.h"

#include "../minhook/MinHook.h"

#include <vector>

//using WndProc = LRESULT(CALLBACK*)(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
using wglSwapBuffers = BOOL(__stdcall*)(HDC hdc);
using SDL_SetRelativeMouseMode = int(__cdecl*)(unsigned int mode);
using mousemove = void(__fastcall*)(int idx, int idy);
using map = void(*)();
using minimap = void(*)();

namespace hooks
{
	//extern MH_STATUS initSuccess;

	extern SDL_SetRelativeMouseMode unhookMouse;

	extern wglSwapBuffers originalSwapBuffers;
	extern wglSwapBuffers targetSwapBuffers;

	extern WNDPROC originalWndProc;

	//extern WndProc originalWndProc;
	//extern const WndProc targetWndProc;

	/*extern mousemove originalMouseMove;
	extern const mousemove targetMouseMove;*/

	//extern const map targetMap;
	//extern const minimap targetMinimap;

	extern std::vector<customHook> hookStorage;

	void initHooks();
	void createHooks();
	void enableHooks();
	void shutdownHooks();

	bool detour(BYTE* src, BYTE* dst, const uintptr_t len);
	// make disable state
	LPVOID trampoline(BYTE* src, BYTE* dst, const uintptr_t len);
	void enableDetour(std::uintptr_t address = 0);
	void disableDetour(std::uintptr_t address = 0);
	void unhookDetour();
}

namespace detours
{
	BOOL __stdcall detourSwapBuffers(HDC hdc);
	BOOL WINAPI detourWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void __fastcall detourMouseMove(int idx, int idy);
}