#ifndef HOOKS_REBIRTH
#define HOOKS_REBIRTH

#include "../minhook/MinHook.h"
#include "data.h"

namespace hooks
{
	typedef LRESULT(CALLBACK* WndProc)(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	typedef BOOL(__stdcall* wglSwapBuffers)(HDC hdc);
	typedef int(__cdecl* SDL_SetRelativeMouseMode)(unsigned int mode);
	typedef void(__fastcall* mousemove)(int idx, int idy);

	inline MH_STATUS initSuccess;

	// unhookMouse(0) : Disable ingame mouse input
	// unhookMouse(1) : Enable ingame mouse input
	inline const SDL_SetRelativeMouseMode unhookMouse = reinterpret_cast<SDL_SetRelativeMouseMode>(GetProcAddress(GetModuleHandle(L"SDL2.dll"), "SDL_SetRelativeMouseMode"));

	inline wglSwapBuffers originalSwapBuffers{ nullptr };
	inline const wglSwapBuffers targetSwapBuffers = reinterpret_cast<wglSwapBuffers>(GetProcAddress(GetModuleHandle(L"opengl32.dll"), "wglSwapBuffers"));

	inline WndProc originalWndProc{ nullptr };
	inline const WndProc targetWndProc = reinterpret_cast<WndProc>(GetWindowLongPtr(FindWindow(NULL, L"AssaultCube"), GWLP_WNDPROC)); // most AC won't detect FindWindow

	inline mousemove originalMouseMove{ nullptr };
	inline const mousemove targetMouseMove = reinterpret_cast<mousemove>(offsets::mousemove);

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
}

#endif