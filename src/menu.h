#pragma once
#include <Windows.h>

namespace gui
{
	inline bool isContextCreated{ false };
	inline HGLRC gameContext{ nullptr };
	inline HGLRC myContext{ nullptr };
	inline bool showMenu{ false };

	void createContext(HDC hdc);
	void startRender();
	void menu();
	void endRender();
	void shutdownContext();

	void toggleMenu();
}

