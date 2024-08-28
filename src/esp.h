#pragma once

namespace esp
{
	namespace toggle
	{
		static bool name{ false };
		static bool box{ false };
		static bool health{ false };
		static bool armor{ false };
		static bool distance{ false };
	}

	void draw();
	void drawName();
	void drawBox();
	void drawHealth();
	void drawArmor();
	void drawDistance();
}
