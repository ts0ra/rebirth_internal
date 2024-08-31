void esp::drawBox(playerEnt* player)
{
	/*float height = abs(headScreenPos.y - footScreenPos.y);
	float width = height / 2.0f;

	ImVec2 topLeft = ImVec2(headScreenPos.x - width / 2, headScreenPos.y);
	ImVec2 bottomRight = ImVec2(footScreenPos.x + width / 2, footScreenPos.y);

	ImGui::GetBackgroundDrawList()->AddRect(
		topLeft,
		bottomRight,
		static_cast<ImU32>(ImColor(esp::color::box[0], esp::color::box[1], esp::color::box[2]))
	);*/

	//Vector3 newHeadPos = player->headPos + Vector3{ 0.0f, 0.0f, 0.75f };
	//Vector3 newFootPos = player->footPos - Vector3{ 0.0f, 0.0f, 0.25f };
	//// Calculate the height based on headPos and footPos
	//float height = abs(newHeadPos.z - newFootPos.z);

	//// Define a width factor to make the box wider
	//float widthFactor = 2.0f; // Adjust this value to make the box wider or narrower

	//// Define the 3D bounding box corners relative to the player's position
	//Vector3 boxCorners[8] = {
	//	newFootPos + Vector3{-0.5f * widthFactor, -0.5f * widthFactor, 0.0f}, // Bottom-front-left
	//	newFootPos + Vector3{0.5f * widthFactor, -0.5f * widthFactor, 0.0f},  // Bottom-front-right
	//	newFootPos + Vector3{0.5f * widthFactor, 0.5f * widthFactor, 0.0f},   // Bottom-back-right
	//	newFootPos + Vector3{-0.5f * widthFactor, 0.5f * widthFactor, 0.0f},  // Bottom-back-left
	//	newFootPos + Vector3{-0.5f * widthFactor, -0.5f * widthFactor, height}, // Top-front-left
	//	newFootPos + Vector3{0.5f * widthFactor, -0.5f * widthFactor, height},  // Top-front-right
	//	newFootPos + Vector3{0.5f * widthFactor, 0.5f * widthFactor, height},   // Top-back-right
	//	newFootPos + Vector3{-0.5f * widthFactor, 0.5f * widthFactor, height}   // Top-back-left
	//};

	//// Project the 3D points to 2D screen coordinates
	//ImVec2 screenCorners[8];
	//for (int i = 0; i < 8; ++i)
	//{
	//	if (!worldToScreen(boxCorners[i], screenCorners[i], data::game::mvpmatrix, data::widthGame, data::heightGame))
	//	{
	//		return; // If any point is off-screen, skip drawing the box
	//	}
	//}

	//// Draw the 3D box using ImGui
	//ImU32 boxColor = ImColor(esp::color::box[0], esp::color::box[1], esp::color::box[2]);
	//ImDrawList* drawList = ImGui::GetBackgroundDrawList();

	//// Draw bottom rectangle
	//drawList->AddLine(screenCorners[0], screenCorners[1], boxColor);
	//drawList->AddLine(screenCorners[1], screenCorners[2], boxColor);
	//drawList->AddLine(screenCorners[2], screenCorners[3], boxColor);
	//drawList->AddLine(screenCorners[3], screenCorners[0], boxColor);

	//// Draw top rectangle
	//drawList->AddLine(screenCorners[4], screenCorners[5], boxColor);
	//drawList->AddLine(screenCorners[5], screenCorners[6], boxColor);
	//drawList->AddLine(screenCorners[6], screenCorners[7], boxColor);
	//drawList->AddLine(screenCorners[7], screenCorners[4], boxColor);

	//// Draw vertical lines connecting top and bottom rectangles
	//drawList->AddLine(screenCorners[0], screenCorners[4], boxColor);
	//drawList->AddLine(screenCorners[1], screenCorners[5], boxColor);
	//drawList->AddLine(screenCorners[2], screenCorners[6], boxColor);
	//drawList->AddLine(screenCorners[3], screenCorners[7], boxColor);
}

//inline bool worldToScreen(const Vector3& pos, Vector2& screen, const float* matrix, const int width, const int height)
//{
//    Vector4 clipCoords{};
//	clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
//	clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
//	clipCoords.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
//	clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];
//
//	if (clipCoords.w < 0.1f)
//		return false;
//
//    Vector3 NDC{};
//	NDC.x = clipCoords.x / clipCoords.w;
//	NDC.y = clipCoords.y / clipCoords.w;
//	NDC.z = clipCoords.z / clipCoords.w;
//
//    screen.x = (width / 2.0f * NDC.x) + (NDC.x + width / 2.0f);
//    screen.y = -(height / 2.0f * NDC.y) + (NDC.y + height / 2.0f);
//
//	return true;
//}
//
//inline bool worldToScreen(const Vector3& pos, ImVec2& screen, const float* matrix, const int width, const int height)
//{
//    Vector4 clipCoords{};
//    clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
//    clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
//    clipCoords.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
//    clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];
//
//    if (clipCoords.w < 0.1f)
//        return false;
//
//    Vector3 NDC{};
//    NDC.x = clipCoords.x / clipCoords.w;
//    NDC.y = clipCoords.y / clipCoords.w;
//    NDC.z = clipCoords.z / clipCoords.w;
//
//    screen.x = (width / 2.0f * NDC.x) + (NDC.x + width / 2.0f);
//    screen.y = -(height / 2.0f * NDC.y) + (NDC.y + height / 2.0f);
//
//    return true;
//}
//
//inline bool worldToScreen(const Vector3& pos, Vector2& screen, const glmatrixf& matrix, const int width, const int height)
//{
//    Vector4 clipCoords{};
//    clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
//    clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
//    clipCoords.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
//    clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];
//
//    if (clipCoords.w < 0.1f)
//        return false;
//
//    Vector3 NDC{};
//    NDC.x = clipCoords.x / clipCoords.w;
//    NDC.y = clipCoords.y / clipCoords.w;
//    NDC.z = clipCoords.z / clipCoords.w;
//
//    screen.x = (width / 2.0f * NDC.x) + (NDC.x + width / 2.0f);
//    screen.y = -(height / 2.0f * NDC.y) + (NDC.y + height / 2.0f);
//
//    return true;
//}
//
//inline bool worldToScreen(const Vector3& pos, ImVec2& screen, const glmatrixf& matrix, const int width, const int height)
//{
//    Vector4 clipCoords{};
//    clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
//    clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
//    clipCoords.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
//    clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];
//
//    if (clipCoords.w < 0.1f)
//        return false;
//
//    Vector3 NDC{};
//    NDC.x = clipCoords.x / clipCoords.w;
//    NDC.y = clipCoords.y / clipCoords.w;
//    NDC.z = clipCoords.z / clipCoords.w;
//
//    screen.x = (width / 2.0f * NDC.x) + (NDC.x + width / 2.0f);
//    screen.y = -(height / 2.0f * NDC.y) + (NDC.y + height / 2.0f);
//
//    return true;
//}

#pragma once

#include <array>
#include <cmath>
#include <cstring>
#include <cassert>

// Loop macros have been removed in favor of modern C++ range-based for loops or standard algorithms.

// Forward declarations
struct vec;
struct vec4;
struct quat;

struct Entity
{
    int16_t x; //0x0000
    int16_t y; //0x0002
    int16_t z; //0x0004
    int16_t attr1; //0x0006
    int8_t type; //0x0008
    int8_t attr2; //0x0009
    int8_t attr3; //0x000A
    int8_t attr4; //0x000B
    int16_t attr5; //0x000C
    int8_t attr6; //0x000E
    int8_t attr7; //0x000F
    bool spawned; //0x0010
    char pad[3]; //0x0011 / code cave
    int32_t lastMillis; //0x0014
}; // Size: 0x0018

struct PlayerEnt
{
    char pad_0000[4]; //0x0000
    vec headPos; //0x0004
    char pad_0010[24]; //0x0010
    vec footPos; //0x0028
    vec viewAngles; //0x0034
    char pad_0040[172]; //0x0040
    int32_t health; //0x00EC
    int32_t armor; //0x00F0
    char pad_00F4[273]; //0x00F4
    char username[16]; //0x0205
    char pad_0215[247]; //0x0215
    int32_t teamSide; //0x030C
    char pad_0310[8]; //0x0310
    bool isDead; //0x0318
    char pad_0319[75]; //0x0319
    currentWpnObj* ptrToCurrentWeaponObj; //0x0364
    char pad_0368[3440]; //0x0368
}; // Size: 0x10D8

struct CurrentWpnObj
{
    char pad_0000[4]; //0x0000
    int32_t gunId; //0x0004
    char pad_0008[4]; //0x0008
    weaponStruct* ptrToCurrentWeaponStruct; //0x000C
    int32_t* ptrToReserveAmmo; //0x0010
    int32_t* ptrToLoadedAmmo; //0x0014
    int32_t* ptrToWeaponDelay; //0x0018
    int32_t shotCount; //0x001C
    char pad_0020[320]; //0x0020
}; // Size: 0x0160

struct Weapon
{
    char modelName[23]; //0x0000
    char weaponName[42]; //0x0017
    int16_t shootFX; //0x0042
    int16_t reloadFX; //0x0044
    int16_t reloadTime; //0x0046
    int16_t shootDelay; //0x0048
    int16_t damage; //0x004A
    int16_t piercing; //0x004C
    int16_t projectileSpeed; //0x004E
    int16_t part; //0x0050
    int16_t spread; //0x0052
    int16_t recoil; //0x0054
    int16_t magsize; //0x0056
    int16_t mdlKickRot; //0x0058
    int16_t mdlKickBack; //0x005A
    int16_t recoilIncrease; //0x005C
    int16_t recoilBase; //0x005E
    int16_t maxRecoil; //0x0060
    int16_t recoilBackfade; //0x0062
    int16_t pushFactor; //0x0064
    bool isAuto; //0x0066
}; // Size: 0x0067

struct PlayerListPtr
{
    std::array<PlayerEnt*, 33> players;
};

struct GLMatrixf
{
    std::array<float, 16> v;

    constexpr float operator[](std::size_t i) const { return v[i]; }
    float& operator[](std::size_t i) { return v[i]; }

    void rotateAroundX(float angle)
    {
        float c = std::cos(angle);
        float s = std::sin(angle);
        rotateVector(v[4], v[8], c, s);
        rotateVector(v[5], v[9], c, s);
        rotateVector(v[6], v[10], c, s);
    }

    void rotateAroundY(float angle)
    {
        float c = std::cos(angle);
        float s = std::sin(angle);
        rotateVector(v[8], v[0], c, s);
        rotateVector(v[9], v[1], c, s);
        rotateVector(v[10], v[2], c, s);
    }

    void rotateAroundZ(float angle)
    {
        float c = std::cos(angle);
        float s = std::sin(angle);
        rotateVector(v[0], v[4], c, s);
        rotateVector(v[1], v[5], c, s);
        rotateVector(v[2], v[6], c, s);
    }

    void multiply(const std::array<float, 16>& x, const std::array<float, 16>& y)
    {
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                v[j + i * 4] = x[i] * y[j] +
                    x[i + 4] * y[j + 4] +
                    x[i + 8] * y[j + 8] +
                    x[i + 12] * y[j + 12];
            }
        }
    }

    void multiply(const GLMatrixf& x, const GLMatrixf& y)
    {
        multiply(x.v, y.v);
    }

    void identity()
    {
        v = { 1, 0, 0, 0,
              0, 1, 0, 0,
              0, 0, 1, 0,
              0, 0, 0, 1 };
    }

    void translate(float x, float y, float z)
    {
        v[12] += x;
        v[13] += y;
        v[14] += z;
    }

    void translate(const vec& o);

    void scale(float x, float y, float z)
    {
        for (int i = 0; i < 4; ++i)
        {
            v[i] *= x;
            v[i + 4] *= y;
            v[i + 8] *= z;
        }
    }

    void projective()
    {
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                v[i + j * 4] = 0.5f * (v[i + j * 4] + v[3 + j * 4]);
            }
        }
    }

    void invertNormal(vec& dir) const;
    void invertVertex(vec& pos) const;

    float transformX(const vec& p) const { return p.x * v[0] + p.y * v[4] + p.z * v[8] + v[12]; }
    float transformY(const vec& p) const { return p.x * v[1] + p.y * v[5] + p.z * v[9] + v[13]; }
    float transformZ(const vec& p) const { return p.x * v[2] + p.y * v[6] + p.z * v[10] + v[14]; }
    float transformW(const vec& p) const { return p.x * v[3] + p.y * v[7] + p.z * v[11] + v[15]; }

    void transform(const vec& in, vec4& out) const
    {
        out.x = transformX(in);
        out.y = transformY(in);
        out.z = transformZ(in);
        out.w = transformW(in);
    }

    vec getTranslation() const
    {
        return vec{ v[12], v[13], v[14] };
    }

    float determinant() const
    {
        float a1 = v[0], a2 = v[1], a3 = v[2], a4 = v[3],
            b1 = v[4], b2 = v[5], b3 = v[6], b4 = v[7],
            c1 = v[8], c2 = v[9], c3 = v[10], c4 = v[11],
            d1 = v[12], d2 = v[13], d3 = v[14], d4 = v[15];

        return a1 * utils::det3x3(b2, b3, b4, c2, c3, c4, d2, d3, d4)
            - b1 * utils::det3x3(a2, a3, a4, c2, c3, c4, d2, d3, d4)
            + c1 * utils::det3x3(a2, a3, a4, b2, b3, b4, d2, d3, d4)
            - d1 * utils::det3x3(a2, a3, a4, b2, b3, b4, c2, c3, c4);
    }

    bool invert(const GLMatrixf& m, float minDet = 1.0e-10f)
    {
        float a1 = m.v[0], b1 = m.v[4], c1 = m.v[8], d1 = m.v[12];
        adjoint(m);
        float det = a1 * v[0] + b1 * v[1] + c1 * v[2] + d1 * v[3];
        if (std::fabs(det) < minDet) return false;
        float invDet = 1.0f / det;
        for (auto& elem : v) elem *= invDet;
        return true;
    }

    void fromQuat(const quat& q)
    {
        float xx = q.x * q.x, xy = q.x * q.y, xz = q.x * q.z, xw = q.x * q.w,
            yy = q.y * q.y, yz = q.y * q.z, yw = q.y * q.w,
            zz = q.z * q.z, zw = q.z * q.w;

        v[0] = 1 - 2 * (yy + zz);
        v[1] = 2 * (xy - zw);
        v[2] = 2 * (xz + yw);

        v[4] = 2 * (xy + zw);
        v[5] = 1 - 2 * (xx + zz);
        v[6] = 2 * (yz - xw);

        v[8] = 2 * (xz - yw);
        v[9] = 2 * (yz + xw);
        v[10] = 1 - 2 * (xx + yy);

        v[3] = v[7] = v[11] = 0;
        v[12] = v[13] = v[14] = 0;
        v[15] = 1;
    }

private:
    static void rotateVector(float& a, float& b, float c, float s)
    {
        float tempA = a;
        a = tempA * c + b * s;
        b = b * c - tempA * s;
    }

    void adjoint(const GLMatrixf& m);
};

//////////////////////////

To refactor and improve the provided code for better readability, maintainability, and adherence to modern C++ standards(C++17 or C++20), I'll focus on several key areas:

1. * *Macro Definitions * *: Replace macros with inline functions or constexpr functions to avoid issues related to macro substitution and scope.

2. * *Struct Alignment * *: Use `alignas` to ensure correct alignment for structs that may be involved in direct memory access or casting.

3. * *Initialization * *: Use `std: : array` for fixed - size arrays to provide better type safety and ease of use.

4. * *Code Simplification * *: Simplify matrix operations and improve readability.

Here is the refactored code :

```cpp
#pragma once

#include <array>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <utility> // for std::exchange

// Constants
constexpr int kMaxUsernameLength = 16;
constexpr int kMaxWeaponNameLength = 42;
constexpr int kMaxModelNameLength = 23;

// Utility functions
inline constexpr float degToRad(float degrees) {
    return degrees * static_cast<float>(M_PI) / 180.0f;
}

// Struct Definitions
struct alignas(16) Entity {
    int16_t x; // 0x0000
    int16_t y; // 0x0002
    int16_t z; // 0x0004
    int16_t attr1; // 0x0006
    int8_t type; // 0x0008
    int8_t attr2; // 0x0009
    int8_t attr3; // 0x000A
    int8_t attr4; // 0x000B
    int16_t attr5; // 0x000C
    int8_t attr6; // 0x000E
    int8_t attr7; // 0x000F
    bool spawned; // 0x0010
    char pad[3]; // 0x0011
    int32_t lastmillis; // 0x0014
};

struct alignas(16) PlayerEnt {
    char pad_0000[4]; // 0x0000
    vec headPos; // 0x0004
    char pad_0010[24]; // 0x0010
    vec footPos; // 0x0028
    vec viewAngles; // 0x0034
    char pad_0040[172]; // 0x0040
    int32_t health; // 0x00EC
    int32_t armor; // 0x00F0
    char pad_00F4[273]; // 0x00F4
    char username[kMaxUsernameLength]; // 0x0205
    char pad_0215[247]; // 0x0215
    int32_t teamSide; // 0x030C
    char pad_0310[8]; // 0x0310
    bool isDead; // 0x0318
    char pad_0319[75]; // 0x0319
    struct CurrentWpnObj* ptrToCurrentWeaponObj; // 0x0364
    char pad_0368[3440]; // 0x0368
};

struct alignas(16) CurrentWpnObj {
    char pad_0000[4]; // 0x0000
    int32_t gunId; // 0x0004
    char pad_0008[4]; // 0x0008
    struct WeaponStruct* ptrToCurrentWeaponStruct; // 0x000C
    int32_t* ptrToReserveAmmo; // 0x0010
    int32_t* ptrToLoadedAmmo; // 0x0014
    int32_t* ptrToWeaponDelay; // 0x0018
    int32_t shotCount; // 0x001C
    char pad_0020[320]; // 0x0020
};

struct alignas(16) Weapon {
    char modelName[kMaxModelNameLength]; // 0x0000
    char weaponName[kMaxWeaponNameLength]; // 0x0017
    char pad_0041[1]; // 0x0041
    int16_t shootFX; // 0x0042
    int16_t reloadFX; // 0x0044
    int16_t reloadTime; // 0x0046
    int16_t shootDelay; // 0x0048
    int16_t damage; // 0x004A
    int16_t piercing; // 0x004C
    int16_t projectileSpeed; // 0x004E
    int16_t part; // 0x0050
    int16_t spread; // 0x0052
    int16_t recoil; // 0x0054
    int16_t magsize; // 0x0056
    int16_t mdl_kick_rot; // 0x0058
    int16_t mdl_kick_back; // 0x005A
    int16_t recoilIncrease; // 0x005C
    int16_t recoilBase; // 0x005E
    int16_t maxRecoil; // 0x0060
    int16_t recoilBackfade; // 0x0062
    int16_t pushFactor; // 0x0064
    bool isAuto; // 0x0066
};

struct alignas(16) PlayerListPtr {
    PlayerEnt* player[32];
};

struct alignas(16) GlMatrixf {
    std::array<float, 16> v;

    float operator[](size_t i) const { return v[i]; }
    float& operator[](size_t i) { return v[i]; }

    void rotateAroundX(float angle) {
        float c = std::cos(angle), s = std::sin(angle);
        for (int i = 0; i < 3; ++i) {
            std::swap(v[4 + i * 4], v[8 + i * 4]);
            v[4 + i * 4] = c * v[4 + i * 4] - s * v[8 + i * 4];
            v[8 + i * 4] = s * v[4 + i * 4] + c * v[8 + i * 4];
        }
    }

    void rotateAroundY(float angle) {
        float c = std::cos(angle), s = std::sin(angle);
        for (int i = 0; i < 3; ++i) {
            std::swap(v[i * 4], v[i * 4 + 2]);
            v[i * 4] = c * v[i * 4] + s * v[i * 4 + 2];
            v[i * 4 + 2] = -s * v[i * 4] + c * v[i * 4 + 2];
        }
    }

    void rotateAroundZ(float angle) {
        float c = std::cos(angle), s = std::sin(angle);
        for (int i = 0; i < 3; ++i) {
            std::swap(v[i + 4], v[i + 8]);
            v[i + 4] = c * v[i + 4] - s * v[i + 8];
            v[i + 8] = s * v[i + 4] + c * v[i + 8];
        }
    }

    template<typename XT, typename YT>
    void multiply(const XT x[16], const YT y[16]) {
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                v[col + row * 4] = 0;
                for (int k = 0; k < 4; ++k) {
                    v[col + row * 4] += x[row * 4 + k] * y[k * 4 + col];
                }
            }
        }
    }

    void multiply(const GlMatrixf& x, const GlMatrixf& y) {
        multiply(x.v.data(), y.v.data());
    }

    void identity() {
        v.fill(0.0f);
        v[0] = v[5] = v[10] = v[15] = 1.0f;
    }

    void translate(float x, float y, float z) {
        v[12] += x;
        v[13] += y;
        v[14] += z;
    }

    void translate(const vec& o) {
        translate(o.x, o.y, o.z);
    }

    void scale(float x, float y, float z) {
        for (int i = 0; i < 4; ++i) {
            v[i] *= x;
            v[4 + i] *= y;
            v[8 + i] *= z;
        }
    }

    void projective() {
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 4; ++j) {
                v[i + j * 4] = 0.5f * (v[i + j * 4] + v[3 + j * 4]);
            }
        }
    }

    void invertNormal(vec&

        dir) const {
        vec n = dir;
        dir.x = n.x * v[0] + n.y * v[1] + n.z * v[2];
        dir.y = n.x * v[4] + n.y * v[5] + n.z * v[6];
        dir.z = n.x * v[8] + n.y * v[9] + n.z * v[10];
    }

    void invertVertex(vec& pos) const {
        vec p = pos;
        p.x -= v[12];
        p.y -= v[13];
        p.z -= v[14];
        pos.x = p.x * v[0] + p.y * v[1] + p.z * v[2];
        pos.y = p.x * v[4] + p.y * v[5] + p.z * v[6];
        pos.z = p.x * v[8] + p.y * v[9] + p.z * v[10];
    }

    float transformX(const vec& p) const {
        return p.x * v[0] + p.y * v[4] + p.z * v[8] + v[12];
    }

    float transformY(const vec& p) const {
        return p.x * v[1] + p.y * v[5] + p.z * v[9] + v[13];
    }

    float transformZ(const vec& p) const {
        return p.x * v[2] + p.y * v[6] + p.z * v[10] + v[14];
    }

    float transformW(const vec& p) const {
        return p.x * v[3] + p.y * v[7] + p.z * v[11] + v[15];
    }

    void transform(const vec& in, vec4& out) const {
        out.x = transformX(in);
        out.y = transformY(in);
        out.z = transformZ(in);
        out.w = transformW(in);
    }

    vec getTranslation() const {
        return vec(v[12], v[13], v[14]);
    }

    float determinant() const {
        auto det3x3 = [](float a1, float a2, float a3, float b1, float b2, float b3, float c1, float c2, float c3) {
            return a1 * (b2 * c3 - b3 * c2) - a2 * (b1 * c3 - b3 * c1) + a3 * (b1 * c2 - b2 * c1);
            };

        float a1 = v[0], a2 = v[1], a3 = v[2], a4 = v[3];
        float b1 = v[4], b2 = v[5], b3 = v[6], b4 = v[7];
        float c1 = v[8], c2 = v[9], c3 = v[10], c4 = v[11];
        float d1 = v[12], d2 = v[13], d3 = v[14], d4 = v[15];

        return a1 * det3x3(b2, b3, b4, c2, c3, c4, d2, d3, d4)
            - b1 * det3x3(a2, a3, a4, c2, c3, c4, d2, d3, d4)
            + c1 * det3x3(a2, a3, a4, b2, b3, b4, d2, d3, d4)
            - d1 * det3x3(a2, a3, a4, b2, b3, b4, c2, c3, c4);
    }

    void adjoint(const GlMatrixf& m) {
        auto det3x3 = [](float a1, float a2, float a3, float b1, float b2, float b3, float c1, float c2, float c3) {
            return a1 * (b2 * c3 - b3 * c2) - a2 * (b1 * c3 - b3 * c1) + a3 * (b1 * c2 - b2 * c1);
            };

        float a1 = m.v[0], a2 = m.v[1], a3 = m.v[2], a4 = m.v[3];
        float b1 = m.v[4], b2 = m.v[5], b3 = m.v[6], b4 = m.v[7];
        float c1 = m.v[8], c2 = m.v[9], c3 = m.v[10], c4 = m.v[11];
        float d1 = m.v[12], d2 = m.v[13], d3 = m.v[14], d4 = m.v[15];

        v[0] = det3x3(b2, b3, b4, c2, c3, c4, d2, d3, d4);
        v[1] = -det3x3(a2, a3, a4, c2, c3, c4, d2, d3, d4);
        v[2] = det3x3(a2, a3, a4, b2, b3, b4, d2, d3, d4);
        v[3] = -det3x3(a2, a3, a4, b2, b3, b4, c2, c3, c4);

        v[4] = -det3x3(b1, b3, b4, c1, c3, c4, d1, d3, d4);
        v[5] = det3x3(a1, a3, a4, c1, c3, c4, d1, d3, d4);
        v[6] = -det3x3(a1, a3, a4, b1, b3, b4, d1, d3, d4);
        v[7] = det3x3(a1, a3, a4, b1, b3, b4, c1, c3, c4);

        v[8] = det3x3(b1, b2, b4, c1, c2, c4, d1, d2, d4);
        v[9] = -det3x3(a1, a2, a4, c1, c2, c4, d1, d2, d4);
        v[10] = det3x3(a1, a2, a4, b1, b2, b4, d1, d2, d4);
        v[11] = -det3x3(a1, a2, a4, b1, b2, b4, c1, c2, c4);

        v[12] = -det3x3(b1, b2, b3, c1, c2, c3, d1, d2, d3);
        v[13] = det3x3(a1, a2, a3, c1, c2, c3, d1, d2, d3);
        v[14] = -det3x3(a1, a2, a3, b1, b2, b3, d1, d2, d3);
        v[15] = det3x3(a1, a2, a3, b1, b2, b3, c1, c2, c3);
    }

    bool invert(const GlMatrixf& m, float mindet = 1.0e-10f) {
        float a1 = m.v[0], b1 = m.v[4], c1 = m.v[8], d1 = m.v[12];
        adjoint(m);
        float det = a1 * v[0] + b1 * v[1] + c1 * v[2] + d1 * v[3];
        if (std::fabs(det) < mindet) return false;
        float invdet = 1.0f / det;
        std::transform(v.begin(), v.end(), v.begin(), [invdet](float val) { return val * invdet; });
        return true;
    }

    void fromQuat(const quat& q) {
        float xx = q.x * q.x, xy = q.x * q.y, xz = q.x * q.z, xw = q.x * q.w;
        float yy = q.y * q.y, yz = q.y * q.z, yw = q.y * q.w;
        float zz = q.z * q.z, zw = q.z * q.w;

        v[0] = 1 - 2 * (yy + zz);
        v[1] = 2 * (xy - zw);
        v[2] = 2 * (xz + yw);

        v[4] = 2 * (xy + zw);
        v[5] = 1 - 2 * (xx + zz);
        v[6] = 2 * (yz - xw);

        v[8] = 2 * (xz - yw);
        v[9] = 2 * (yz +

            xw);
        v[10] = 1 - 2 * (xx + yy);

        std::fill(v.begin() + 3, v.begin() + 12, 0.0f);
        v[15] = 1.0f;
    }
};