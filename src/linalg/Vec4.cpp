#include <math.h>
#include "linalg/Vec4.h"

// Vec4 is treated as a Vec3, but it has
// a homogeneous value which is 1 by default

Vec4 Vec4::operator+(const Vec4& v) {
    Vec4 vec;
    vec.x = this->x + v.x; vec.y = this->y + v.y; vec.z = this->z + v.z;
    return vec;
}

void Vec4::operator+=(const Vec4& v) {
    this->x += v.x; this->y += v.y; this->z += v.z;
}

Vec4 Vec4::operator-(const Vec4& v) {
    Vec4 vec;
    vec.x = this->x - v.x; vec.y = this->y - v.y; vec.z = this->z - v.z;
    return vec;
}

void Vec4::operator-=(const Vec4& v) {
    this->x -= v.x; this->y -= v.y; this->z -= v.z;
}

Vec4 Vec4::operator-() {
    Vec4 vec;
    vec.x = -this->x; vec.y = -this->y; vec.z = -this->z;
    return vec;
}

Vec4 Vec4::operator*(const float k) {
    Vec4 vec;
    vec.x = this->x * k; vec.y = this->y * k; vec.z = this->z * k;
    return vec;
}

Vec4 Vec4::operator/(const float k) {
    Vec4 vec;
    vec.x = this->x / k; vec.y = this->y / k; vec.z = this->z / k;
    return vec;
}

float dot(Vec4& v1, Vec4& v2) {
    return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

float Vec4::length() {
    return sqrt(dot(*this, *this));
}

Vec4 normalise(Vec4& v) {
    float l = v.length();
    return { v.x / l, v.y / l, v.z / l };
}

Vec4 cross(Vec4& v1, Vec4& v2) {
    Vec4 v;
    v.x = v1.y * v2.z - v1.z * v2.y;
    v.y = v1.z * v2.x - v1.x * v2.z;
    v.z = v1.x * v2.y - v1.y * v2.x;
    return v;
}