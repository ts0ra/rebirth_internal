#define _USE_MATH_DEFINES

#include "utils.h"
#include "enums.h"
#include "data.h"

#include <cmath>
#include <algorithm>
#include <iostream>

namespace utils
{
    bool worldToScreen(const Vector3& pos, Vector2& screen, const int& width, const int& height)
    {
        Vector4 clipCoords{};
        data::game::mvpmatrix->transform(pos, clipCoords);

        if (clipCoords.w < 0.1f) return false;

        Vector3 NDC{};
        clipCoords.toNDC(NDC);

        screen.x = static_cast<int>((width / 2.0f * NDC.x) + (NDC.x + width / 2.0f));
        screen.y = static_cast<int>(-(height / 2.0f * NDC.y) + (NDC.y + height / 2.0f));

        return true;
    }

    bool isTeamGameMode(int gameMode)
    {
        if (gameMode == gameModes::TEAMDEATHMATCH ||
            gameMode == gameModes::TEAMSURVIVOR ||
            gameMode == gameModes::CTF ||
            gameMode == gameModes::BOTTEAMDEATHMATCH ||
            gameMode == gameModes::TEAMONESHOTONEKILL ||
            gameMode == gameModes::HUNTTHEFLAG ||
            gameMode == gameModes::TEAMKEEPTHEFLAG ||
            gameMode == gameModes::TEAMPF ||
            gameMode == gameModes::TEAMLSS ||
            gameMode == gameModes::BOTTEAMSURVIVOR ||
            gameMode == gameModes::BOTTEAMONESHOTONKILL
            )
        {
            return true;
        }
        return false;
    }

    float calculateVerticalFOV(float fovX, int screenWidth, int screenHeight)
    {
        float aspectRatio = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);
        float fovXRad = fovX * (M_PI / 180.0f); // Convert to radians

        // Calculate the vertical FOV using the aspect ratio
        float fovYRad = 2.0f * atan(tan(fovXRad / 2.0f) / aspectRatio);
        float fovY = fovYRad * (180.0f / M_PI); // Convert back to degrees

        return fovY;
    }

    float calculateFOVCircleDiameter(float fovX, float fovY, int screenWidth, int screenHeight)
    {
        // Convert FOVs from degrees to radians
        float fovXRad = fovX * (M_PI / 180.0f);
        float fovYRad = fovY * (M_PI / 180.0f);

        // Calculate aspect ratio
        float aspectRatio = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);

        // Calculate the radius of the circle based on the vertical FOV
        float radiusY = (screenHeight / 2.0f) * tan(fovYRad / 2.0f);

        // Calculate the radius of the circle based on the horizontal FOV
        float radiusX = (screenWidth / 2.0f) * tan(fovXRad / 2.0f);

        // Calculate the diameters
        float diameterY = 2.0f * radiusY;
        float diameterX = 2.0f * radiusX;

        // Use the larger of the two diameters for the FOV circle
        float fovDiameter = max(diameterX, diameterY);

        return fovDiameter;
    }

    Vector3 calculateAngle(const Vector3& source, const Vector3& destination)
    {
        Vector3 delta = destination - source;
        float hypotenuse = sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
        Vector3 angles;
        angles.x = atan2f(delta.y, delta.x) * (180.0f / M_PI);
        angles.y = atan2f(delta.z, hypotenuse) * (180.0f / M_PI);
        angles.z = 0.0f;

        angles.x += 90; // need to add this otherwise viewangle will be offside 90 degree

        if (angles.x < 0.0f)
        {
            angles.x += 360.0f; // normalized because if it's negative, it will be off by 360 degree (this fix flicker issue)
        }

        return angles;
    }

    float getAngleDifference(float angle1, float angle2)
    {
        float diff = angle1 - angle2;
        while (diff > 180.0f) diff -= 360.0f;
        while (diff < -180.0f) diff += 360.0f;
        return fabs(diff);
    }

    /*bool isTargetWithinFOV(const Vector3& ourViewAngles, const Vector3& ourTargetViewAngles, float fov)
    {
        float yawDifference = getAngleDifference(ourViewAngles.y, ourTargetViewAngles.y);
        float pitchDifference = getAngleDifference(ourViewAngles.x, ourTargetViewAngles.x);

        return (yawDifference <= fov && pitchDifference <= fov);
    }*/

    bool isEnemyWithinFOV(const Vector3& enemyPos, float fov)
    {
        Vector2 screenCenter = Vector2(data::widthGame / 2, data::heightGame / 2);
		Vector2 enemyScreenPos;
        worldToScreen(enemyPos, enemyScreenPos, data::widthGame, data::heightGame);
        float deltaX = enemyScreenPos.x - screenCenter.x;
        float deltaY = enemyScreenPos.y - screenCenter.y;
        float distance = sqrt(deltaX * deltaX + deltaY * deltaY);
        return distance <= (fov / 2.0f);
    }

    Vector3 smoothAim(const Vector3& currentViewAngles, const Vector3& targetViewAngles, float smoothingFactor)
    {
        Vector3 smoothedAngles;
        smoothedAngles.x = currentViewAngles.x + (targetViewAngles.x - currentViewAngles.x) / smoothingFactor;
        smoothedAngles.y = currentViewAngles.y + (targetViewAngles.y - currentViewAngles.y) / smoothingFactor;
        smoothedAngles.z = 0.0f;
        return smoothedAngles;
    }

    Vector3 anglesToDirection(float yaw, float pitch)
    {
        // Convert yaw and pitch from degrees to radians
        float yawRad = yaw * (M_PI / 180.0f);
        float pitchRad = pitch * (M_PI / 180.0f);

        // Calculate direction vector components
        float x = cos(pitchRad) * cos(yawRad);
        float y = sin(pitchRad);
        float z = cos(pitchRad) * sin(yawRad);

        return { x, y, z };
    }
}