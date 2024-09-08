#include "hack.h"
#include "hooks.h"
#include "data.h"
#include "game_struct.h"
#include "mem.h"

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
	}
}