#pragma once

#include "types.h"

namespace utils
{
    bool worldToScreen(const Vector3& pos, Vector2& screen, const int& width, const int& height);
    bool isTeamGameMode(int gameMode);
}