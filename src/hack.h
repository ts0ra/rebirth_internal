#pragma once

namespace hack
{
	namespace toggle
	{
		extern bool aimbot;

		extern bool health;
		extern bool ammo;
		extern bool armor;
		extern bool granade;

		extern bool rapidFire;
		extern bool fullAuto;
		extern bool grenadeNoDelay;

		extern bool map;
		extern bool minimap;
	}

	namespace state
	{
		extern bool rapidFireState;

		extern bool mapState;
		extern bool minimapState;
	}

	void run();
}
