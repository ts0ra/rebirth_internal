#include "hack.h"
#include "hooks.h"
#include "data.h"

#include <iostream>

namespace hack
{
	namespace toggle
	{
		bool enableAimbot{ false };

		bool health{ false };
		bool ammo{ false };
		bool armor{ false };
		bool granade{ false };

		bool rapidFire{ false };

		bool map{ false };
		bool minimap{ false };
	}

	namespace state
	{
		bool AimbotIsOn{ false };

		bool healthIsOn{ false };
		bool ammoIsOn{ false };
		bool armorIsOn{ false };
		bool granadeIsOn{ false };

		bool rapidFireIsOn{ false };

		bool mapIsOn{ false };
		bool minimapIsOn{ false };

		bool firstRun{ true };
	}

	namespace dataWeapon
	{
		int originalWpnFireRate;
		//int customWpnFireRate;
	}

	void run()
	{
		if (toggle::map && !state::mapIsOn)
		{
			state::mapIsOn = !state::mapIsOn;
			hooks::enableDetour(offsets::function::radarMap);
		}
		else if (!toggle::map && state::mapIsOn)
		{
			state::mapIsOn = !state::mapIsOn;
			hooks::disableDetour(offsets::function::radarMap);
		}

		if (toggle::minimap && !state::minimapIsOn)
		{
			state::minimapIsOn = !state::minimapIsOn;
			hooks::enableDetour(offsets::function::radarMinimap);
		}
		else if (!toggle::minimap && state::minimapIsOn)
		{
			state::minimapIsOn = !state::minimapIsOn;
			hooks::disableDetour(offsets::function::radarMinimap);
		}

		if (toggle::rapidFire && !state::rapidFireIsOn)
		{
			state::rapidFireIsOn = !state::rapidFireIsOn;
			data::game::localPlayer->wpnPtrSelect->ptrToGunInfo->shootDelay = 0;
		}
		else if (!toggle::rapidFire && state::rapidFireIsOn)
		{
			state::rapidFireIsOn = !state::rapidFireIsOn;
			data::game::localPlayer->wpnPtrSelect->ptrToGunInfo->shootDelay = dataWeapon::originalWpnFireRate;
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

		if (state::firstRun)
		{
			dataWeapon::originalWpnFireRate = data::game::localPlayer->wpnPtrSelect->ptrToGunInfo->shootDelay;
		}

		state::firstRun = false;
	}
}