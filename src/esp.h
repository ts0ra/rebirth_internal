#ifndef ESP_REBIRTH
#define ESP_REBIRTH

#include "data.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_opengl2.h"
#include "../imgui/imgui_impl_win32.h"

namespace esp
{
	namespace toggle
	{
		inline bool drawESP{ false };
		inline bool name{ false };
		inline bool box{ false };
		inline bool health{ false };
		inline bool armor{ false };
		inline bool distance{ false };
		inline bool drawFov{ false };
	}

	namespace color
	{
		inline float box[3]{ 1.0f, 1.0f, 1.0f };
		inline float name[3]{ 1.0f, 1.0f, 1.0f };
		inline float distance[3]{ 1.0f, 1.0f, 1.0f };
		inline float fov[3]{ 1.0f, 1.0f, 1.0f };
	}

	namespace setting
	{
		inline int fovType{ 0 };
		inline float fovThickness{ 1.0f };
		inline float fovRounding{ 0.0f };
	}

	void draw();
	void drawName(playerEnt* player);
	void drawBox(playerEnt* player);
	void drawHealth(playerEnt* player);
	void drawArmor(playerEnt* player);
	void drawDistance(playerEnt* player);
	void drawFOV();

	void getHealthColor(float healthPercentage, ImU32& outputColor);
}

#endif
