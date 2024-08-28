#ifndef MENU_GUI_REBIRTH
#define MENU_GUI_REBIRTH

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


#endif