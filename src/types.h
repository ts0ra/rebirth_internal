#pragma once

#include <cmath>

// Constants
constexpr float RAD = 0.01745329252f;

// 2D Vector
struct Vector2 {
    float x = 0.0f, y = 0.0f;

    Vector2() = default;
    Vector2(float x, float y);

    Vector2 operator+(const Vector2& other) const noexcept;
    Vector2 operator-(const Vector2& other) const noexcept;
    Vector2 operator*(float scalar) const noexcept;
    Vector2 operator/(float scalar) const noexcept;
};

// 3D Vector
struct Vector3 {
    float x = 0.0f, y = 0.0f, z = 0.0f;

    Vector3() = default;
    Vector3(float x, float y, float z);
    Vector3(const float* values);

    float& operator[](std::size_t i) noexcept;
    float operator[](std::size_t i) const noexcept;

    bool isZero() const noexcept;
    bool operator==(const Vector3& other) const noexcept;
    bool operator!=(const Vector3& other) const noexcept;
    Vector3 operator-() const noexcept;

    Vector3& multiply(float scalar) noexcept;
    Vector3& divide(float scalar) noexcept;
    Vector3& add(float scalar) noexcept;
    Vector3& subtract(float scalar) noexcept;

    Vector3 operator+(const Vector3& other) const noexcept;
    Vector3 operator-(const Vector3& other) const noexcept;
    Vector3 operator*(float scalar) const noexcept;
    Vector3 operator/(float scalar) const noexcept;

    Vector3& add(const Vector3& other) noexcept;
    Vector3& subtract(const Vector3& other) noexcept;

    float squaredLength() const noexcept;
    float squaredXY() const noexcept;
    float dot(const Vector3& other) const noexcept;
    float dotXY(const Vector3& other) const noexcept;
    float magnitude() const noexcept;
    Vector3& normalize() noexcept;

    float distance(const Vector3& other) const noexcept;
    float distance(const Vector3& other, Vector3& temp) const noexcept;
    float squaredDistance(const Vector3& other) const noexcept;

    float distanceXY(const Vector3& other) const noexcept;
    float magnitudeXY() const noexcept;
    float angleXY() const noexcept;

    bool reject(const Vector3& other, float max) const noexcept;

    Vector3& cross(const Vector3& a, const Vector3& b) noexcept;
    float crossXY(const Vector3& a) const noexcept;

    void rotateAroundZ(float angle) noexcept;
    void rotateAroundX(float angle) noexcept;
    void rotateAroundY(float angle) noexcept;

    Vector3& rotate(float angle, const Vector3& axis) noexcept;
    Vector3& rotate(float c, float s, const Vector3& axis) noexcept;

    void orthogonal(const Vector3& d) noexcept;
};

// 4D Vector
struct Vector4 {
    float x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;

    Vector4() = default;
    Vector4(float x, float y, float z, float w);
    explicit Vector4(const Vector3& p, float w = 0);

    float& operator[](std::size_t i) noexcept;
    float operator[](std::size_t i) const noexcept;

    Vector4 operator+(const Vector4& other) const noexcept;
    Vector4 operator-(const Vector4& other) const noexcept;
    Vector4 operator*(float scalar) const noexcept;
    Vector4 operator/(float scalar) const noexcept;

    void toNDC(Vector3& out) const noexcept;
};

// Quaternion
struct Quaternion {
    float x, y, z, w;

    constexpr Quaternion(float x = 0, float y = 0, float z = 0, float w = 0) noexcept;
    Quaternion(const Vector3& axis, float angle) noexcept;
    Quaternion(float yaw, float pitch) noexcept;

    Quaternion& roll(float roll) noexcept;
};
