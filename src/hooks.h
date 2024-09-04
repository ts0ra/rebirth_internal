#pragma once

#include "offsets.h"

#include "../minhook/MinHook.h"

using WndProc = LRESULT(CALLBACK*)(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
using wglSwapBuffers = BOOL(__stdcall*)(HDC hdc);
using SDL_SetRelativeMouseMode = int(__cdecl*)(unsigned int mode);
using mousemove = void(__fastcall*)(int idx, int idy);
using minimap = void(*)();

namespace hooks
{
	extern MH_STATUS initSuccess;

	extern const SDL_SetRelativeMouseMode unhookMouse;

	extern wglSwapBuffers originalSwapBuffers;
	extern const wglSwapBuffers targetSwapBuffers;

	extern WndProc originalWndProc;
	extern const WndProc targetWndProc;

	extern mousemove originalMouseMove;
	extern const mousemove targetMouseMove;

	extern minimap originalMap;
	extern const minimap targetMap;

	void initHooks();
	void createHooks();
	void enableHooks();
	void shutdownHooks();
}

namespace detours
{
	BOOL __stdcall detourSwapBuffers(HDC hdc);
	BOOL WINAPI detourWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void __fastcall detourMouseMove(int idx, int idy);
	void detourMap();
}