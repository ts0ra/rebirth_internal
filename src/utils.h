#ifndef UTILS_REBIRTH
#define UTILS_REBIRTH

enum itemTypes                // static entity types
{
    NOTUSED = 0,                // entity slot not in use in map (usually seen at deleted entities)
    LIGHT,                      // lightsource, attr1 = radius, attr2 = intensity (or attr2..4 = r-g-b)
    PLAYERSTART,                // attr1 = angle, attr2 = team
    I_CLIPS, I_AMMO, I_GRENADE, // attr1 = elevation
    I_HEALTH, I_HELMET, I_ARMOUR, I_AKIMBO,
    MAPMODEL,                   // attr1 = angle, attr2 = idx, attr3 = elevation, attr4 = texture, attr5 = pitch, attr6 = roll
    CARROT,                     // attr1 = tag, attr2 = type
    LADDER,                     // attr1 = height
    CTF_FLAG,                   // attr1 = angle, attr2 = red/blue
    SOUND,                      // attr1 = idx, attr2 = radius, attr3 = size, attr4 = volume
    CLIP,                       // attr1 = elevation, attr2 = xradius, attr3 = yradius, attr4 = height, attr6 = slope, attr7 = shape
    PLCLIP,                     // attr1 = elevation, attr2 = xradius, attr3 = yradius, attr4 = height, attr6 = slope, attr7 = shape
    DUMMYENT,                   // temporary entity without any function - will not be saved to map files, used to mark positions and for scripting
    MAXENTTYPES
};

enum gameModes                    // game modes
{
    DEMO = -1,
    TEAMDEATHMATCH = 0,           // 0
    COOPEDIT,
    DEATHMATCH,
    SURVIVOR,
    TEAMSURVIVOR,
    CTF,                          // 5
    PISTOLFRENZY,
    BOTTEAMDEATHMATCH,
    BOTDEATHMATCH,
    LASTSWISSSTANDING,
    ONESHOTONEKILL,               // 10
    TEAMONESHOTONEKILL,
    BOTONESHOTONEKILL,
    HUNTTHEFLAG,
    TEAMKEEPTHEFLAG,
    KEEPTHEFLAG,                  // 15
    TEAMPF,
    TEAMLSS,
    BOTPISTOLFRENZY,
    BOTLSS,
    BOTTEAMSURVIVOR,              // 20
    BOTTEAMONESHOTONKILL,
    NUM
};

struct Vector2
{
    float x, y;
};

struct Vector2Int
{
    int x, y;
};

struct Vector3
{
	float x, y, z;
};

struct Vector4
{
	float x, y, z, w;
};

inline bool worldToScreen(const Vector3& pos, Vector2& screen, const float* matrix, const int width, const int height)
{
    Vector4 clipCoords{};
	clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
	clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
	clipCoords.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
	clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];

	if (clipCoords.w < 0.1f)
		return false;

    Vector3 NDC{};
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

    screen.x = (width / 2.0f * NDC.x) + (NDC.x + width / 2.0f);
    screen.y = -(height / 2.0f * NDC.y) + (NDC.y + height / 2.0f);

	return true;
}

inline bool isTeamGameMode(int gameMode)
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

#endif // !UTILS_REBIRTH
