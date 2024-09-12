#include "hack.h"
#include "hooks.h"
#include "data.h"
#include "game_struct.h"
#include "mem.h"
#include "utils.h"
#include "esp.h"

#include <iostream>

namespace hack
{
	namespace toggle
	{
		bool aimbot{ false };

		bool health{ false };
		bool ammo{ false };
		bool armor{ false };
		bool granade{ false };

		bool rapidFire{ false };
		bool fullAuto{ false };
		bool grenadeNoDelay{ false };

		bool map{ false };
		bool minimap{ false };
	}

	namespace state
	{
		bool rapidFireState{ false };

		bool mapState{ false };
		bool minimapState{ false };
	}

	void run()
	{
		if (toggle::map != state::mapState)
		{
			state::mapState = toggle::map;
			
			if (toggle::map)
			{
				mem::patch((BYTE*)offsets::midFunction::radarMap, (BYTE*)"\xE9\x1D\x01\x00\x00\x90", 6);
				//hooks::enableDetour(offsets::function::radarMap);
			}
			else
			{
				mem::patch((BYTE*)offsets::midFunction::radarMap, (BYTE*)"\x0F\x8D\xCC\x00\x00\x00", 6);
				//hooks::disableDetour(offsets::function::radarMap);
			}
		}

		if (toggle::minimap != state::minimapState)
		{
			state::minimapState = toggle::minimap;

			if (toggle::minimap)
			{
				mem::patch((BYTE*)offsets::midFunction::radarMinimap, (BYTE*)"\xE9\x2B\x01\x00\x00\x90", 6);
				//hooks::enableDetour(offsets::function::radarMinimap);
			}
			else
			{
				mem::patch((BYTE*)offsets::midFunction::radarMinimap, (BYTE*)"\x0F\x8D\xD5\x00\x00\x00", 6);
				//hooks::disableDetour(offsets::function::radarMinimap);
			}
		}

		if (toggle::rapidFire != state::rapidFireState)
		{
			state::rapidFireState = toggle::rapidFire;

			if (toggle::rapidFire)
			{
				mem::nop((BYTE*)offsets::midFunction::rapidFire, 2);
			}
			else
			{
				mem::patch((BYTE*)offsets::midFunction::rapidFire, (BYTE*)"\x89\x08", 2);
			}
		}

		if (toggle::fullAuto)
		{
			data::game::localPlayer->wpnPtrCarbine->ptrToGunInfo->isAuto = true;
			data::game::localPlayer->wpnPtrSniper->ptrToGunInfo->isAuto = true;
			data::game::localPlayer->wpnPtrShotgun->ptrToGunInfo->isAuto = true;
			data::game::localPlayer->wpnPtrPistol->ptrToGunInfo->isAuto = true;
		}
		else
		{
			data::game::localPlayer->wpnPtrCarbine->ptrToGunInfo->isAuto = false;
			data::game::localPlayer->wpnPtrSniper->ptrToGunInfo->isAuto = false;
			data::game::localPlayer->wpnPtrShotgun->ptrToGunInfo->isAuto = false;
			data::game::localPlayer->wpnPtrPistol->ptrToGunInfo->isAuto = false;
		}

		if (toggle::grenadeNoDelay)
		{
			data::game::localPlayer->wpnPtrGrenade->ptrToGunInfo->shootDelay = 0;
			data::game::localPlayer->wpnPtrGrenade->throwWait = 0;
		}
		else
		{
			data::game::localPlayer->wpnPtrGrenade->ptrToGunInfo->shootDelay = 650;
			data::game::localPlayer->wpnPtrGrenade->throwWait = 325;
		}

		if (toggle::health)
		{
			data::game::localPlayer->health = 999;
		}

		if (toggle::armor)
		{
			data::game::localPlayer->armor = 999;
		}

		if (toggle::ammo)
		{
			*data::game::localPlayer->wpnPtrSelect->ptrToAmmo = 999;
			*data::game::localPlayer->wpnPtrSelect->ptrToMag = 999;
		}

		if (toggle::granade)
		{
			*data::game::localPlayer->wpnPtrGrenade->ptrToMag = 999;
		}

		if (toggle::aimbot)
		{
			if (!data::game::checkData()) data::game::getData();
			if (data::game::localPlayer->state == 0) // local alive
			{
				float smallestAngleDifference = FLT_MAX; // Initialize with the maximum possible float value
				Vector3 closestTargetViewAngle;
				Vector3 localViewAngle = Vector3(data::game::localPlayer->yaw, data::game::localPlayer->pitch, data::game::localPlayer->roll);

				for (int i = 0; i < *data::game::totalPlayer; ++i)
				{
					playerEnt* tempPlayer = data::game::playerList->players[i];

					if (tempPlayer == nullptr) continue; // skip if invalid player
					if (utils::isTeamGameMode(*data::game::gameMode) &&
						tempPlayer->teamSide == data::game::localPlayer->teamSide) continue; // skip if player is on the same team
					if (tempPlayer->state == 1) continue; // skip if player is dead

					Vector3 tempPlayerHeadPos = tempPlayer->headPos;
					Vector3 targetViewAngle = utils::calculateAngle(data::game::localPlayer->headPos, tempPlayerHeadPos);
					Vector3 targetViewDir = utils::anglesToDirection(tempPlayer->yaw, tempPlayer->pitch);

					float fovX = esp::setting::fovDegree; // Horizontal FOV

					float fovY = utils::calculateVerticalFOV(fovX, data::widthGame, data::heightGame);
					float fovDiameter = utils::calculateFOVCircleDiameter(fovX, fovY, data::widthGame, data::heightGame);

					if (utils::isEnemyWithinFOV(tempPlayer->headPos, fovDiameter))
					{
						float angleDifference = utils::getAngleDifference(localViewAngle.y, targetViewAngle.y) +
												utils::getAngleDifference(localViewAngle.x, targetViewAngle.x);

						if (angleDifference < smallestAngleDifference)
						{
							smallestAngleDifference = angleDifference;
							closestTargetViewAngle = targetViewAngle;
						}
					}
				}
				if ((smallestAngleDifference < FLT_MAX) && (GetAsyncKeyState(VK_RBUTTON) & 0x8000))
				{
					Vector3 smoothedAngles = utils::smoothAim(localViewAngle, closestTargetViewAngle, esp::setting::smoothFactor);
					data::game::localPlayer->yaw = smoothedAngles.x;
					data::game::localPlayer->pitch = smoothedAngles.y;
				}
			}
		}
	}
}