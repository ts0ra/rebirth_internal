#ifndef ESP_REBIRTH
#define ESP_REBIRTH

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
	}

	void draw();
	void drawName();
	void drawBox();
	void drawHealth();
	void drawArmor();
	void drawDistance();
}

#endif
