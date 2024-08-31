#include "utils.h"
#include "enums.h"
#include "data.h"

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
}