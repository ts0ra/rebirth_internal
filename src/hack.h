#pragma once

namespace hack
{
	namespace toggle
	{
		extern bool enableAimbot;

		extern bool health;
		extern bool ammo;
		extern bool armor;
		extern bool granade;

		extern bool rapidFire;

		extern bool map;
		extern bool minimap;
	}

	namespace state
	{
		extern bool AimbotIsOn;

		extern bool healthIsOn;
		extern bool ammoIsOn;
		extern bool armorIsOn;
		extern bool granadeIsOn;

		extern bool rapidFireIsOn;

		extern bool mapIsOn;
		extern bool minimapIsOn;
	}

	namespace dataWeapon
	{
		extern int currentWeaponFireRate;
	}

	void run();
}
