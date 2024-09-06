#include "esp.h"
#include "data.h"
#include "utils.h"

#include <iostream>

namespace esp
{
	namespace toggle
	{
		bool drawESP{ false };
		bool name{ false };
		bool box{ false };
		bool health{ false };
		bool armor{ false };
		bool distance{ false };
		bool drawFOV{ false };
	}

	namespace color
	{
		float box[3]{ 1.0f, 1.0f, 1.0f };
		float name[3]{ 1.0f, 1.0f, 1.0f };
		float distance[3]{ 1.0f, 1.0f, 1.0f };
		float fov[3]{ 1.0f, 1.0f, 1.0f };
	}

	namespace setting
	{
		int fovType{ 0 };
		float fovThickness{ 1.0f };
		float fovRounding{ 0.0f };
	}

	namespace dataESP
	{
		playerEnt* targetPlayer{ nullptr };
		Vector2 headScreenPos{};
		Vector2 footScreenPos{};
	}

	void draw()
	{
		if (toggle::drawESP)
		{
			for (int i = 0; i < *data::game::totalPlayer; ++i)
			{
				if (data::game::playerList == nullptr) { data::renewData(); continue; } // renew data if playerList is invalid
				
				dataESP::targetPlayer = data::game::playerList->players[i];

				if (dataESP::targetPlayer == nullptr) continue; // skip if invalid player
				if (utils::isTeamGameMode(*data::game::gameMode) &&
					dataESP::targetPlayer->teamSide == data::game::localPlayer->teamSide) continue; // skip if player is on the same team
				if (dataESP::targetPlayer->state == 1) continue; // skip if player is dead
				
				Vector3 newHeadPos = dataESP::targetPlayer->headPos + Vector3{ 0.0f, 0.0f, 0.70f }; // offset the head position
				Vector3 newFootPos = dataESP::targetPlayer->footPos - Vector3{ 0.0f, 0.0f, 0.15f }; // offset the foot position

				if (utils::worldToScreen(newHeadPos, dataESP::headScreenPos, data::widthGame, data::heightGame) &&
					utils::worldToScreen(newFootPos, dataESP::footScreenPos, data::widthGame, data::heightGame))
				{
					if (toggle::name)
						drawName();

					if (toggle::box)
						drawBox();

					if (toggle::health)
						drawHealth();

					if (toggle::armor)
						drawArmor();

					if (toggle::distance)
						drawDistance();
				}
			}
		}

		if (toggle::drawFOV)
			drawFOV();
	}

	void drawName()
	{
		float textWidht = ImGui::CalcTextSize(dataESP::targetPlayer->name).x;
		ImVec2 textPosition = ImVec2(dataESP::headScreenPos.x - textWidht / 2.0f, dataESP::headScreenPos.y - 15.0f);
		ImU32 color = ImColor(esp::color::name[0], esp::color::name[1], esp::color::name[2]);

		ImGui::GetBackgroundDrawList()->AddText(
			textPosition,
			color,
			dataESP::targetPlayer->name
		);
	}

	void drawBox()
	{
		float height = abs(dataESP::headScreenPos.y - dataESP::footScreenPos.y);
		float width = height / 2.0f;

		ImVec2 topLeft = ImVec2(dataESP::headScreenPos.x - width / 2, dataESP::headScreenPos.y);
		ImVec2 bottomRight = ImVec2(dataESP::footScreenPos.x + width / 2, dataESP::footScreenPos.y);

		ImU32 color = ImColor(esp::color::box[0], esp::color::box[1], esp::color::box[2]);

		ImGui::GetBackgroundDrawList()->AddRect(
			topLeft,
			bottomRight,
			color
		);
	}

	void drawHealth()
	{
		float height = abs(dataESP::headScreenPos.y - dataESP::footScreenPos.y);
		float width = height / 2.0f;
		float maxHealth = 100.0f;

		float healthBarHeight = height * (dataESP::targetPlayer->health / maxHealth);
		float healthBarWidth = 2.0f;

		ImVec2 topLeft = ImVec2(dataESP::headScreenPos.x - width / 2 - healthBarWidth - 2, dataESP::footScreenPos.y - healthBarHeight);
		ImVec2 bottomRight = ImVec2(dataESP::headScreenPos.x - width / 2 - healthBarWidth, dataESP::footScreenPos.y);

		ImVec2 topLeftBackground = ImVec2(dataESP::headScreenPos.x - width / 2 - healthBarWidth - 2, dataESP::headScreenPos.y);
		ImVec2 bottomRightBackground = ImVec2(dataESP::headScreenPos.x - width / 2 - healthBarWidth, dataESP::footScreenPos.y);

		ImU32 backgroundColor = ImColor(0.5f, 0.5f, 0.5f, 0.5f);

		float healthPercentage = dataESP::targetPlayer->health / maxHealth;

		ImU32 healthColor;
		getHealthColor(healthPercentage, healthColor);

		ImGui::GetBackgroundDrawList()->AddRectFilled(
			topLeftBackground,
			bottomRightBackground,
			backgroundColor
		);

		ImGui::GetBackgroundDrawList()->AddRectFilled(
			topLeft,
			bottomRight,
			healthColor
		);
	}

	void drawArmor()
	{
		float height = abs(dataESP::headScreenPos.y - dataESP::footScreenPos.y);
		float width = height / 2.0f;
		float maxArmor = 100.0f;

		float armorBarHeight = height * (dataESP::targetPlayer->armor / maxArmor);
		float armorBarWidth = 2.0f;

		ImVec2 topLeft = ImVec2(dataESP::headScreenPos.x - width / 2 - 2 * armorBarWidth - 4, dataESP::footScreenPos.y - armorBarHeight);
		ImVec2 bottomRight = ImVec2(dataESP::headScreenPos.x - width / 2 - armorBarWidth - 4, dataESP::footScreenPos.y);
		
		ImVec2 topLeftBackground = ImVec2(dataESP::headScreenPos.x - width / 2 - 2 * armorBarWidth - 4, dataESP::headScreenPos.y);
		ImVec2 bottomRightBackground = ImVec2(dataESP::headScreenPos.x - width / 2 - armorBarWidth - 4, dataESP::footScreenPos.y);

		ImU32 backgroundColor = ImColor(0.5f, 0.5f, 0.5f, 0.5f);
		ImU32 color = ImColor(0.0f, 0.0f, 1.0f);

		ImGui::GetBackgroundDrawList()->AddRectFilled(
			topLeftBackground,
			bottomRightBackground,
			backgroundColor
		);

		ImGui::GetBackgroundDrawList()->AddRectFilled(
			topLeft,
			bottomRight,
			color
		);
	}

	void drawDistance()
	{
		double distance = sqrt(dataESP::targetPlayer->headPos.squaredDistance(data::game::localPlayer->headPos));

		char distanceText[16];
		sprintf_s(distanceText, sizeof(distanceText), "%dm", static_cast<int>(distance));

		float textWidth = ImGui::CalcTextSize(distanceText).x;
		ImVec2 textPos = ImVec2(
			dataESP::headScreenPos.x - textWidth / 2.0f,
			dataESP::footScreenPos.y + 1.0f
		);
		ImU32 color = ImColor(esp::color::distance[0], esp::color::distance[1], esp::color::distance[2]);

		// Draw the distance text
		ImGui::GetBackgroundDrawList()->AddText(
			textPos,
			color,
			distanceText
		);
	}

	void drawFOV()
	{
		if (setting::fovType == 0)
		{
			ImGui::GetBackgroundDrawList()->AddCircle(
				ImVec2(data::widthGame / 2.0f, data::heightGame / 2.0f),
				20.0f, // change this later after implementing aimbot fov (that 20.0f value)
				static_cast<ImU32>(ImColor(esp::color::fov[0], esp::color::fov[1], esp::color::fov[2])),
				NULL,
				esp::setting::fovThickness
			);
		}
		else
		{
			ImGui::GetBackgroundDrawList()->AddRect(
				ImVec2(data::widthGame / 2.0f - 20.0f, data::heightGame / 2.0f - 20.0f), // change this later after implementing aimbot fov (that 20 value)
				ImVec2(data::widthGame / 2.0f + 20.0f, data::heightGame / 2.0f + 20.0f), // change this later after implementing aimbot fov (that 20 value)
				static_cast<ImU32>(ImColor(esp::color::fov[0], esp::color::fov[1], esp::color::fov[2])),
				esp::setting::fovRounding
			);
		}
	}

	void getHealthColor(float healthPercentage, ImU32& outputColor)
	{
		float r = 1.0f;
		float g = 1.0f;

		if (healthPercentage > 0.5f)
		{
			g = 1.0f;
			r = 2.0f * (1.0f - healthPercentage);
		}
		else
		{
			r = 1.0f;
			g = 2.0f * healthPercentage;
		}

		outputColor = ImColor(r, g, 0.0f);
	}
}
