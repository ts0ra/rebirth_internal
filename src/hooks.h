#pragma once

#include "offsets.h"
#include "types.h"

#include <vector>

using wglSwapBuffers = BOOL(__stdcall*)(HDC hdc);
using SDL_SetRelativeMouseMode = int(__cdecl*)(unsigned int mode);
using mousemove = void(__fastcall*)(int idx, int idy);
using midFunction = void(*)();

namespace hook
{
	namespace data
	{
		extern bool unloadState;
		extern bool readyToUnload;
		extern bool isUnload;

		extern std::vector<TrampolineHook> hookList;

		extern SDL_SetRelativeMouseMode SDL_SetRelativeMouseMode_o;

		extern wglSwapBuffers wglSwapBuffers_o;
		extern wglSwapBuffers wglSwapBuffers_t;

		extern WNDPROC WndProc_o;
	}

	void initHooks();
	void createHooks();
	void enableHooks();
	void shutdownHooks();
}

namespace trampoline
{
	namespace function
	{
		BOOL __stdcall wglSwapBuffers_hk(HDC hdc);
		BOOL WINAPI WndProc_hk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		void __fastcall mouseMove_hk(int idx, int idy);
	}

	bool detour(std::uintptr_t src, std::uintptr_t dst, unsigned int size);
	std::uintptr_t create(std::uintptr_t src, std::uintptr_t dst, unsigned int size);
	bool enable(std::uintptr_t src = 0);
	bool disable(std::uintptr_t src = 0);
	bool unhook();
}