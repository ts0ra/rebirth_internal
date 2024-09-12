#include "types.h"

// Vector2 Implementations
Vector2::Vector2(float x, float y) : x(x), y(y) {}
Vector2 Vector2::operator+(const Vector2& other) const noexcept { return { x + other.x, y + other.y }; }
Vector2 Vector2::operator-(const Vector2& other) const noexcept { return { x - other.x, y - other.y }; }
Vector2 Vector2::operator*(float scalar) const noexcept { return { x * scalar, y * scalar }; }
Vector2 Vector2::operator/(float scalar) const noexcept { return { x / scalar, y / scalar }; }

// Vector3 Implementations
Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
Vector3::Vector3(const float* values) : x(values[0]), y(values[1]), z(values[2]) {}
float& Vector3::operator[](std::size_t i) noexcept {
    return (i == 0 ? x : (i == 1 ? y : z));
}
float Vector3::operator[](std::size_t i) const noexcept {
    return (i == 0 ? x : (i == 1 ? y : z));
}
bool Vector3::isZero() const noexcept { return x == 0 && y == 0 && z == 0; }
bool Vector3::operator==(const Vector3& other) const noexcept { return x == other.x && y == other.y && z == other.z; }
bool Vector3::operator!=(const Vector3& other) const noexcept { return !(*this == other); }
Vector3 Vector3::operator-() const noexcept { return { -x, -y, -z }; }
Vector3& Vector3::multiply(float scalar) noexcept { x *= scalar; y *= scalar; z *= scalar; return *this; }
Vector3& Vector3::divide(float scalar) noexcept { x /= scalar; y /= scalar; z /= scalar; return *this; }
Vector3& Vector3::add(float scalar) noexcept { x += scalar; y += scalar; z += scalar; return *this; }
Vector3& Vector3::subtract(float scalar) noexcept { x -= scalar; y -= scalar; z -= scalar; return *this; }
Vector3 Vector3::operator+(const Vector3& other) const noexcept { return { x + other.x, y + other.y, z + other.z }; }
Vector3 Vector3::operator-(const Vector3& other) const noexcept { return { x - other.x, y - other.y, z - other.z }; }
Vector3 Vector3::operator*(float scalar) const noexcept { return { x * scalar, y * scalar, z * scalar }; }
Vector3 Vector3::operator/(float scalar) const noexcept { return { x / scalar, y / scalar, z / scalar }; }
Vector3& Vector3::add(const Vector3& other) noexcept { x += other.x; y += other.y; z += other.z; return *this; }
Vector3& Vector3::subtract(const Vector3& other) noexcept { x -= other.x; y -= other.y; z -= other.z; return *this; }
float Vector3::squaredLength() const noexcept { return x * x + y * y + z * z; }
float Vector3::squaredXY() const noexcept { return x * x + y * y; }
float Vector3::dot(const Vector3& other) const noexcept { return x * other.x + y * other.y + z * other.z; }
float Vector3::dotXY(const Vector3& other) const noexcept { return x * other.x + y * other.y; }
float Vector3::magnitude() const noexcept { return std::sqrt(squaredLength()); }
Vector3& Vector3::normalize() noexcept { return divide(magnitude()); }
float Vector3::distance(const Vector3& other) const noexcept { Vector3 temp = *this; return distance(other, temp); }
float Vector3::distance(const Vector3& other, Vector3& temp) const noexcept {
    temp = *this; temp.subtract(other);
    return temp.magnitude();
}
float Vector3::squaredDistance(const Vector3& other) const noexcept { 
    Vector3 temp = *this; 
    temp.subtract(other); 
    return temp.squaredLength(); 
}
float Vector3::distanceXY(const Vector3& other) const noexcept {
    float dx = other.x - x, dy = other.y - y;
    return std::sqrt(dx * dx + dy * dy);
}
float Vector3::magnitudeXY() const noexcept { return std::sqrt(x * x + y * y); }
float Vector3::angleXY() const noexcept { return std::atan2(y, x) / RAD; }
bool Vector3::reject(const Vector3& other, float max) const noexcept {
    return x > other.x + max || x < other.x - max ||
        y > other.y + max || y < other.y - max;
}
Vector3& Vector3::cross(const Vector3& a, const Vector3& b) noexcept {
    x = a.y * b.z - a.z * b.y;
    y = a.z * b.x - a.x * b.z;
    z = a.x * b.y - a.y * b.x;
    return *this;
}

float Vector3::crossXY(const Vector3& a) const noexcept {
    return x * a.y - y * a.x;
}

void Vector3::rotateAroundZ(float angle) noexcept {
    float cosA = std::cos(angle);
    float sinA = std::sin(angle);
    *this = { cosA * x - sinA * y, sinA * x + cosA * y, z };
}

void Vector3::rotateAroundX(float angle) noexcept {
    float cosA = std::cos(angle);
    float sinA = std::sin(angle);
    *this = { x, cosA * y - sinA * z, cosA * z + sinA * y };
}

void Vector3::rotateAroundY(float angle) noexcept {
    float cosA = std::cos(angle);
    float sinA = std::sin(angle);
    *this = { cosA * x - sinA * z, y, sinA * x + cosA * z };
}

Vector3& Vector3::rotate(float angle, const Vector3& axis) noexcept {
    float c = std::cos(angle);
    float s = std::sin(angle);
    return rotate(c, s, axis);
}

Vector3& Vector3::rotate(float c, float s, const Vector3& axis) noexcept {
    float xx = axis.x * axis.x;
    float yy = axis.y * axis.y;
    float zz = axis.z * axis.z;
    float xy = axis.x * axis.y;
    float xz = axis.x * axis.z;
    float yz = axis.y * axis.z;
    float xs = axis.x * s;
    float ys = axis.y * s;
    float zs = axis.z * s;
    float oneMinusC = 1.0f - c;

    *this = { x * (xx * oneMinusC + c) + y * (xy * oneMinusC - zs) + z * (xz * oneMinusC + ys),
             x * (xy * oneMinusC + zs) + y * (yy * oneMinusC + c) + z * (yz * oneMinusC - xs),
             x * (xz * oneMinusC - ys) + y * (yz * oneMinusC + xs) + z * (zz * oneMinusC + c) };
    return *this;
}

void Vector3::orthogonal(const Vector3& d) noexcept {
    std::size_t i = std::fabs(d.x) > std::fabs(d.y)
        ? (std::fabs(d.x) > std::fabs(d.z) ? 0 : 2)
        : (std::fabs(d.y) > std::fabs(d.z) ? 1 : 2);
    x = d[(i + 1) % 3];
    y = -d[i];
    z = 0;
}

// Vector4 Implementations
Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
Vector4::Vector4(const Vector3& p, float w) : x(p.x), y(p.y), z(p.z), w(w) {}
float& Vector4::operator[](std::size_t i) noexcept {
    return (i == 0 ? x : (i == 1 ? y : (i == 2 ? z : w)));
}
float Vector4::operator[](std::size_t i) const noexcept {
    return (i == 0 ? x : (i == 1 ? y : (i == 2 ? z : w)));
}
Vector4 Vector4::operator+(const Vector4& other) const noexcept {
    return { x + other.x, y + other.y, z + other.z, w + other.w };
}

Vector4 Vector4::operator-(const Vector4& other) const noexcept {
    return { x - other.x, y - other.y, z - other.z, w - other.w };
}

Vector4 Vector4::operator*(float scalar) const noexcept {
    return { x * scalar, y * scalar, z * scalar, w * scalar };
}

Vector4 Vector4::operator/(float scalar) const noexcept {
    return { x / scalar, y / scalar, z / scalar, w / scalar };
}

void Vector4::toNDC(Vector3& out) const noexcept {
    out = { x / w, y / w, z / w };
}

// Quaternion Implementations
constexpr Quaternion::Quaternion(float x, float y, float z, float w) noexcept
    : x(x), y(y), z(z), w(w) {}

Quaternion::Quaternion(const Vector3& axis, float angle) noexcept {
    w = std::cos(angle / 2);
    float s = std::sin(angle / 2);
    x = s * axis.x;
    y = s * axis.y;
    z = s * axis.z;
}

Quaternion::Quaternion(float yaw, float pitch) noexcept {
    yaw *= RAD / 2;
    pitch *= RAD / 2;
    float yw = std::cos(yaw);
    float ys = std::sin(yaw);
    float pw = std::cos(pitch);
    float ps = std::sin(pitch);
    x = ps * ys;
    y = ps * yw;
    z = pw * ys;
    w = pw * yw;
}

Quaternion& Quaternion::roll(float roll) noexcept {
    roll *= RAD / 2;
    float px = std::sin(roll);
    float pw = std::cos(roll);
    Quaternion temp = *this;
    x = pw * temp.x + px * temp.w;
    y = pw * temp.y - px * temp.z;
    z = pw * temp.z + px * temp.y;
    w = pw * temp.w - px * temp.x;
    return *this;
}

