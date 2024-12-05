#include <math.h>
#include "linalg/Vec3.h"

Vec3 Vec3::operator+(const Vec3& v) {
    Vec3 vec;
    vec.x = this->x + v.x; vec.y = this->y + v.y; vec.z = this->z + v.z;
    return vec;
}

void Vec3::operator+=(const Vec3& v) {
    this->x += v.x; this->y += v.y; this->z += v.z;
}

Vec3 Vec3::operator-(const Vec3& v) {
    Vec3 vec;
    vec.x = this->x - v.x; vec.y = this->y - v.y; vec.z = this->z - v.z;
    return vec;
}

void Vec3::operator-=(const Vec3& v) {
    this->x -= v.x; this->y -= v.y; this->z -= v.z;
}

Vec3 Vec3::operator-() {
    Vec3 vec;
    vec.x = -this->x; vec.y = -this->y; vec.z = -this->z;
    return vec;
}

Vec3 Vec3::operator*(const float k) {
    Vec3 vec;
    vec.x = this->x * k; vec.y = this->y * k; vec.z = this->z * k;
    return vec;
}

Vec3 Vec3::operator/(const float k) {
    Vec3 vec;
    vec.x = this->x / k; vec.y = this->y / k; vec.z = this->z / k;
    return vec;
}

float dot(Vec3& v1, Vec3& v2) {
    return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

float Vec3::length() {
    return sqrt(dot(*this, *this));
}

Vec3 normalise(Vec3& v) {
    float l = v.length();
    return { v.x / l, v.y / l, v.z / l };
}

Vec3 cross(Vec3& v1, Vec3& v2) {
    Vec3 v;
    v.x = v1.y * v2.z - v1.z * v2.y;
    v.y = v1.z * v2.x - v1.x * v2.z;
    v.z = v1.x * v2.y - v1.y * v2.x;
    return v;
}