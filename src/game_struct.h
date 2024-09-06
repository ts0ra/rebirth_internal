#pragma once

#include "types.h"

#include <array>
#include <cmath>
#include <cstring>
#include <cstdint>
#include <vector>

// Forward declaration of playerEnt
struct playerEnt;

struct entity {
    int16_t x;            // 0x0000
    int16_t y;            // 0x0002
    int16_t z;            // 0x0004
    int16_t attr1;        // 0x0006
    uint8_t type;         // 0x0008
    uint8_t attr2;        // 0x0009
    uint8_t attr3;        // 0x000A
    uint8_t attr4;        // 0x000B
    int16_t attr5;        // 0x000C
    int8_t attr6;         // 0x000E
    uint8_t attr7;        // 0x000F
    bool spawned;       // 0x0010
    char pad_0011[3];   // 0x0011 / code cave
    int32_t lastmillis;     // 0x0014
};                      // Size: 0x0018

struct gunInfo {
    char modelName[23]; // 0x0000
    char weaponName[42]; // 0x0017
    char pad[1]; // 0x0041 unused
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
}; // Size: 0x0067

struct weapon {
    char pad_0000[4]; //0x0000
    int32_t gunType; //0x0004
    struct playerEnt* ptrToPlayerEnt; //0x0008
    struct gunInfo* ptrToGunInfo; //0x000C
    int32_t* ptrToAmmo; //0x0010
    int32_t* ptrToMag; //0x0014
    int32_t* ptrToGunWait; //0x0018
    int32_t shots; //0x001C
    int32_t reloading; //0x0020
    int32_t lastAction; //0x0024
};

struct grenadeEntity
{
    char pad_0000[4]; //0x0000
    Vector3 grenadeOrigin; //0x0004
    Vector3 velocity; //0x0010
    Vector3 deltaPos; //0x001C
    Vector3 grenadeNewPos; //0x0028
    float yaw; //0x0034
    float pitch; //0x0038
    float roll; //0x003C
    float pitchVel; //0x0040
    float maxSpeed; //0x0044
    int32_t timeInAir; //0x0048
    float radius; //0x004C
    float eyeHeight; //0x0050
    float maxEyeHeight; //0x0054
    float aboveEye; //0x0058
    bool inWater; //0x005C
    bool onFloor; //0x005D
    bool onLadder; //0x005E
    bool jumpNext; //0x005F
    bool jumpD; //0x0060
    bool crouching; //0x0061
    bool crouchedInAir; //0x0062
    bool tryCrouch; //0x0063
    bool canCollide; //0x0064
    bool stuck; //0x0065
    bool scoping; //0x0066
    char pad_0067[1]; //0x0067
    int32_t lastJump; //0x0068
    float lastJumpHeight; //0x006C
    int32_t lastSplash; //0x0070
    int8_t move; //0x0074
    int8_t strafe; //0x0075
    uint8_t state; //0x0076 1 = isDead
    uint8_t type; //0x0077
    float eyeHeightVel; //0x0078
    int32_t lastPos; //0x007C
    int32_t millis; //0x0080
    int32_t timeToAlive; //0x0084
    int32_t bounceType; //0x0088
    float rotSpeed; //0x008C
    bool plClipped; //0x0090
    char pad_0091[3]; //0x0091
    struct playerEnt* ptrToOwner; //0x0094
    bool isLocal; //0x0098
    char pad_0099[3]; //0x0099
    int32_t nadeState; //0x009C
    float distanceSinceBounce; //0x00A0
}; //Size: 0x00A4

struct sniperStruct
{
    char pad_0000[4]; //0x0000
    int32_t gunType; //0x0004
    struct playerEnt* ptrToPlayerEnt; //0x0008
    struct gunInfo* ptrToGunInfo; //0x000C
    int32_t* ptrToAmmo; //0x0010
    int32_t* ptrToMag; //0x0014
    int32_t* ptrToGunWait; //0x0018
    int32_t shots; //0x001C
    int32_t reloading; //0x0020
    int32_t lastAction; //0x0024
    bool isScoped; //0x0028
    char pad_0029[3]; //0x0029
    int32_t scopedSince; //0x002C
}; //Size: 0x0030

struct akimboStruct
{
    char pad_0000[4]; //0x0000
    int32_t gunType; //0x0004
    struct playerEnt* ptrToPlayerEnt; //0x0008
    struct gunInfo* ptrToGunInfo; //0x000C
    int32_t* ptrToAmmo; //0x0010
    int32_t* ptrToMag; //0x0014
    int32_t* ptrToGunWait; //0x0018
    int32_t shots; //0x001C
    int32_t reloading; //0x0020
    int32_t lastAction; //0x0024
    int32_t akimboSide; //0x0028
    int32_t akimboMillis; //0x002C
    int32_t akimboLastActionLeft; //0x0030
    int32_t akimboLastActionRight; //0x0034
}; //Size: 0x0038

struct grenadeStruct
{
    char pad_0000[4]; //0x0000
    int32_t gunType; //0x0004
    struct playerEnt* ptrToPlayerEnt; //0x0008
    struct gunInfo* ptrToGunInfo; //0x000C
    int32_t* ptrToAmmo; //0x0010
    int32_t* ptrToMag; //0x0014
    int32_t* ptrToGunWait; //0x0018
    int32_t shots; //0x001C
    int32_t reloading; //0x0020
    int32_t lastAction; //0x0024
    struct grenadeEntity* ptrToInHandNade; //0x0028
    int32_t throwWait; //0x002C
    int32_t throwMillis; //0x0030
    int32_t cookingMillis; //0x0034
    int32_t state; //0x0038
}; //Size: 0x003C


struct playerEnt {
    char pad_0000[4]; //0x0000
    Vector3 headPos; //0x0004
    Vector3 velocity; //0x0010
    Vector3 deltaPos; //0x001C
    Vector3 footPos; //0x0028
    float yaw; //0x0034
    float pitch; //0x0038
    float roll; //0x003C
    float pitchVel; //0x0040
    float maxSpeed; //0x0044
    int32_t timeInAir; //0x0048
    float radius; //0x004C
    float eyeHeight; //0x0050
    float maxEyeHeight; //0x0054
    float aboveEye; //0x0058
    bool inWater; //0x005C
    bool onFloor; //0x005D
    bool onLadder; //0x005E
    bool jumpNext; //0x005F
    bool jumpD; //0x0060
    bool crouching; //0x0061
    bool crouchedInAir; //0x0062
    bool tryCrouch; //0x0063
    bool canCollide; //0x0064
    bool stuck; //0x0065
    bool scoping; //0x0066
    char pad_0067[1]; //0x0067
    int32_t lastJump; //0x0068
    float lastJumpHeight; //0x006C
    int32_t lastSplash; //0x0070
    int8_t move; //0x0074
    int8_t strafe; //0x0075
    uint8_t state; //0x0076
    uint8_t type; //0x0077
    float eyeHeightVel; //0x0078
    int32_t lastPos; //0x007C
    bool left; //0x0080
    bool right; //0x0081
    bool up; //0x0082
    bool down; //0x0083
    char pad_0084[104]; //0x0084
    int32_t health; //0x00EC
    int32_t armor; //0x00F0
    int32_t primary; //0x00F4
    int32_t nextPrimary; //0x00F8
    int32_t gunSelect; //0x00FC
    bool akimboMode; //0x0100
    char pad_0101[195]; //0x0101
    uint32_t clientNum; //0x01C4
    int32_t lastUpdate; //0x01C8
    int32_t plag; //0x01CC
    int32_t ping; //0x01D0
    uint32_t address; //0x01D4
    int32_t lifeSequence; //0x01D8
    int32_t frags; //0x01DC
    int32_t flagScore; //0x01E0
    int32_t deaths; //0x01E4
    int32_t tks; //0x01E8
    int32_t lastAction; //0x01EC
    int32_t lastMove; //0x01F0
    int32_t lastPain; //0x01F4
    int32_t lastVoicecom; //0x01F8
    int32_t lastDeath; //0x01FC
    int32_t clientRole; //0x0200
    bool attacking; //0x0204
    char name[260]; //0x0205
    char pad_0309[3]; //0x0309
    int32_t teamSide; //0x030C
    int32_t weaponChanging; //0x0310
    int32_t nextWeapon; //0x0314
    int32_t spectateMode; //0x0318
    int32_t followPlayerCN; //0x031C
    int32_t earDamageMillis; //0x0320
    float maxRoll; //0x0324
    float maxRollEffect; //0x0328
    float movRoll; //0x032C
    float effRoll; //0x0330
    int32_t fFOV; //0x0334
    int32_t scopeFOV; //0x0338
    struct weapon* wpnPtrKnife; //0x033C
    struct weapon* wpnPtrPistol; //0x0340
    struct weapon* wpnPtrCarbine; //0x0344
    struct weapon* wpnPtrShotgun; //0x0348
    struct weapon* wpnPtrSubgun; //0x034C
    struct sniperStruct* wpnPtrSniper; //0x0350
    struct weapon* wpnPtrAssault; //0x0354
    struct grenadeStruct* wpnPtrGrenade; //0x0358
    struct akimboStruct* wpnPtrAkimbo; //0x035C
    struct weapon* wpnPtrPrevWpnSelect; //0x0360
    struct weapon* wpnPtrSelect; //0x0364
    struct weapon* wpnPtrNextWpnSelect; //0x0368
    struct weapon* wpnPtrPrimary; //0x036C
    struct weapon* wpnPtrNextPrimary; //0x0370
    struct weapon* wpnPtrLastAttWpn; //0x0374
    int32_t nextUpdate; //0x0378
    int32_t curPos; //0x037C
    int32_t numPos; //0x0380
    char pad_0384[84]; //0x0384
    int8_t* skinNoTeam; //0x03D8
    int8_t* skinCLA; //0x03DC
    int8_t* skinRVSF; //0x03E0
    float deltaYaw; //0x03E4
    float deltaPitch; //0x03E8
    float newYaw; //0x03EC
    float newPitch; //0x03F0

};

struct playerArray {
    std::array<playerEnt*, 32> players;
};

struct glmatrixf {
    std::array<float, 16> v;

    float operator[](size_t i) const { return v[i]; }
    float& operator[](size_t i) { return v[i]; }

    static float det2x2(float a, float b, float c, float d) {
        return a * d - b * c;
    }

    static float det3x3(float a1, float a2, float a3, float b1, float b2, float b3, float c1, float c2, float c3) {
        return 
            a1 * det2x2(b2, b3, c2, c3) -
            b1 * det2x2(a2, a3, c2, c3) +
            c1 * det2x2(a2, a3, b2, b3);
    }

    void rotate_around_x(float angle) {
        float c = std::cos(angle), s = std::sin(angle);
        rotate_vector(v[4], v[8], c, s);
        rotate_vector(v[5], v[9], c, s);
        rotate_vector(v[6], v[10], c, s);
    }

    void rotate_around_y(float angle) {
        float c = std::cos(angle), s = std::sin(angle);
        rotate_vector(v[8], v[0], c, s);
        rotate_vector(v[9], v[1], c, s);
        rotate_vector(v[10], v[2], c, s);
    }

    void rotate_around_z(float angle) {
        float c = std::cos(angle), s = std::sin(angle);
        rotate_vector(v[0], v[4], c, s);
        rotate_vector(v[1], v[5], c, s);
        rotate_vector(v[2], v[6], c, s);
    }

    void rotate_vector(float& A, float& B, float c, float s) {
        float a = A, b = B;
        A = a * c + b * s;
        B = b * c - a * s;
    }

    void multiply(const glmatrixf& other) {
        std::array<float, 16> result{};
        for (size_t row = 0; row < 4; ++row) {
            for (size_t col = 0; col < 4; ++col) {
                result[col + row * 4] = v[col + row * 4] * other.v[col] +
                    v[col + row * 4 + 4] * other.v[col + 1] +
                    v[col + row * 4 + 8] * other.v[col + 2] +
                    v[col + row * 4 + 12] * other.v[col + 3];
            }
        }
        v = result;
    }

    void identity() {
        v.fill(0);
        v[0] = v[5] = v[10] = v[15] = 1.0f;
    }

    void translate(float x, float y, float z) {
        v[12] += x;
        v[13] += y;
        v[14] += z;
    }

    void translate(const Vector3& o) {
        translate(o.x, o.y, o.z);
    }

    void scale(float x, float y, float z) {
        for (size_t i = 0; i < 4; ++i) {
            v[i] *= x;
            v[i + 4] *= y;
            v[i + 8] *= z;
        }
    }

    void projective() {
        for (size_t i = 0; i < 2; ++i) {
            for (size_t j = 0; j < 4; ++j) {
                v[i + j * 4] = 0.5f * (v[i + j * 4] + v[3 + j * 4]);
            }
        }
    }

    void invertnormal(Vector3& dir) const {
        Vector3 n(dir);
        dir.x = n.x * v[0] + n.y * v[1] + n.z * v[2];
        dir.y = n.x * v[4] + n.y * v[5] + n.z * v[6];
        dir.z = n.x * v[8] + n.y * v[9] + n.z * v[10];
    }

    void invertvertex(Vector3& pos) const {
        Vector3 p(pos);
        p.x -= v[12];
        p.y -= v[13];
        p.z -= v[14];
        pos.x = p.x * v[0] + p.y * v[1] + p.z * v[2];
        pos.y = p.x * v[4] + p.y * v[5] + p.z * v[6];
        pos.z = p.x * v[8] + p.y * v[9] + p.z * v[10];
    }

    float transformx(const Vector3& p) const {
        return p.x * v[0] + p.y * v[4] + p.z * v[8] + v[12];
    }

    float transformy(const Vector3& p) const {
        return p.x * v[1] + p.y * v[5] + p.z * v[9] + v[13];
    }

    float transformz(const Vector3& p) const {
        return p.x * v[2] + p.y * v[6] + p.z * v[10] + v[14];
    }

    float transformw(const Vector3& p) const {
        return p.x * v[3] + p.y * v[7] + p.z * v[11] + v[15];
    }

    void transform(const Vector3& in, Vector4& out) const {
        out.x = transformx(in);
        out.y = transformy(in);
        out.z = transformz(in);
        out.w = transformw(in);
    }

    Vector3 gettranslation() const {
        return Vector3(v[12], v[13], v[14]);
    }

    float determinant() const {
        float a1 = v[0], a2 = v[1], a3 = v[2], a4 = v[3],
            b1 = v[4], b2 = v[5], b3 = v[6], b4 = v[7],
            c1 = v[8], c2 = v[9], c3 = v[10], c4 = v[11],
            d1 = v[12], d2 = v[13], d3 = v[14], d4 = v[15];

        return a1 * det3x3(b2, b3, b4, c2, c3, c4, d2, d3, d4)
            - b1 * det3x3(a2, a3, a4, c2, c3, c4, d2, d3, d4)
            + c1 * det3x3(a2, a3, a4, b2, b3, b4, d2, d3, d4)
            - d1 * det3x3(a2, a3, a4, b2, b3, b4, c2, c3, c4);
    }

    void adjoint(const glmatrixf& m) {
        const float* a = m.v.data();

        v[0] = det3x3(a[4], a[5], a[6], a[8], a[9], a[10], a[12], a[13], a[14]);
        v[1] = -det3x3(a[4], a[5], a[6], a[8], a[9], a[10], a[12], a[13], a[14]);
        v[2] = det3x3(a[4], a[5], a[6], a[8], a[9], a[10], a[12], a[13], a[14]);
        v[3] = -det3x3(a[4], a[5], a[6], a[8], a[9], a[10], a[12], a[13], a[14]);

        v[4] = -det3x3(a[0], a[1], a[2], a[8], a[9], a[10], a[12], a[13], a[14]);
        v[5] = det3x3(a[0], a[1], a[2], a[8], a[9], a[10], a[12], a[13], a[14]);
        v[6] = -det3x3(a[0], a[1], a[2], a[8], a[9], a[10], a[12], a[13], a[14]);
        v[7] = det3x3(a[0], a[1], a[2], a[8], a[9], a[10], a[12], a[13], a[14]);

        v[8] = det3x3(a[0], a[1], a[2], a[4], a[5], a[6], a[12], a[13], a[14]);
        v[9] = -det3x3(a[0], a[1], a[2], a[4], a[5], a[6], a[12], a[13], a[14]);
        v[10] = det3x3(a[0], a[1], a[2], a[4], a[5], a[6], a[12], a[13], a[14]);
        v[11] = -det3x3(a[0], a[1], a[2], a[4], a[5], a[6], a[12], a[13], a[14]);

        v[12] = -det3x3(a[0], a[1], a[2], a[4], a[5], a[6], a[8], a[9], a[10]);
        v[13] = det3x3(a[0], a[1], a[2], a[4], a[5], a[6], a[8], a[9], a[10]);
        v[14] = -det3x3(a[0], a[1], a[2], a[4], a[5], a[6], a[8], a[9], a[10]);
        v[15] = det3x3(a[0], a[1], a[2], a[4], a[5], a[6], a[8], a[9], a[10]);
    }

    bool invert(const glmatrixf& m, float mindet = 1.0e-10f) {
        float a1 = m.v[0], b1 = m.v[4], c1 = m.v[8], d1 = m.v[12];
        adjoint(m);
        float det = a1 * v[0] + b1 * v[1] + c1 * v[2] + d1 * v[3];
        if (std::fabs(det) < mindet) return false;
        float invdet = 1 / det;
        for (auto& elem : v) {
            elem *= invdet;
        }
        return true;
    }

    void fromquat(const Quaternion& q) {
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

        v[3] = v[7] = v[11] = v[12] = v[13] = v[14] = 0;
        v[15] = 1;
    }
};
