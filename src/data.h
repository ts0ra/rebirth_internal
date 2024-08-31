#pragma once

#include "offsets.h"
#include "game_struct.h"

#include <Windows.h>

namespace data
{
	inline HWND hWndGame{ nullptr };
	inline RECT rectGame{ 0, 0, 0, 0 };
	inline int widthGame{ 0 };
	inline int heightGame{ 0 };

	namespace game
	{
		inline playerArray* playerList = *reinterpret_cast<playerArray**>(offsets::base::playerList); // use index to access player playerList->players[i]
		inline playerEnt* localPlayer = *reinterpret_cast<playerEnt**>(offsets::base::localPlayer);
		inline glmatrixf* mvpmatrix = reinterpret_cast<glmatrixf*>(offsets::base::modelViewProjectionMatrix);
		inline int* gameMode = reinterpret_cast<int*>(offsets::base::gameMode);
		inline int* totalPlayer = reinterpret_cast<int*>(offsets::base::totalPlayer);
	}
}