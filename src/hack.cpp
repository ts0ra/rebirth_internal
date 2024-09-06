#include "hack.h"
#include "hooks.h"

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

		bool mapIsOn{ false };
		bool minimapIsOn{ false };
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
	}
}