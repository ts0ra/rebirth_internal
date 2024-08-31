#pragma once

#include "types.h"
#include "game_struct.h"

#include "../imgui/imgui.h"

namespace esp
{
	namespace toggle
	{
		extern bool drawESP;
		extern bool name;
		extern bool box;
		extern bool health;
		extern bool armor;
		extern bool distance;
		extern bool drawFOV;
	}

	namespace color
	{
		extern float box[3];
		extern float name[3];
		extern float distance[3];
		extern float fov[3];
	}

	namespace setting
	{
		extern int fovType;
		extern float fovThickness;
		extern float fovRounding;
	}

	namespace dataESP
	{
		extern playerEnt* targetPlayer;
		extern Vector2 headScreenPos;
		extern Vector2 footScreenPos;
	}

	void draw();
	void drawName();
	void drawBox();
	void drawHealth();
	void drawArmor();
	void drawDistance();
	void drawFOV();

	void getHealthColor(float healthPercentage, ImU32& outputColor);
}
