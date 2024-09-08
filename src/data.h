#pragma once

#include "game_struct.h"

#include <Windows.h>

namespace data
{
	extern HMODULE hModule;
	extern HWND hWndGame;
	extern RECT rectGame;
	extern int widthGame;
	extern int heightGame;

	namespace game
	{
		extern std::uintptr_t baseAddress;
		extern playerArray* playerList;
		extern playerEnt* localPlayer;
		extern glmatrixf* mvpmatrix;
		extern int* gameMode;
		extern int* totalPlayer;

		void getData();
		bool checkData();
	}

	void getData(HMODULE hMod);
}