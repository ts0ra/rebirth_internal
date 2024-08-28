#pragma once
#include "../minhook/MinHook.h"

namespace hooks
{
	typedef LRESULT(CALLBACK* WndProc)(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	typedef BOOL(__stdcall* wglSwapBuffers)(HDC hdc);
	typedef int(__cdecl* SDL_SetRelativeMouseMode)(unsigned int mode);
	//typedef void(__fastcall* mousemove)(int idx, int idy);

	static MH_STATUS initSuccess;

	// unhookMouse(0) : Disable ingame mouse input
	// unhookMouse(1) : Enable ingame mouse input
	static SDL_SetRelativeMouseMode unhookMouse = reinterpret_cast<SDL_SetRelativeMouseMode>(GetProcAddress(GetModuleHandle(L"SDL2.dll"), "SDL_SetRelativeMouseMode"));

	static wglSwapBuffers targetSwapBuffers = reinterpret_cast<wglSwapBuffers>(GetProcAddress(GetModuleHandle(L"opengl32.dll"), "wglSwapBuffers"));
	static wglSwapBuffers originalSwapBuffers{ nullptr };

	void initHooks();
	void createHooks();
	void enableHooks();
	void shutdownHooks();
}

namespace detours
{
	BOOL __stdcall detourSwapBuffers(HDC hdc);
	BOOL WINAPI detourWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
}