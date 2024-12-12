#ifndef VEC2_H
#define VEC2_H

#include <math.h>

struct Vec2 {
    union { float x = 0, u; };
    union { float y = 0, v; };

    inline Vec2(float x = 0, float y = 0) : 
        x(x), y(y) {}
    
    inline Vec2 operator+(const Vec2& v) {
        Vec2 vec;
        vec.x = this->x + v.x; vec.y = this->y + v.y;
        return vec;
    }

    inline void operator+=(const Vec2& v) {
        this->x += v.x; this->y += v.y;
    }

    inline Vec2 operator-(const Vec2& v) {
        Vec2 vec;
        vec.x = this->x - v.x; vec.y = this->y - v.y;
        return vec;
    }

    inline Vec2 operator*(const Vec2& v) {
        return { this->x * v.x, this->y * v.y };
    }

    inline Vec2 operator/(const Vec2& v) {
        return { this->x / v.x, this->y / v.y };
    }

    inline void operator-=(const Vec2& v) {
        this->x -= v.x; this->y -= v.y;
    }

    inline Vec2 operator-() {
        Vec2 vec;
        vec.x = -this->x; vec.y = -this->y;
        return vec;
    }

    inline Vec2 operator*(const float k) {
        Vec2 vec;
        vec.x = this->x * k; vec.y = this->y * k;
        return vec;
    }

    inline Vec2 operator/(const float k) {
        Vec2 vec;
        vec.x = this->x / k; vec.y = this->y / k;
        return vec;
    }
};

inline float dot(Vec2& v1, Vec2& v2) {
    return (v1.x * v2.x + v1.y * v2.y);
}

inline float length(Vec2 v) {
    return sqrt(dot(v, v));
}

inline Vec2 normalise(Vec2& v) {
    float l = length(v);
    return { v.x / l, v.y / l };
}

inline float det(Vec2& v1, Vec2& v2) {
    return ((v1.x * v2.y) - (v2.x * v1.y));
}


#endif