#include "data.h"

#include "offsets.h"

namespace data
{
	HMODULE hModule;
	HWND hWndGame;
	RECT rectGame;
	int widthGame;
	int heightGame;

	namespace game
	{
		std::uintptr_t baseAddress;
		playerArray* playerList;
		playerEnt* localPlayer;
		glmatrixf* mvpmatrix;
		int* gameMode;
		int* totalPlayer;

		void getData()
		{
			std::uintptr_t baseAddress = reinterpret_cast<std::uintptr_t>(GetModuleHandle(L"ac_client.exe"));
			game::playerList = *reinterpret_cast<playerArray**>(offsets::base::playerList);
			game::localPlayer = *reinterpret_cast<playerEnt**>(offsets::base::localPlayer);
			game::mvpmatrix = reinterpret_cast<glmatrixf*>(offsets::base::modelViewProjectionMatrix);
			game::gameMode = reinterpret_cast<int*>(offsets::base::gameMode);
			game::totalPlayer = reinterpret_cast<int*>(offsets::base::totalPlayer);
		}

		bool checkData()
		{
			return game::playerList != nullptr &&
				game::localPlayer != nullptr &&
				game::mvpmatrix != nullptr &&
				game::gameMode != nullptr &&
				game::totalPlayer != nullptr;
		}
	}

	void getData(HMODULE hMod)
	{
		data::hModule = hMod;
		hWndGame = FindWindow(NULL, L"AssaultCube");
		GetClientRect(data::hWndGame, &data::rectGame);
		widthGame = data::rectGame.right - data::rectGame.left;
		heightGame = data::rectGame.bottom - data::rectGame.top;
	}

}