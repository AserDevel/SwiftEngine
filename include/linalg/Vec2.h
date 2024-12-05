#ifndef VEC2_H
#define VEC2_H

struct Vec2 {
    union { float x = 0, u; };
    union { float y = 0, v; };

    Vec2(float x = 0, float y = 0) : 
        x(x), y(y) {}

    float length();
    
    Vec2 operator+(const Vec2& v);
    void operator+=(const Vec2& v);
    Vec2 operator-(const Vec2& v);
    void operator-=(const Vec2& v);
    Vec2 operator-();
    Vec2 operator*(const float k);
    Vec2 operator/(const float k);
};

float dot(Vec2& v1, Vec2& v2);

float det(Vec2& v1, Vec2& v2);

Vec2 normalise(Vec2& v);


#endif