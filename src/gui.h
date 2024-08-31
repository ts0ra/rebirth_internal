#pragma once

#include <Windows.h>

namespace gui
{
	extern bool isContextCreated;
	extern HGLRC gameContext;
	extern HGLRC myContext;
	extern bool showMenu;

	void createContext(HDC hdc);
	void startRender();
	void menu();
	void endRender();
	void shutdownContext();

	void toggleMenu();
}