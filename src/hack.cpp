#include "hack.h"
#include "hooks.h"

#include <iostream>

namespace hack
{
	namespace toggle
	{
		bool map{ false };
		bool minimap{ false };
	}

	namespace state
	{
		bool mapIsOn{ false };
		bool minimapIsOn{ false };
	}

	void run()
	{
		if (toggle::map && !state::mapIsOn)
		{
			std::cout << "Active onc\n";
			state::mapIsOn = !state::mapIsOn;
			hooks::enableDetour(offsets::function::radarMap);
		}
		else if (!toggle::map && state::mapIsOn)
		{
			std::cout << "Deactive onc\n";
			state::mapIsOn = !state::mapIsOn;
			hooks::disableDetour(offsets::function::radarMap);
		}
	}
}