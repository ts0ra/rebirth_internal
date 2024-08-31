#pragma once

#include "types.h"

#include <array>
#include <cmath>
#include <cstring>
#include <cstdint>

using uchar = unsigned char;

struct entity {
    short x;            // 0x0000
    short y;            // 0x0002
    short z;            // 0x0004
    short attr1;        // 0x0006
    uchar type;         // 0x0008
    uchar attr2;        // 0x0009
    uchar attr3;        // 0x000A
    uchar attr4;        // 0x000B
    short attr5;        // 0x000C
    char attr6;         // 0x000E
    uchar attr7;        // 0x000F
    bool spawned;       // 0x0010
    char pad_0011[3];   // 0x0011 / code cave
    int lastmillis;     // 0x0014
};                      // Size: 0x0018

struct gunInfo {
    char modelName[23]; // 0x0000
    char weaponName[42]; // 0x0017
    char pad[1]; // 0x0041 unused
    short shootFX; // 0x0042
    short reloadFX; // 0x0044
    short reloadTime; // 0x0046
    short shootDelay; // 0x0048
    short damage; // 0x004A
    short piercing; // 0x004C
    short projectileSpeed; // 0x004E
    short part; // 0x0050
    short spread; // 0x0052
    short recoil; // 0x0054
    short magsize; // 0x0056
    short mdl_kick_rot; // 0x0058
    short mdl_kick_back; // 0x005A
    short recoilIncrease; // 0x005C
    short recoilBase; // 0x005E
    short maxRecoil; // 0x0060
    short recoilBackfade; // 0x0062
    short pushFactor; // 0x0064
    bool isAuto; // 0x0066
}; // Size: 0x0067

struct weapon {
    char pad1[4];           // 0x0000
    int gunId;              // 0x0004
    char pad2[4];           // 0x0008
    gunInfo* ptrToGunInfo; // 0x000C
    int* ptrToReserveAmmo;      // 0x0010
    int* ptrToLoadedAmmo;   // 0x0014
    int* ptrToWeaponDelay;  // 0x0018
    int shotCount;          // 0x001C
};

struct playerEnt {
    char pad1[4];   // 0x0000
    Vector3 headPos;        // 0x0004
    char pad2[24];  // 0x0010
    Vector3 footPos; // 0x0028
    Vector3 viewAngles; // 0x0034
    char pad3[172]; // 0x0040
    int health; // 0x00EC
    int armor; // 0x00F0
    char pad4[273]; // 0x00F4
    char username[16]; // 0x0205
    char pad5[247]; // 0x0215
    int teamSide; // 0x030C
    char pad6[8]; // 0x0310
    bool isDead; // 0x0318
    char pad7[75]; // 0x0319
    weapon* ptrToCurrentWeapon; // 0x0364
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
