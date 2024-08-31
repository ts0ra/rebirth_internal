#pragma once

#define loop(v,m) for(int v = 0; v<int(m); v++)
#define loopi(m) loop(i,m)
#define loopj(m) loop(j,m)

#include "utils.h"

#include <cstring>
#include <cmath>

struct glmatrixf
{
    float v[16];

    float operator[](int i) const { return v[i]; }
    float& operator[](int i) { return v[i]; }

#define ROTVEC(A, B) \
    { \
        float a = A, b = B; \
        A = a*c + b*s; \
        B = b*c - a*s; \
    }

    void rotate_around_x(float angle)
    {
        float c = cosf(angle), s = sinf(angle);
        ROTVEC(v[4], v[8]);
        ROTVEC(v[5], v[9]);
        ROTVEC(v[6], v[10]);
    }

    void rotate_around_y(float angle)
    {
        float c = cosf(angle), s = sinf(angle);
        ROTVEC(v[8], v[0]);
        ROTVEC(v[9], v[1]);
        ROTVEC(v[10], v[2]);
    }

    void rotate_around_z(float angle)
    {
        float c = cosf(angle), s = sinf(angle);
        ROTVEC(v[0], v[4]);
        ROTVEC(v[1], v[5]);
        ROTVEC(v[2], v[6]);
    }

#undef ROTVEC

#define MULMAT(row, col) \
       v[col + row] = x[row]*y[col] + x[row + 4]*y[col + 1] + x[row + 8]*y[col + 2] + x[row + 12]*y[col + 3];

    template<class XT, class YT>
    void mul(const XT x[16], const YT y[16])
    {
        MULMAT(0, 0); MULMAT(1, 0); MULMAT(2, 0); MULMAT(3, 0);
        MULMAT(0, 4); MULMAT(1, 4); MULMAT(2, 4); MULMAT(3, 4);
        MULMAT(0, 8); MULMAT(1, 8); MULMAT(2, 8); MULMAT(3, 8);
        MULMAT(0, 12); MULMAT(1, 12); MULMAT(2, 12); MULMAT(3, 12);
    }

#undef MULMAT

    void mul(const glmatrixf& x, const glmatrixf& y)
    {
        mul(x.v, y.v);
    }

    void identity()
    {
        static const float m[16] =
        {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };
        memcpy(v, m, sizeof(v));
    }

    void translate(float x, float y, float z)
    {
        v[12] += x;
        v[13] += y;
        v[14] += z;
    }

    void translate(const vec& o)
    {
        translate(o.x, o.y, o.z);
    }

    void scale(float x, float y, float z)
    {
        v[0] *= x; v[1] *= x; v[2] *= x; v[3] *= x;
        v[4] *= y; v[5] *= y; v[6] *= y; v[7] *= y;
        v[8] *= z; v[9] *= z; v[10] *= z; v[11] *= z;
    }

    void projective()
    {
        loopi(2) loopj(4) v[i + j * 4] = 0.5f * (v[i + j * 4] + v[3 + j * 4]);
    }

    void invertnormal(vec& dir) const
    {
        vec n(dir);
        dir.x = n.x * v[0] + n.y * v[1] + n.z * v[2];
        dir.y = n.x * v[4] + n.y * v[5] + n.z * v[6];
        dir.z = n.x * v[8] + n.y * v[9] + n.z * v[10];
    }

    void invertvertex(vec& pos) const
    {
        vec p(pos);
        p.x -= v[12];
        p.y -= v[13];
        p.z -= v[14];
        pos.x = p.x * v[0] + p.y * v[1] + p.z * v[2];
        pos.y = p.x * v[4] + p.y * v[5] + p.z * v[6];
        pos.z = p.x * v[8] + p.y * v[9] + p.z * v[10];
    }

    float transformx(const vec& p) const
    {
        return p.x * v[0] + p.y * v[4] + p.z * v[8] + v[12];
    }

    float transformy(const vec& p) const
    {
        return p.x * v[1] + p.y * v[5] + p.z * v[9] + v[13];
    }

    float transformz(const vec& p) const
    {
        return p.x * v[2] + p.y * v[6] + p.z * v[10] + v[14];
    }

    float transformw(const vec& p) const
    {
        return p.x * v[3] + p.y * v[7] + p.z * v[11] + v[15];
    }

    void transform(const vec& in, vec4& out) const
    {
        out.x = transformx(in);
        out.y = transformy(in);
        out.z = transformz(in);
        out.w = transformw(in);
    }

    vec gettranslation() const
    {
        return vec(v[12], v[13], v[14]);
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
    void adjoint(const glmatrixf& m)
    {
        float a1 = m.v[0], a2 = m.v[1], a3 = m.v[2], a4 = m.v[3],
            b1 = m.v[4], b2 = m.v[5], b3 = m.v[6], b4 = m.v[7],
            c1 = m.v[8], c2 = m.v[9], c3 = m.v[10], c4 = m.v[11],
            d1 = m.v[12], d2 = m.v[13], d3 = m.v[14], d4 = m.v[15];

        v[0] = utils::det3x3(b2, b3, b4, c2, c3, c4, d2, d3, d4);
        v[1] = -utils::det3x3(a2, a3, a4, c2, c3, c4, d2, d3, d4);
        v[2] = utils::det3x3(a2, a3, a4, b2, b3, b4, d2, d3, d4);
        v[3] = -utils::det3x3(a2, a3, a4, b2, b3, b4, c2, c3, c4);

        v[4] = -utils::det3x3(b1, b3, b4, c1, c3, c4, d1, d3, d4);
        v[5] = utils::det3x3(a1, a3, a4, c1, c3, c4, d1, d3, d4);
        v[6] = -utils::det3x3(a1, a3, a4, b1, b3, b4, d1, d3, d4);
        v[7] = utils::det3x3(a1, a3, a4, b1, b3, b4, c1, c3, c4);

        v[8] = utils::det3x3(b1, b2, b4, c1, c2, c4, d1, d2, d4);
        v[9] = -utils::det3x3(a1, a2, a4, c1, c2, c4, d1, d2, d4);
        v[10] = utils::det3x3(a1, a2, a4, b1, b2, b4, d1, d2, d4);
        v[11] = -utils::det3x3(a1, a2, a4, b1, b2, b4, c1, c2, c4);

        v[12] = -utils::det3x3(b1, b2, b3, c1, c2, c3, d1, d2, d3);
        v[13] = utils::det3x3(a1, a2, a3, c1, c2, c3, d1, d2, d3);
        v[14] = -utils::det3x3(a1, a2, a3, b1, b2, b3, d1, d2, d3);
        v[15] = utils::det3x3(a1, a2, a3, b1, b2, b3, c1, c2, c3);
    }
    bool invert(const glmatrixf& m, float mindet = 1.0e-10f)
    {
        float a1 = m.v[0], b1 = m.v[4], c1 = m.v[8], d1 = m.v[12];
        adjoint(m);
        float det = a1 * v[0] + b1 * v[1] + c1 * v[2] + d1 * v[3]; // float det = m.determinant();
        if (fabs(det) < mindet) return false;
        float invdet = 1 / det;
        loopi(16) v[i] *= invdet;
        return true;
    }

    void fromquat(quat& q)
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

        v[3] = v[7] = v[11] = v[12] = v[13] = v[14] = 0;
        v[15] = 1;
    }
};