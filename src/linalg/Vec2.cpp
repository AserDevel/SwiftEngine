#include <math.h>
#include "linalg/Vec2.h"

Vec2 Vec2::operator+(const Vec2& v) {
    Vec2 vec;
    vec.x = this->x + v.x; vec.y = this->y + v.y;
    return vec;
}

void Vec2::operator+=(const Vec2& v) {
    this->x += v.x; this->y += v.y;
}

Vec2 Vec2::operator-(const Vec2& v) {
    Vec2 vec;
    vec.x = this->x - v.x; vec.y = this->y - v.y;
    return vec;
}

void Vec2::operator-=(const Vec2& v) {
    this->x -= v.x; this->y -= v.y;
}

Vec2 Vec2::operator-() {
    Vec2 vec;
    vec.x = -this->x; vec.y = -this->y;
    return vec;
}

Vec2 Vec2::operator*(const float k) {
    Vec2 vec;
    vec.x = this->x * k; vec.y = this->y * k;
    return vec;
}

Vec2 Vec2::operator/(const float k) {
    Vec2 vec;
    vec.x = this->x / k; vec.y = this->y / k;
    return vec;
}

float dot(Vec2& v1, Vec2& v2) {
    return (v1.x * v2.x + v1.y * v2.y);
}

float Vec2::length() {
    return sqrt(dot(*this, *this));
}

Vec2 normalise(Vec2& v) {
    float l = v.length();
    return { v.x / l, v.y / l };
}

float det(Vec2& v1, Vec2& v2) {
    return ((v1.x * v2.y) - (v2.x * v1.y));
}