#include "hooks.h"

#include "gui.h"
#include "esp.h"
#include "data.h"
#include "hack.h"
#include "mem.h"
#include "console.h"
#include "shared.h"

#include "../imgui/imgui_impl_win32.h"

#include <iostream>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace hook
{
	namespace data
	{
		bool unloadState;
		bool readyToUnload;
		bool isUnload;

		std::vector<TrampolineHook> hookList;

		SDL_SetRelativeMouseMode SDL_SetRelativeMouseMode_o;

		wglSwapBuffers wglSwapBuffers_o;
		wglSwapBuffers wglSwapBuffers_t;

		WNDPROC WndProc_o;
	}

	void initHooks()
	{
		data::unloadState = false;
		data::readyToUnload = false;
		data::isUnload = false;

		data::SDL_SetRelativeMouseMode_o = reinterpret_cast<SDL_SetRelativeMouseMode>(GetProcAddress(GetModuleHandle(L"SDL2.dll"), "SDL_SetRelativeMouseMode"));
		data::wglSwapBuffers_t = reinterpret_cast<wglSwapBuffers>(GetProcAddress(GetModuleHandle(L"opengl32.dll"), "wglSwapBuffers"));
	}

	void createHooks()
	{
		// most AC won't detect FindWindow
		data::WndProc_o = reinterpret_cast<WNDPROC>(SetWindowLongPtr(FindWindow(NULL, L"AssaultCube"), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(trampoline::function::WndProc_hk)));
		
		std::uintptr_t wglSwapBuffers_s = trampoline::create(
			reinterpret_cast<std::uintptr_t>(data::wglSwapBuffers_t),
			reinterpret_cast<std::uintptr_t>(trampoline::function::wglSwapBuffers_hk),
			5
		);

		if (!wglSwapBuffers_s) std::cout << "Failed to create hook for wglSwapBuffers\n";

		data::wglSwapBuffers_o = reinterpret_cast<wglSwapBuffers>(wglSwapBuffers_s);
	}

	void enableHooks()
	{
		trampoline::enable();
	}

	void shutdownHooks()
	{
		if (!data::isUnload)
		{
			if (gui::showMenu)
				gui::toggleMenu();
			console::shutdownConsole();
			gui::shutdownContext();
			trampoline::disable();
			SetWindowLongPtr(FindWindow(NULL, L"AssaultCube"), GWLP_WNDPROC, (LONG_PTR)hook::data::WndProc_o);
			trampoline::unhook();

			data::isUnload = true;
		}
		CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(exitThread), nullptr, 0, nullptr);
	}
}

namespace trampoline
{
	namespace function
	{
		BOOL __stdcall wglSwapBuffers_hk(HDC hdc)
		{
			hook::data::readyToUnload = false;

			gui::gameContext = wglGetCurrentContext();

			if (!gui::isContextCreated)
			{
				gui::createContext(hdc);
			}

			if (!data::game::checkData())
			{
				data::game::getData();
			}

			wglMakeCurrent(hdc, gui::myContext);

			gui::startRender();
			if (gui::showMenu)
				gui::menu();
			esp::draw();
			gui::endRender();
			hack::run();

			wglMakeCurrent(hdc, gui::gameContext);

			hook::data::readyToUnload = true;

			return hook::data::wglSwapBuffers_o(hdc);
		}

		BOOL WINAPI WndProc_hk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			if (hook::data::readyToUnload && hook::data::unloadState)
			{
				hook::shutdownHooks();
				return 0;
			}

			switch (msg)
			{
			case WM_KEYDOWN:
				switch (wParam)
				{
				case VK_INSERT:
					gui::toggleMenu();
					break;
				case VK_END:
					hook::data::unloadState = true;
					hook::data::readyToUnload = false;
					break;
				}
			}

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

			return CallWindowProc(hook::data::WndProc_o, hwnd, msg, wParam, lParam);
		}
	}

	bool detour(std::uintptr_t src, std::uintptr_t dst, unsigned int size)
	{
		if (size < 5) return false;

		DWORD oldProtect;
		if (!VirtualProtect(reinterpret_cast<LPVOID>(src), size, PAGE_EXECUTE_READWRITE, &oldProtect)) return false;

		std::memset(reinterpret_cast<void*>(src), 0x90, size);			// set all bytes to NOP

		// cast to std::intptr_t for signed arithmetic
		std::intptr_t src_addr = static_cast<std::intptr_t>(src);
		std::intptr_t dst_addr = static_cast<std::intptr_t>(dst);
		std::intptr_t relativeAddress = dst_addr - src_addr - 5;		// calculate relative address

		*reinterpret_cast<std::uint8_t*>(src) = 0xE9;					// set first byte to JMP
		*reinterpret_cast<std::intptr_t*>(src + 1) = relativeAddress;	// set the next 4 bytes to the relative address

		if (!VirtualProtect(reinterpret_cast<LPVOID>(src), size, oldProtect, &oldProtect)) return false;

		return true;
	}

	// If the function succeeds, the return value is nonzero (gateaway address).
	// If the function fails, the return value is zero.
	std::uintptr_t create(std::uintptr_t src, std::uintptr_t dst, unsigned int size)
	{
		if (size < 5) return 0;

		TrampolineHook tempHook{};
		DWORD oldProtect;

		if (!VirtualProtect(reinterpret_cast<LPVOID>(src), size, PAGE_EXECUTE_READWRITE, &oldProtect)) return 0;

		std::memcpy(tempHook.stolenBytes.data(), reinterpret_cast<void*>(src), size);

		std::uintptr_t gateaway = reinterpret_cast<std::uintptr_t>(VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
		if (!gateaway) return 0;

		std::memcpy(reinterpret_cast<void*>(gateaway), reinterpret_cast<void*>(src), size);

		std::intptr_t src_addr = static_cast<std::intptr_t>(src);
		std::intptr_t gateaway_addr = static_cast<std::intptr_t>(gateaway);
		std::intptr_t gateawayRelativeAddr = src_addr - gateaway_addr - 5;

		*(reinterpret_cast<uint8_t*>(gateaway) + size) = 0xE9;
		*reinterpret_cast<std::intptr_t*>(reinterpret_cast<uint8_t*>(gateaway) + size + 1) = gateawayRelativeAddr;

		tempHook.targetFunction = src;
		tempHook.hookFunction = dst;
		tempHook.gateawayFunction = gateaway;
		tempHook.size = size;

		hook::data::hookList.push_back(tempHook);

		if (!VirtualProtect(reinterpret_cast<LPVOID>(src), size, oldProtect, &oldProtect)) return 0;

		return gateaway;
	}

	bool enable(std::uintptr_t src)
	{
		if (src)
		{
			for (auto& hook : hook::data::hookList)
			{
				if (hook.targetFunction == src)
				{
					return detour(hook.targetFunction, hook.hookFunction, hook.size);
				}
			}
		}

		for (auto& hook : hook::data::hookList)
		{
			if (!detour(hook.targetFunction, hook.hookFunction, hook.size)) return false;
		}
		return true;
	}

	bool disable(std::uintptr_t src)
	{
		if (src)
		{
			for (auto& hook : hook::data::hookList)
			{
				if (hook.targetFunction == src)
				{
					DWORD oldProtect;
					if (!VirtualProtect(reinterpret_cast<LPVOID>(hook.targetFunction), hook.size, PAGE_EXECUTE_READWRITE, &oldProtect)) return false;

					std::memcpy(reinterpret_cast<void*>(hook.targetFunction), hook.stolenBytes.data(), hook.size);

					if (!VirtualProtect(reinterpret_cast<LPVOID>(hook.targetFunction), hook.size, oldProtect, &oldProtect)) return false;

					return true;
				}
			}
		}

		for (auto& hook : hook::data::hookList)
		{
			DWORD oldProtect;
			if (!VirtualProtect(reinterpret_cast<LPVOID>(hook.targetFunction), hook.size, PAGE_EXECUTE_READWRITE, &oldProtect)) return false;

			std::memcpy(reinterpret_cast<void*>(hook.targetFunction), hook.stolenBytes.data(), hook.size);

			if (!VirtualProtect(reinterpret_cast<LPVOID>(hook.targetFunction), hook.size, oldProtect, &oldProtect)) return false;
		}
		return true;
	}

	bool unhook()
	{
		for (auto& hook : hook::data::hookList)
		{
			DWORD oldProtect;
			if (!VirtualProtect(reinterpret_cast<LPVOID>(hook.targetFunction), hook.size, PAGE_EXECUTE_READWRITE, &oldProtect)) return false;

			std::memcpy(reinterpret_cast<void*>(hook.targetFunction), hook.stolenBytes.data(), hook.size);

			if (!VirtualProtect(reinterpret_cast<LPVOID>(hook.targetFunction), hook.size, oldProtect, &oldProtect)) return false;

			if (!VirtualFree(reinterpret_cast<LPVOID>(hook.gateawayFunction), 0, MEM_RELEASE)) return false;
		}
		return true;
	}

}