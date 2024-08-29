#include "esp.h"

#include <iostream>

static Vector2 headScreenPos{}, footScreenPos{};

void esp::draw()
{
	if (esp::toggle::drawESP)
	{
		for (int i = 0; i < *data::game::totalPlayer; ++i)
		{
			playerEnt* player = data::game::playerList->player[i];
			if (player == nullptr) continue; // skip if invalid player
			if (isTeamGameMode(*data::game::gameMode) && 
				player->teamSide == data::game::localPlayer->teamSide) continue; // skip if player is on the same team
			if (player->isDead) continue; // skip if player is dead

			if (worldToScreen(player->headPos, headScreenPos, data::game::viewMatrix, data::widthGame, data::heightGame) &&
				worldToScreen(player->footPos, footScreenPos, data::game::viewMatrix, data::widthGame, data::heightGame))
			{
				if (esp::toggle::name)
					esp::drawName(player);

				if (esp::toggle::box)
					esp::drawBox(player);

				if (esp::toggle::health)
					esp::drawHealth(player);

				if (esp::toggle::armor)
					esp::drawArmor(player);

				if (esp::toggle::distance)
					esp::drawDistance(player);				
			}
		}
	}

	if (esp::toggle::drawFov)
		esp::drawFOV();
}

void esp::drawName(playerEnt* player)
{
	float textWidht = ImGui::CalcTextSize(player->username).x;
	ImGui::GetBackgroundDrawList()->AddText(
		ImVec2(headScreenPos.x - textWidht / 2.0f, headScreenPos.y - 15.0f),
		static_cast<ImU32>(ImColor(esp::color::name[0], esp::color::name[1], esp::color::name[2])),
		player->username
	);
}

void esp::drawBox(playerEnt* player)
{
	float height = abs(headScreenPos.y - footScreenPos.y);
	float width = height / 2.0f;

	ImVec2 topLeft = ImVec2(headScreenPos.x - width / 2, headScreenPos.y);
	ImVec2 bottomRight = ImVec2(footScreenPos.x + width / 2, footScreenPos.y);

	ImGui::GetBackgroundDrawList()->AddRect(
		topLeft,
		bottomRight,
		static_cast<ImU32>(ImColor(esp::color::box[0], esp::color::box[1], esp::color::box[2]))
	);
}

void esp::drawHealth(playerEnt* player)
{
	float height = abs(headScreenPos.y - footScreenPos.y);
	float width = height / 2.0f;
	float maxHealth = 100.0f;

	float healthBarHeight = height * (player->health / maxHealth);
	float healthBarWidth = 2.0f;

	ImVec2 topLeft = ImVec2(headScreenPos.x - width / 2 - healthBarWidth - 2, footScreenPos.y - healthBarHeight);
	ImVec2 bottomRight = ImVec2(headScreenPos.x - width / 2 - healthBarWidth, footScreenPos.y);

	float healthPercentage = player->health / maxHealth;

	ImU32 healthColor;
	getHealthColor(healthPercentage, healthColor);

	ImGui::GetBackgroundDrawList()->AddRectFilled(
		ImVec2(headScreenPos.x - width / 2 - healthBarWidth - 2, headScreenPos.y),
		ImVec2(headScreenPos.x - width / 2 - healthBarWidth, footScreenPos.y),
		static_cast<ImU32>(ImColor(0.5f, 0.5f, 0.5f))
	);

	ImGui::GetBackgroundDrawList()->AddRectFilled(
		topLeft,
		bottomRight,
		healthColor
	);
}

void esp::drawArmor(playerEnt* player)
{
	float height = abs(headScreenPos.y - footScreenPos.y);
	float width = height / 2.0f;
	float maxArmor = 100.0f;

	float armorBarHeight = height * (player->armor / maxArmor);
	float armorBarWidth = 2.0f;

	ImVec2 topLeft = ImVec2(headScreenPos.x - width / 2 - 2 * armorBarWidth - 4, footScreenPos.y - armorBarHeight);
	ImVec2 bottomRight = ImVec2(headScreenPos.x - width / 2 - armorBarWidth - 4, footScreenPos.y);

	ImGui::GetBackgroundDrawList()->AddRectFilled(
		ImVec2(headScreenPos.x - width / 2 - 2 * armorBarWidth - 4, headScreenPos.y),
		ImVec2(headScreenPos.x - width / 2 - armorBarWidth - 4, footScreenPos.y),
		static_cast<ImU32>(ImColor(0.5f, 0.5f, 0.5f))
	);

	ImGui::GetBackgroundDrawList()->AddRectFilled(
		topLeft,
		bottomRight,
		static_cast<ImU32>(ImColor(0.0f, 0.0f, 1.0f))
	);
}

void esp::drawDistance(playerEnt* player)
{
	double distance = sqrt(
		pow(player->headPos.x - data::game::localPlayer->headPos.x, 2) +
		pow(player->headPos.y - data::game::localPlayer->headPos.y, 2) +
		pow(player->headPos.z - data::game::localPlayer->headPos.z, 2)
	);

	char distanceText[16];
	sprintf_s(distanceText, sizeof(distanceText), "%d m", static_cast<int>(distance));

	float textWidth = ImGui::CalcTextSize(distanceText).x;

	ImVec2 textPos = ImVec2(
		headScreenPos.x - textWidth / 2.0f,
		footScreenPos.y + 1.0f
	);

	// Draw the distance text
	ImGui::GetBackgroundDrawList()->AddText(
		textPos,
		static_cast<ImU32>(ImColor(esp::color::distance[0], esp::color::distance[1], esp::color::distance[2])),
		distanceText
	);
}

void esp::drawFOV()
{
	if (esp::setting::fovType == 0)
	{
		ImGui::GetBackgroundDrawList()->AddCircle(
			ImVec2(static_cast<float>(data::widthGame) / 2.0f, static_cast<float>(data::heightGame) / 2.0f),
			20.0f, // change this later after implementing aimbot fov (that 20.0f value)
			static_cast<ImU32>(ImColor(esp::color::fov[0], esp::color::fov[1], esp::color::fov[2])),
			NULL,
			esp::setting::fovThickness
		);
	}
	else
	{
		ImGui::GetBackgroundDrawList()->AddRect(
			ImVec2(static_cast<float>(data::widthGame) / 2.0f - 20.0f, static_cast<float>(data::heightGame) / 2.0f - 20.0f), // change this later after implementing aimbot fov (that 20 value)
			ImVec2(static_cast<float>(data::widthGame) / 2.0f + 20.0f, static_cast<float>(data::heightGame) / 2.0f + 20.0f), // change this later after implementing aimbot fov (that 20 value)
			static_cast<ImU32>(ImColor(esp::color::fov[0], esp::color::fov[1], esp::color::fov[2])),
			esp::setting::fovRounding
		);
	}
}

void esp::getHealthColor(float healthPercentage, ImU32& outputColor)
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
