#pragma once
#include <windef.h>

namespace gui
{
	bool isContextCreated{ false };
	HGLRC gameContext{ nullptr };
	HGLRC myContext{ nullptr };
	bool showMenu{ false };

	void createContext(HDC hdc);
	void startRender();
	void menu();
	void endRender();
	void shutdownContext();
}

