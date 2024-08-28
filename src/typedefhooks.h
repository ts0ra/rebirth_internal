#pragma once
#include <Windows.h>

typedef LRESULT(CALLBACK* WndProc)(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
typedef BOOL(__stdcall* wglSwapBuffers)(HDC hdc);
typedef int(__cdecl* SDL_SetRelativeMouseMode)(unsigned int mode);
typedef int(__fastcall* SDL_GetRelativeMouseMode)(void);
typedef void(__fastcall* mousemove)(int idx, int idy);