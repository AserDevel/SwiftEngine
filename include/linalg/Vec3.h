#ifndef VEC3_H
#define VEC3_H

struct Vec3 {
    union { float x = 0, r, u; };
    union { float y = 0, g, v; };
    union { float z = 0, b, w; };

    Vec3(float a = 0, float b = 0, float c = 0) : 
        x(a), y(b), z(c) {};

    float length();
    
    Vec3 operator+(const Vec3& v);
    void operator+=(const Vec3& v);
    Vec3 operator-(const Vec3& v);
    void operator-=(const Vec3& v);
    Vec3 operator-();
    Vec3 operator*(const float k);
    Vec3 operator/(const float k);
};

float dot(Vec3& v1, Vec3& v2);

Vec3 cross(Vec3& v1, Vec3& v2);

Vec3 normalise(Vec3& v);

#endif