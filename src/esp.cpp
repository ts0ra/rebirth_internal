#include "esp.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_opengl2.h"
#include "../imgui/imgui_impl_win32.h"

void esp::draw()
{
	if (esp::toggle::name)
		esp::drawName();

	if (esp::toggle::box)
		esp::drawBox();

	if (esp::toggle::health)
		esp::drawHealth();

	if (esp::toggle::armor)
		esp::drawArmor();

	if (esp::toggle::distance)
		esp::drawDistance();
}

void esp::drawName()
{
	ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(0, 0), ImVec2(100, 100), IM_COL32(255, 0, 0, 255));
}

void esp::drawBox()
{

}

void esp::drawHealth()
{

}

void esp::drawArmor()
{

}

void esp::drawDistance()
{

}
