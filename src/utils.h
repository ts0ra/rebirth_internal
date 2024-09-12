#pragma once

#include "types.h"

namespace utils
{
    bool worldToScreen(const Vector3& pos, Vector2& screen, const int& width, const int& height);
    bool isTeamGameMode(int gameMode);
    float calculateVerticalFOV(float fovX, int screenWidth, int screenHeight);
    float calculateFOVCircleDiameter(float fovX, float fovY, int screenWidth, int screenHeight);
    Vector3 calculateAngle(const Vector3& source, const Vector3& destination);
    float getAngleDifference(float angle1, float angle2);
    //bool isTargetWithinFOV(const Vector3& ourViewAngles, const Vector3& ourTargetViewAngles, float fov);
    bool isEnemyWithinFOV(const Vector3& enemyPos, float fov);
    Vector3 smoothAim(const Vector3& currentViewAngles, const Vector3& targetViewAngles, float smoothingFactor);
    Vector3 anglesToDirection(float yaw, float pitch);
}